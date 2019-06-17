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
#include "Car.h"
#include "NetworkExporter.h"
#include "Bus.h"
#include "MotorBike.h"
#include "Truck.h"
#include "GraphicImpressionExporter.h"

class NetworkExporterTests: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    // virtual void SetUp() will be called before each test is run.  You
    // should define it if you need to initialize the variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {
        importResult = SuccessEnum();
    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    virtual void TearDown() {
    }

    // Declares the variables your tests want to use.
    SuccessEnum importResult;
    RoadNetwork* roadNetwork;
    Vehicle* testVehicle;
    Road* testRoad;
};

// Tests the default constructor.
TEST_F(NetworkExporterTests, fileCompare) {

    // Source: Test van Serge Demeyer (TicTacToe21) (letterlijk overgenomen)

    ASSERT_TRUE(directoryExists("tests/outputTests"));

    std::ofstream myfile;
    myfile.open("tests/outputTests/file1.txt");
    myfile.close();
    myfile.open("tests/outputTests/file2.txt");
    myfile.close();

    EXPECT_TRUE(fileExists("tests/outputTests/file1.txt"));
    EXPECT_TRUE(fileExists("tests/outputTests/file2.txt"));
    EXPECT_TRUE(fileIsEmpty("tests/outputTests/file1.txt"));
    EXPECT_TRUE(fileIsEmpty("tests/outputTests/file2.txt"));


    //compare 2 empty files
// *** Beware: the following does not work with older versions of libstdc++
// *** It doesn't work with gcc version 4.0.1 (Apple Inc. build 5465)
// *** It does work with gcc version 4.2.1
    EXPECT_TRUE(fileCompare("tests/outputTests/file1.txt", "tests/outputTests/file2.txt"));
    EXPECT_TRUE(fileCompare("tests/outputTests/file2.txt", "tests/outputTests/file1.txt"));

    //compare an empty and a non-empty files
    myfile.open("tests/outputTests/file1.txt");
    myfile << "xxx" << std::endl << "yyy";
    myfile.close();
    EXPECT_TRUE(fileExists("tests/outputTests/file1.txt"));
    EXPECT_FALSE(fileIsEmpty("tests/outputTests/file1.txt"));
    EXPECT_FALSE(fileCompare("tests/outputTests/file1.txt", "tests/outputTests/file2.txt"));
    EXPECT_FALSE(fileCompare("tests/outputTests/file2.txt", "tests/outputTests/file1.txt"));

    //compare two equal files
    myfile.open("tests/outputTests/file2.txt");
    myfile << "xxx" << std::endl << "yyy";
    myfile.close();
    EXPECT_TRUE(fileExists("tests/outputTests/file2.txt"));
    EXPECT_FALSE(fileIsEmpty("tests/outputTests/file2.txt"));
    EXPECT_TRUE(fileCompare("tests/outputTests/file1.txt", "tests/outputTests/file2.txt"));
    EXPECT_TRUE(fileCompare("tests/outputTests/file2.txt", "tests/outputTests/file1.txt"));

    //compare 2 non-empty files which are off by a character in the middle
    myfile.open("tests/outputTests/file2.txt");
    myfile << "xxx" << std::endl << "xyy";
    myfile.close();
    EXPECT_FALSE(fileCompare("tests/outputTests/file1.txt", "tests/outputTests/file2.txt"));
    EXPECT_FALSE(fileCompare("tests/outputTests/file2.txt", "tests/outputTests/file1.txt"));

    //compare 2 non-empty files where one is one character shorter than the other
    myfile.open("tests/outputTests/file2.txt");
    myfile << "xxx" << std::endl << "yy";
    myfile.close();
    EXPECT_FALSE(fileCompare("tests/outputTests/file1.txt", "tests/outputTests/file2.txt"));
    EXPECT_FALSE(fileCompare("tests/outputTests/file2.txt", "tests/outputTests/file1.txt"));

    //compare existing against non existing file
    EXPECT_FALSE(
            fileCompare("tests/outputTests/file1.txt", "tests/outputTests/nonexisting.txt"));
    EXPECT_FALSE(
            fileCompare("tests/outputTests/nonexisting.txt", "tests/outputTests/file1.txt"));
}

