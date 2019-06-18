/**
 * @file AntropseTests.cpp
 * @brief Tests for the system
 *
 * @author Arno Deceuninck
 *
 * @date 14/03/2019
 *
 * @note This file is based on the TicTacToeTests from Serge Demeyer
 */

#include <iostream>
#include <gtest/gtest.h>
#include <fstream>
#include "Vehicle.h"
#include "RoadNetwork.h"
#include "Road.h"
#include "CONST.h"
#include "NetworkImporter.h"
#include "AntropseUtils.h"

class NetworkImporterTests : public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    // virtual void SetUp() will be called before each test is run.  You
    // should define it if you need to initialize the variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {
//        roadNetwork = RoadNetwork();
        importResult = SuccessEnum();
    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    virtual void TearDown() {
    }

    // Declares the variables your tests want to use.
    SuccessEnum importResult;
    RoadNetwork *roadNetwork;
    Vehicle *testVehicle;
    Road *testRoad;
};

// Tests the default constructor.
TEST_F(NetworkImporterTests, DefaultReadFile) {

    std::string nameTest = "DefaultReadFile";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
//    importResult = NetworkImporter::importRoadNetwork(ifname, std::cout, roadNetwork);
    outputFile.close();

    EXPECT_TRUE(fileIsEmpty(ofname));

    EXPECT_EQ(Success, importResult);

    EXPECT_EQ(2, roadNetwork->nrOfCars());
    EXPECT_EQ(2, roadNetwork->nrOfRoads());

    testRoad = roadNetwork->findRoad("E313");
    EXPECT_EQ(120, testRoad->getSpeedLimit(0));
    EXPECT_EQ(5000, testRoad->getLength());
    EXPECT_TRUE(NULL == testRoad->getIntersection());

    testRoad = roadNetwork->findRoad("E19");
    EXPECT_EQ(100, testRoad->getSpeedLimit(0));
    EXPECT_EQ(2000, testRoad->getLength());
    EXPECT_EQ("E313", testRoad->getIntersection()->getName());

    testVehicle = roadNetwork->getCars()[0];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("1THK180", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(0, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    testVehicle = roadNetwork->getCars()[1];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("651BUF", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(20, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    EXPECT_TRUE(fileIsEmpty(ofname));
}

TEST_F(NetworkImporterTests, SomeoneFloating) {

    std::string nameTest = "SomeoneFloating";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars()); // Floating point speed -> not importing
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    testRoad = roadNetwork->findRoad("E19");
    EXPECT_EQ(90, testRoad->getSpeedLimit(0));
    EXPECT_EQ(30, testRoad->getLength());
    EXPECT_TRUE(NULL == testRoad->getIntersection());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));

    delete roadNetwork;
}

TEST_F(NetworkImporterTests, NoPersonalSpace) {
    std::string nameTest = "NoPersonalSpace";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";

    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport,
              importResult); // De auto die te dicht bij een bestaande wagen geplaatst wordt, mag niet geplaatst worden

    // De tweede auto zou niet op het netwerk geplaatst mogen worden omdat deze de verkeerssituatie inconsistent maakt
    EXPECT_EQ(1, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    testRoad = roadNetwork->findRoad("E19");
    EXPECT_EQ(100, testRoad->getSpeedLimit(0));
    EXPECT_EQ(1000, testRoad->getLength());
    EXPECT_TRUE(NULL == testRoad->getIntersection());

    testVehicle = roadNetwork->getCars()[0];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("1THK180", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(4, testVehicle->getCurrentPosition());
    EXPECT_EQ(70, testVehicle->getCurrentSpeed());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));

    delete roadNetwork;
}

TEST_F(NetworkImporterTests, RocketHigh) {
    std::string nameTest = "RocketHigh";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";

    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport, importResult);

    // De auto zou niet op het netwerk geplaatst mogen worden omdat deze de verkeerssituatie inconsistent maakt
    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    testRoad = roadNetwork->findRoad("E19");
    EXPECT_EQ(90, testRoad->getSpeedLimit(0));
    EXPECT_EQ(30, testRoad->getLength());
    EXPECT_TRUE(NULL == testRoad->getIntersection());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));

    delete roadNetwork;
}