TEST_F(NetworkExporterTests, OutputGoingForward){

    std::string testName = "OutputGoingForward";

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL);
    testVehicle = new Car("ANT-432", testRoad, 20, 0);
    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    // Exporter code
    std::ofstream output;
    std::string ifname = "tests/outputTests/generated/" + testName + ".txt";
    output.open(ifname.c_str());

    std::ofstream errStream;
    std::string ofname = "tests/outputTests/generated/errorLog" + testName + ".txt";
    errStream.open(ofname.c_str());

    NetworkExporter exporter;
    EXPECT_TRUE(exporter.properlyInitialized());
    exporter.documentStart(std::cout);
    EXPECT_TRUE(exporter.documentStarted());

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars(errStream);
        exporter.exportOn(output, *roadNetwork);
    }

    exporter.documentEnd(std::cout);
    EXPECT_FALSE(exporter.documentStarted());

    std::string expectedFileName = "tests/outputTests/expected/" + testName + ".txt";
    EXPECT_TRUE(fileCompare(ifname, expectedFileName));

    EXPECT_TRUE(fileIsEmpty(ofname));

    delete roadNetwork;
}

TEST_F(NetworkExporterTests, EmptyRoadNetwork){

    std::string testName = "EmptyRoadNetwork";

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();

    // Exporter code
    std::ofstream output;
    std::string ifname = "tests/outputTests/generated/" + testName + ".txt";
    output.open(ifname.c_str());

    NetworkExporter exporter;
    EXPECT_TRUE(exporter.properlyInitialized());
    exporter.documentStart(std::cout);
    EXPECT_TRUE(exporter.documentStarted());

    exporter.exportOn(output, *roadNetwork);

    exporter.documentEnd(std::cout);
    EXPECT_FALSE(exporter.documentStarted());

    std::string expectedFileName = "tests/outputTests/expected/" + testName + ".txt";
    EXPECT_TRUE(fileCompare(ifname, expectedFileName));

    delete roadNetwork;
}

TEST_F(NetworkExporterTests, FloatingPoints){

    std::string testName = "FloatingPoints";

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL);
    testVehicle = new Car("ANT-432", testRoad, 19.5, 0.3);
    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    // Exporter code
    std::ofstream output;
    std::string ifname = "tests/outputTests/generated/" + testName + ".txt";
    output.open(ifname.c_str());

    NetworkExporter exporter;
    EXPECT_TRUE(exporter.properlyInitialized());
    exporter.documentStart(std::cout);
    EXPECT_TRUE(exporter.documentStarted());

    exporter.exportOn(output, *roadNetwork);

    exporter.documentEnd(std::cout);
    EXPECT_FALSE(exporter.documentStarted());

    std::string expectedFileName = "tests/outputTests/expected/" + testName + ".txt";
    EXPECT_TRUE(fileCompare(ifname, expectedFileName));

    delete roadNetwork;
}

TEST_F(NetworkExporterTests, BusjeKomtZo){

    std::string testName = "BusjeKomtZo";

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL);
    testVehicle = new Bus("ANT-432", testRoad, 20, 0);
    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    // Exporter code
    std::ofstream output;
    std::string ifname = "tests/outputTests/generated/" + testName + ".txt";
    output.open(ifname.c_str());

    NetworkExporter exporter;
    EXPECT_TRUE(exporter.properlyInitialized());
    exporter.documentStart(std::cout);
    EXPECT_TRUE(exporter.documentStarted());

    exporter.exportOn(output, *roadNetwork);

    exporter.documentEnd(std::cout);
    EXPECT_FALSE(exporter.documentStarted());

    std::string expectedFileName = "tests/outputTests/expected/" + testName + ".txt";
    EXPECT_TRUE(fileCompare(ifname, expectedFileName));

    delete roadNetwork;
}

TEST_F(NetworkExporterTests, MotorBike){

    // Bicycle, Bicyle, I want to ride my bicycle, I want to ride my bike

    std::string testName = "MotorBike";

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL);
    testVehicle = new MotorBike("ANT-432", testRoad, 20, 0);
    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    // Exporter code
    std::ofstream output;
    std::string ifname = "tests/outputTests/generated/" + testName + ".txt";
    output.open(ifname.c_str());

    NetworkExporter exporter;
    EXPECT_TRUE(exporter.properlyInitialized());
    exporter.documentStart(std::cout);
    EXPECT_TRUE(exporter.documentStarted());

    exporter.exportOn(output, *roadNetwork);

    exporter.documentEnd(std::cout);
    EXPECT_FALSE(exporter.documentStarted());

    std::string expectedFileName = "tests/outputTests/expected/" + testName + ".txt";
    EXPECT_TRUE(fileCompare(ifname, expectedFileName));

    delete roadNetwork;
}