TEST_F(NetworkImporterTests, WayTooLow) {
    std::string nameTest = "WayTooLow";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";

    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport, importResult);

    // De auto zou niet op het netwerk geplaatst mogen worden omdat deze de verkeerssituatie inconsistent maakt
    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    testRoad = roadNetwork->findRoad("E19");
    EXPECT_EQ(90, testRoad->getSpeedLimit(0));
    EXPECT_EQ(30, testRoad->getLength());
    EXPECT_TRUE(NULL == testRoad->getIntersection());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));

    delete roadNetwork;
}

TEST_F(NetworkImporterTests, ByeByeFile) {
    std::string nameTest = "ByeByeFile";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";

    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(ImportAborted, importResult);

    // Er mag niets op het netwerk geplaatst zijn, want het inputbestand bestaat niet
    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(0, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));

    delete roadNetwork;
}

TEST_F(NetworkImporterTests, BusyDay) {

    std::string nameTest = "BusyDay";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
//    importResult = NetworkImporter::importRoadNetwork(ifname, std::cout, roadNetwork);
    outputFile.close();

    EXPECT_TRUE(fileIsEmpty(ofname));

    EXPECT_EQ(Success, importResult);

    EXPECT_EQ(7, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    testRoad = roadNetwork->findRoad("E19");
    EXPECT_EQ(30, testRoad->getSpeedLimit(0));
    EXPECT_EQ(1000, testRoad->getLength());
    EXPECT_TRUE(NULL == testRoad->getIntersection());

    testVehicle = roadNetwork->getCars()[0];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("1THK180", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(0, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    testVehicle = roadNetwork->getCars()[1];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("651BUF1", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(20, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    testVehicle = roadNetwork->getCars()[2];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("651BUF2", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(40, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    testVehicle = roadNetwork->getCars()[3];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("651BUF3", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(60, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    testVehicle = roadNetwork->getCars()[4];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("651BUF4", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(80, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    testVehicle = roadNetwork->getCars()[5];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("651BUF5", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(100, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    testVehicle = roadNetwork->getCars()[6];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("651BUF6", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(120, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    EXPECT_TRUE(fileIsEmpty(ofname));

    delete roadNetwork;
}

TEST_F(NetworkImporterTests, WithoutRoot) {

    std::string nameTest = "WithoutRoot";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";

    std::string expectedOfName = "tests/inputTests/output/expected/" + nameTest + ".txt";;


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
//    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_TRUE(fileCompare(ofname, expectedOfName));

    EXPECT_EQ(ImportAborted, importResult);
}

TEST_F(NetworkImporterTests, NoClosingRoot) {

    std::string nameTest = "NoClosingRoot";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";

    std::string expectedOfName = "tests/inputTests/output/expected/" + nameTest + ".txt";;


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
//    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_TRUE(fileCompare(ofname, expectedOfName));

    EXPECT_EQ(ImportAborted, importResult);
}

TEST_F(NetworkImporterTests, NoOpeningRoot) {

    std::string nameTest = "NoOpeningRoot";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";

    std::string expectedOfName = "tests/inputTests/output/expected/" + nameTest + ".txt";;


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_TRUE(fileCompare(ofname, expectedOfName));
    EXPECT_EQ(ImportAborted, importResult);
}

TEST_F(NetworkImporterTests, MotorBike) {

    std::string nameTest = "MotorBike";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
//    importResult = NetworkImporter::importRoadNetwork(ifname, std::cout, roadNetwork);
    outputFile.close();

    EXPECT_TRUE(fileIsEmpty(ofname));

    EXPECT_EQ(Success, importResult);

    EXPECT_EQ(1, roadNetwork->nrOfCars());

    testVehicle = roadNetwork->getCars()[0];
    EXPECT_EQ("MOTORFIETS", testVehicle->getType());
    EXPECT_EQ("1THK180", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(0, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    EXPECT_TRUE(fileIsEmpty(ofname));
}

TEST_F(NetworkImporterTests, Bus) {

    std::string nameTest = "Bus";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_TRUE(fileIsEmpty(ofname));
    EXPECT_EQ(Success, importResult);
    EXPECT_EQ(1, roadNetwork->nrOfCars());

    testVehicle = roadNetwork->getCars()[0];
    EXPECT_EQ("BUS", testVehicle->getType());
    EXPECT_EQ("1THK180", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(0, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    EXPECT_TRUE(fileIsEmpty(ofname));
}

TEST_F(NetworkImporterTests, Vrachtwagen) {

    std::string nameTest = "Vrachtwagen";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_TRUE(fileIsEmpty(ofname));
    EXPECT_EQ(Success, importResult);
    EXPECT_EQ(1, roadNetwork->nrOfCars());

    testVehicle = roadNetwork->getCars()[0];
    EXPECT_EQ("VRACHTWAGEN", testVehicle->getType());
    EXPECT_EQ("1THK180", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(0, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    EXPECT_TRUE(fileIsEmpty(ofname));
}

TEST_F(NetworkImporterTests, UnknownVehicle) {

    std::string nameTest = "UnknownVehicle";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport, importResult);
    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

// Tests the default constructor.
TEST_F(NetworkImporterTests, UnknownRoadSign) {

    std::string nameTest = "UnknownRoadSign";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
//    importResult = NetworkImporter::importRoadNetwork(ifname, std::cout, roadNetwork);
    outputFile.close();


    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(2, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, UnknownType) {

    std::string nameTest = "UnknownType";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
//    importResult = NetworkImporter::importRoadNetwork(ifname, std::cout, roadNetwork);
    outputFile.close();


    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(2, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, EmptyRoadName) {

    std::string nameTest = "EmptyRoadName";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();


    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(2, roadNetwork->nrOfCars());
    EXPECT_EQ(2, roadNetwork->nrOfRoads());

    testRoad = roadNetwork->findRoad("E313");
    EXPECT_EQ(120, testRoad->getSpeedLimit(0));
    EXPECT_EQ(5000, testRoad->getLength());
    EXPECT_TRUE(NULL == testRoad->getIntersection());

    testRoad = roadNetwork->findRoad("E19");
    EXPECT_EQ(100, testRoad->getSpeedLimit(0));
    EXPECT_EQ(2000, testRoad->getLength());
    EXPECT_EQ("E313", testRoad->getIntersection()->getName());

    testVehicle = roadNetwork->getCars()[0];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("1THK180", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(0, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    testVehicle = roadNetwork->getCars()[1];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("651BUF", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(20, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, StringAsSpeedLimit) {

    std::string nameTest = "StringAsSpeedLimit";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();


    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(2, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, DoubleAsSpeedLimit) {

    std::string nameTest = "DoubleAsSpeedLimit";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();


    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(2, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, NegativeSpeedLimit) {

    std::string nameTest = "NegativeSpeedLimit";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();


    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(2, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, AlreadyUsedLicensePlate) {

    std::string nameTest = "AlreadyUsedLicensePlate";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();


    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(1, roadNetwork->nrOfCars());
    EXPECT_EQ(2, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, DrivingOnNonExistentRoad) {

    std::string nameTest = "DrivingOnNonExistentRoad";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();


    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(2, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, BusStop) {

    std::string nameTest = "BusStop";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();


    EXPECT_EQ(Success, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(2, roadNetwork->nrOfRoads());

    testRoad = roadNetwork->findRoad("E19");
    EXPECT_EQ(20, testRoad->getNextBusStop(0));

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
//    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
    EXPECT_TRUE(fileIsEmpty(ofname));
}

TEST_F(NetworkImporterTests, BusStopOnNonExistentRoad) {

    std::string nameTest = "BusStopOnNonExistentRoad";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();


    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(2, roadNetwork->nrOfRoads());

    testRoad = roadNetwork->findRoad("E19");
    EXPECT_EQ(-1, testRoad->getNextBusStop(0));


    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
//    EXPECT_TRUE(fileIsEmpty(ofname));
}

TEST_F(NetworkImporterTests, Zones) {

    std::string nameTest = "Zones";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    testRoad = roadNetwork->findRoad("E19");


    EXPECT_EQ(Success, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(2, roadNetwork->nrOfRoads());

    EXPECT_EQ(100, testRoad->getSpeedLimit(0));
    EXPECT_EQ(50, testRoad->getSpeedLimit(20));
    EXPECT_EQ(30, testRoad->getSpeedLimit(40));

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
//    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
    EXPECT_TRUE(fileIsEmpty(ofname));
}

TEST_F(NetworkImporterTests, TrafficLight) {

    std::string nameTest = "TrafficLight";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    testRoad = roadNetwork->findRoad("E19");


    EXPECT_EQ(Success, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    EXPECT_TRUE(testRoad->getTrafficLight(40) != NULL);

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
//    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
    EXPECT_TRUE(fileIsEmpty(ofname));
}

TEST_F(NetworkImporterTests, NoRoadSpecified) {

    std::string nameTest = "NoRoadSpecified";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
//    importResult = NetworkImporter::importRoadNetwork(ifname, std::cout, roadNetwork);
    outputFile.close();

//    EXPECT_TRUE(fileIsEmpty(ofname));

    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
//    EXPECT_TRUE(fileIsEmpty(ofname));
}

TEST_F(NetworkImporterTests, IncorrectRoadLength) {

    std::string nameTest = "IncorrectRoadLength";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();


    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(0, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, UnknownRoadAttribute) {

    std::string nameTest = "UnknownRoadAttribute";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
//    importResult = NetworkImporter::importRoadNetwork(ifname, std::cout, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(2, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    testRoad = roadNetwork->findRoad("E19");
    EXPECT_EQ(100, testRoad->getSpeedLimit(0));
    EXPECT_EQ(2000, testRoad->getLength());
    EXPECT_TRUE(NULL == testRoad->getIntersection());

    testVehicle = roadNetwork->getCars()[0];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("1THK180", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(0, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    testVehicle = roadNetwork->getCars()[1];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("651BUF", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(20, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, DoubleRoadName) {

    std::string nameTest = "DoubleRoadName";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
//    importResult = NetworkImporter::importRoadNetwork(ifname, std::cout, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(2, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    testRoad = roadNetwork->findRoad("E19");
    EXPECT_EQ(100, testRoad->getSpeedLimit(0));
    EXPECT_EQ(2000, testRoad->getLength());
    EXPECT_TRUE(NULL == testRoad->getIntersection());

    testVehicle = roadNetwork->getCars()[0];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("1THK180", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(0, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    testVehicle = roadNetwork->getCars()[1];
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("651BUF", testVehicle->getLicensePlate());
    EXPECT_EQ("E19", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(20, testVehicle->getCurrentPosition());
    EXPECT_EQ(0, testVehicle->getCurrentSpeed());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, CarOffRoad) {

    std::string nameTest = "CarOffRoad";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    testRoad = roadNetwork->findRoad("E19");
    EXPECT_EQ(100, testRoad->getSpeedLimit(0));
    EXPECT_EQ(2000, testRoad->getLength());
    EXPECT_TRUE(NULL == testRoad->getIntersection());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, IncompleteZone) {

    std::string nameTest = "IncompleteZone";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, TrafficLightOnUnknownRoad) {

    std::string nameTest = "TrafficLightOnUnknownRoad";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, BusStopOffRoad) {

    std::string nameTest = "BusStopOffRoad";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, TrafficLightWithoutPosition) {

    std::string nameTest = "TrafficLightWithoutPosition";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, TrafficLightOffRoad) {

    std::string nameTest = "TrafficLightOffRoad";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, BusStopDoubleAsPosition) {

    std::string nameTest = "BusStopDoubleAsPosition";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(2, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, DoubleLicencePlate) {

    std::string nameTest = "DoubleLicencePlate";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(1, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
}

TEST_F(NetworkImporterTests, BusStopMissingRoad) {

    std::string nameTest = "BusStopMissingRoad";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();


    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(2, roadNetwork->nrOfRoads());

    testRoad = roadNetwork->findRoad("E19");
    EXPECT_EQ(-1, testRoad->getNextBusStop(0));

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
    //EXPECT_TRUE(fileIsEmpty(ofname));
}

TEST_F(NetworkImporterTests, IncompleteRoadInformation) {

    std::string nameTest = "IncompleteRoadInformation";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();


    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(0, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
    //EXPECT_TRUE(fileIsEmpty(ofname));
}

TEST_F(NetworkImporterTests, IncompleteCarInformation) {

    std::string nameTest = "IncompleteCarInformation";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();


    EXPECT_EQ(PartialImport, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
    //EXPECT_TRUE(fileIsEmpty(ofname));
}

TEST_F(NetworkImporterTests, UnknownConnection) {

    std::string nameTest = "UnknownConnection";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";


    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();


    EXPECT_EQ(Success, importResult);

    EXPECT_EQ(0, roadNetwork->nrOfCars());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    std::string expectedOfname = "tests/inputTests/output/expected/" + nameTest + ".txt";
//    EXPECT_TRUE(fileCompare(expectedOfname, ofname));
    EXPECT_TRUE(fileIsEmpty(ofname));
}

//
//int main(int argc, char **argv) {
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}