TEST_F(NetworkExporterTests, Truck){

    std::string testName = "Truck";

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL);
    testVehicle = new Truck("ANT-432", testRoad, 20, 0);
    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    // Exporter code
    std::ofstream output;
    std::string ifname = "tests/outputTests/generated/" + testName + ".txt";
    output.open(ifname.c_str());

    NetworkExporter exporter;
    EXPECT_TRUE(exporter.properlyInitialized());
    exporter.documentStart(std::cout);
    EXPECT_TRUE(exporter.documentStarted());

    exporter.exportOn(output, *roadNetwork);

    exporter.documentEnd(std::cout);
    EXPECT_FALSE(exporter.documentStarted());

    std::string expectedFileName = "tests/outputTests/expected/" + testName + ".txt";
    EXPECT_TRUE(fileCompare(ifname, expectedFileName));

    delete roadNetwork;
}

TEST_F(NetworkExporterTests, EmptyRoadNetworkGIE){

    std::string testName = "EmptyRoadNetworkGIE";

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();

    // Exporter code
    std::ofstream output;
    std::string ifname = "tests/outputTests/generated/" + testName + ".txt";
    output.open(ifname.c_str());

    GraphicImpressionExporter exporter;
    EXPECT_TRUE(exporter.properlyInitialized());
    exporter.documentStart(std::cout);
    EXPECT_TRUE(exporter.documentStarted());

    exporter.exportOn(output, *roadNetwork);

    exporter.documentEnd(std::cout);
    EXPECT_FALSE(exporter.documentStarted());

    std::string expectedFileName = "tests/outputTests/expected/" + testName + ".txt";
    EXPECT_TRUE(fileCompare(ifname, expectedFileName));

    delete roadNetwork;
}

TEST_F(NetworkExporterTests, BusjeKomtZoGIE){

    std::string testName = "BusjeKomtZoGIE";

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL);
    testVehicle = new Bus("ANT-432", testRoad, 20, 0);
    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    // Exporter code
    std::ofstream output;
    std::string ifname = "tests/outputTests/generated/" + testName + ".txt";
    output.open(ifname.c_str());

    GraphicImpressionExporter exporter;
    EXPECT_TRUE(exporter.properlyInitialized());
    exporter.documentStart(std::cout);
    EXPECT_TRUE(exporter.documentStarted());

    exporter.exportOn(output, *roadNetwork);

    exporter.documentEnd(std::cout);
    EXPECT_FALSE(exporter.documentStarted());

    std::string expectedFileName = "tests/outputTests/expected/" + testName + ".txt";
    EXPECT_TRUE(fileCompare(ifname, expectedFileName));

    delete roadNetwork;
}

TEST_F(NetworkExporterTests, MotorBikeGIE){

    // Bicycle, Bicyle, I want to ride my bicycle, I want to ride my bike

    std::string testName = "MotorBikeGIE";

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL);
    testVehicle = new MotorBike("ANT-432", testRoad, 20, 0);
    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    // Exporter code
    std::ofstream output;
    std::string ifname = "tests/outputTests/generated/" + testName + ".txt";
    output.open(ifname.c_str());

    GraphicImpressionExporter exporter;
    EXPECT_TRUE(exporter.properlyInitialized());
    exporter.documentStart(std::cout);
    EXPECT_TRUE(exporter.documentStarted());

    exporter.exportOn(output, *roadNetwork);

    exporter.documentEnd(std::cout);
    EXPECT_FALSE(exporter.documentStarted());

    std::string expectedFileName = "tests/outputTests/expected/" + testName + ".txt";
    EXPECT_TRUE(fileCompare(ifname, expectedFileName));

    delete roadNetwork;
}

TEST_F(NetworkExporterTests, TruckGIE){

    std::string testName = "TruckGIE";

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL);
    testVehicle = new Truck("ANT-432", testRoad, 20, 0);
    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    // Exporter code
    std::ofstream output;
    std::string ifname = "tests/outputTests/generated/" + testName + ".txt";
    output.open(ifname.c_str());

    GraphicImpressionExporter exporter;
    EXPECT_TRUE(exporter.properlyInitialized());
    exporter.documentStart(std::cout);
    EXPECT_TRUE(exporter.documentStarted());

    exporter.exportOn(output, *roadNetwork);

    exporter.documentEnd(std::cout);
    EXPECT_FALSE(exporter.documentStarted());

    std::string expectedFileName = "tests/outputTests/expected/" + testName + ".txt";
    EXPECT_TRUE(fileCompare(ifname, expectedFileName));

    delete roadNetwork;
}


//int main(int argc, char **argv) {
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}
