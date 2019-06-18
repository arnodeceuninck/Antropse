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
#include "Bus.h"
#include "Truck.h"
#include "MotorBike.h"

class NetworkDomainTests : public ::testing::Test {
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
    RoadNetwork *roadNetwork;
    Vehicle *testVehicle;
    Road *testRoad;
};

TEST_F(NetworkDomainTests, GoingForward) {

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL); // Daar onder in die mielies bij die A12
    testVehicle = new Car("ANT-432", testRoad, 20, 0);
    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    // The actual tests
    EXPECT_TRUE(testRoad->properlyInit());
    EXPECT_EQ(testRoad, roadNetwork->findRoad("A12"));
    EXPECT_TRUE(NULL == roadNetwork->retrieveIntersectionRoad("A12"));
    EXPECT_EQ(testVehicle, roadNetwork->findCar("ANT-432"));
    EXPECT_EQ(1, roadNetwork->nrOfCars());
    EXPECT_TRUE(NULL == roadNetwork->findPreviouscar(testVehicle));
    EXPECT_TRUE(roadNetwork->check());
    EXPECT_FALSE(roadNetwork->isEmpty());
    EXPECT_TRUE(roadNetwork->properlyInitialized());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, UnsortedRoads) {

    std::string testName = "UnsortedRoads";

    std::ofstream errStream;
    std::string ofname = "tests/domainTests/output/generated/errorLog" + testName + ".txt";
    errStream.open(ofname.c_str());

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL);

    Road *tempRoad = testRoad;
    roadNetwork->addRoad(testRoad);

    testRoad = new Road("A13", 120, 5000, tempRoad);
    roadNetwork->addRoad(testRoad);

    // The actual tests
    EXPECT_EQ("A13", roadNetwork->getRoads()[0]->getName());
    EXPECT_EQ("A12", roadNetwork->getRoads()[1]->getName());

    // Hierbijj zou het niet mogen crashen
    roadNetwork->automaticSimulation(errStream);

    EXPECT_TRUE(fileIsEmpty(ofname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, SortedRoads) {

    std::string testName = "SortedRoads";

    std::ofstream errStream;
    std::string ofname = "tests/domainTests/output/generated/errorLog" + testName + ".txt";
    errStream.open(ofname.c_str());

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();

    testRoad = new Road("A12", 120, 5000, NULL);
    Road *tempRoad = new Road("A11", 120, 5000, testRoad);

    roadNetwork->addRoad(tempRoad);
    roadNetwork->addRoad(testRoad);

    // The actual tests
    EXPECT_EQ("A11", roadNetwork->getRoads()[0]->getName());
    EXPECT_EQ("A12", roadNetwork->getRoads()[1]->getName());

    // Hierbijj zou het niet mogen crashen
    roadNetwork->automaticSimulation(errStream);

    EXPECT_TRUE(fileIsEmpty(ofname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, SortedCars) {

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();

    testRoad = new Road("A12", 120, 5000, NULL);
    roadNetwork->addRoad(testRoad);

    testVehicle = new Car("ANT-432", testRoad, 20, 0);
    roadNetwork->addCar(testVehicle);

    testVehicle = new Car("ANT-433", testRoad, 40, 0);
    roadNetwork->addCar(testVehicle);

    testVehicle = new Car("ANT-434", testRoad, 60, 0);
    roadNetwork->addCar(testVehicle);

    // The actual tests
    EXPECT_EQ(roadNetwork->getCars()[0]->getLicensePlate(), "ANT-432");
    EXPECT_EQ(roadNetwork->getCars()[1]->getLicensePlate(), "ANT-433");
    EXPECT_EQ(roadNetwork->getCars()[2]->getLicensePlate(), "ANT-434");

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, UnsortedCars) {

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();

    testRoad = new Road("A12", 120, 5000, NULL);
    roadNetwork->addRoad(testRoad);

    testVehicle = new Car("ANT-432", testRoad, 60, 0);
    roadNetwork->addCar(testVehicle);

    testVehicle = new Car("ANT-433", testRoad, 40, 0);
    roadNetwork->addCar(testVehicle);

    testVehicle = new Car("ANT-434", testRoad, 20, 0);
    roadNetwork->addCar(testVehicle);

    // The actual tests
    EXPECT_EQ(roadNetwork->getCars()[2]->getLicensePlate(), "ANT-432");
    EXPECT_EQ(roadNetwork->getCars()[1]->getLicensePlate(), "ANT-433");
    EXPECT_EQ(roadNetwork->getCars()[0]->getLicensePlate(), "ANT-434");

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, SortedCarsOnDifferentRoads) {

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();

    testRoad = new Road("A12", 120, 5000, NULL);
    Road *tempRoad2 = testRoad;
    roadNetwork->addRoad(testRoad);

    testRoad = new Road("A11", 120, 5000, tempRoad2);
    roadNetwork->addRoad(testRoad);
    Road *tempRoad1 = testRoad;

    testRoad = new Road("A10", 120, 5000, tempRoad1);
    roadNetwork->addRoad(testRoad);

    // Eerst komt testRoad, gevolgd door tempRoad1, gevolgd door tempRoad2

    testVehicle = new Car("ANT-432", testRoad, 20, 0);
    roadNetwork->addCar(testVehicle);

    testVehicle = new Car("ANT-433", tempRoad1, 40, 0);
    roadNetwork->addCar(testVehicle);

    testVehicle = new Car("ANT-434", tempRoad2, 60, 0);
    roadNetwork->addCar(testVehicle);

    // The actual tests
    EXPECT_EQ(roadNetwork->getCars()[0]->getLicensePlate(), "ANT-432");
    EXPECT_EQ(roadNetwork->getCars()[1]->getLicensePlate(), "ANT-433");
    EXPECT_EQ(roadNetwork->getCars()[2]->getLicensePlate(), "ANT-434");

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, UnsortedCarsOnDifferentRoads) {

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();

    testRoad = new Road("A12", 120, 5000, NULL);
    Road *tempRoad2 = testRoad;
    roadNetwork->addRoad(testRoad);

    testRoad = new Road("A11", 120, 5000, tempRoad2);
    roadNetwork->addRoad(testRoad);
    Road *tempRoad1 = testRoad;

    testRoad = new Road("A10", 120, 5000, tempRoad1);
    roadNetwork->addRoad(testRoad);

    // Eerst komt testRoad, gevolgd door tempRoad1, gevolgd door tempRoad2

    testVehicle = new Car("ANT-432", tempRoad2, 20, 0);
    roadNetwork->addCar(testVehicle);

    testVehicle = new Car("ANT-433", tempRoad1, 40, 0);
    roadNetwork->addCar(testVehicle);

    testVehicle = new Car("ANT-434", testRoad, 60, 0);
    roadNetwork->addCar(testVehicle);

    // The actual tests
    EXPECT_EQ(roadNetwork->getCars()[2]->getLicensePlate(), "ANT-432");
    EXPECT_EQ(roadNetwork->getCars()[1]->getLicensePlate(), "ANT-433");
    EXPECT_EQ(roadNetwork->getCars()[0]->getLicensePlate(), "ANT-434");

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, BusyDay) {

    std::string nameTest = "BusyDay";

    std::ofstream outputFile;
    std::string ofname = "tests/domainTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";

    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
//    importResult = NetworkImporter::importRoadNetwork(ifname, std::cout, roadNetwork);
    outputFile.close();

    EXPECT_EQ(7, roadNetwork->nrOfCars());
    EXPECT_TRUE(roadNetwork->check());
    EXPECT_FALSE(roadNetwork->isEmpty());
    EXPECT_TRUE(roadNetwork->properlyInitialized());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    while (!roadNetwork->isEmpty()) {
        roadNetwork->moveAllCars(errStream);
        EXPECT_TRUE(roadNetwork->check());
    }

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, DrivingClose) {

    // So baby pull me closer, in the backseat of your rover

    std::string nameTest = "DrivingClose";

    std::ofstream outputFile;
    std::string ofname = "tests/domainTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/domainTests/" + nameTest + ".xml";

    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
//    importResult = NetworkImporter::importRoadNetwork(ifname, std::cout, roadNetwork);
    outputFile.close();

    EXPECT_EQ(7, roadNetwork->nrOfCars());
    EXPECT_TRUE(roadNetwork->check());
    EXPECT_FALSE(roadNetwork->isEmpty());
    EXPECT_TRUE(roadNetwork->properlyInitialized());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    while (!roadNetwork->isEmpty()) {
        roadNetwork->moveAllCars(errStream);
        EXPECT_TRUE(roadNetwork->check());
    }

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, FollowTheLeader) {

    // Follow the leader, leader, leader follow the leader

    std::string nameTest = "FollowTheLeader";

    std::ofstream outputFile;
    std::string ofname = "tests/domainTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/domainTests/" + nameTest + ".xml";

    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
//    importResult = NetworkImporter::importRoadNetwork(ifname, std::cout, roadNetwork);
    outputFile.close();

    EXPECT_EQ(2, roadNetwork->nrOfCars());
    EXPECT_TRUE(roadNetwork->check());
    EXPECT_FALSE(roadNetwork->isEmpty());
    EXPECT_TRUE(roadNetwork->properlyInitialized());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    while (!roadNetwork->isEmpty()) {
        roadNetwork->moveAllCars(errStream);
        EXPECT_TRUE(roadNetwork->check());
    }

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, GoingFastForward) {

    std::string nameTest = "GoingFastForward";

    std::ofstream outputFile;
    std::string ofname = "tests/domainTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/domainTests/" + nameTest + ".xml";

    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
//    importResult = NetworkImporter::importRoadNetwork(ifname, std::cout, roadNetwork);
    outputFile.close();

    EXPECT_EQ(1, roadNetwork->nrOfCars());
    EXPECT_TRUE(roadNetwork->check());
    EXPECT_FALSE(roadNetwork->isEmpty());
    EXPECT_TRUE(roadNetwork->properlyInitialized());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    while (!roadNetwork->isEmpty()) {
        roadNetwork->moveAllCars(errStream);
        EXPECT_TRUE(roadNetwork->check());
    }

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, NeedForSpeed) {

    // It's been a long day, without you my friend

    // 2 auto's, 2 afzonderlijke wegen, 1 race, 2 winnaars

    std::string nameTest = "NeedForSpeed";

    std::ofstream outputFile;
    std::string ofname = "tests/domainTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/domainTests/" + nameTest + ".xml";

    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
//    importResult = NetworkImporter::importRoadNetwork(ifname, std::cout, roadNetwork);
    outputFile.close();

    EXPECT_EQ(2, roadNetwork->nrOfCars());
    EXPECT_TRUE(roadNetwork->check());
    EXPECT_FALSE(roadNetwork->isEmpty());
    EXPECT_TRUE(roadNetwork->properlyInitialized());
    EXPECT_EQ(2, roadNetwork->nrOfRoads());

    while (!roadNetwork->isEmpty()) {
        roadNetwork->moveAllCars(errStream);
        EXPECT_TRUE(roadNetwork->check());
    }

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, SlowDown) {

    // Despacito, quiero desnudarte a besos despacito

    // Een weg waarop je heel snel mag rijden gevolgd door een weg waarop je gaat moeten vertragen

    std::string nameTest = "SlowDown";

    std::ofstream outputFile;
    std::string ofname = "tests/domainTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/domainTests/" + nameTest + ".xml";

    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
//    importResult = NetworkImporter::importRoadNetwork(ifname, std::cout, roadNetwork);
    outputFile.close();

    EXPECT_EQ(1, roadNetwork->nrOfCars());
    EXPECT_TRUE(roadNetwork->check());
    EXPECT_FALSE(roadNetwork->isEmpty());
    EXPECT_TRUE(roadNetwork->properlyInitialized());
    EXPECT_EQ(2, roadNetwork->nrOfRoads());

    while (!roadNetwork->isEmpty()) {
        roadNetwork->moveAllCars(errStream);
        EXPECT_TRUE(roadNetwork->check());
    }

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, SmallStreets) {

    // Een hoop kleine, opeenvolgende straatjes

    std::string nameTest = "SmallStreets";

    std::ofstream outputFile;
    std::string ofname = "tests/domainTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/domainTests/" + nameTest + ".xml";

    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(1, roadNetwork->nrOfCars());
    EXPECT_TRUE(roadNetwork->check());
    EXPECT_FALSE(roadNetwork->isEmpty());
    EXPECT_TRUE(roadNetwork->properlyInitialized());
    EXPECT_EQ(5, roadNetwork->nrOfRoads());

    while (!roadNetwork->isEmpty()) {
        roadNetwork->moveAllCars(errStream);
        EXPECT_TRUE(roadNetwork->check());
    }

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, FloatingPoints) {

    // I believe I can fly

    std::string nameTest = "FloatingPoints";

    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL);
    testVehicle = new Car("ANT-432", testRoad, 20.5, 0.5);
    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    EXPECT_EQ(20.5, testVehicle->getCurrentPosition());

    // The actual tests
    EXPECT_EQ(testRoad, roadNetwork->findRoad("A12"));
    EXPECT_TRUE(NULL == roadNetwork->retrieveIntersectionRoad("A12"));
    EXPECT_EQ(testVehicle, roadNetwork->findCar("ANT-432"));
    EXPECT_EQ(1, roadNetwork->nrOfCars());
    EXPECT_TRUE(NULL == roadNetwork->findPreviouscar(testVehicle));
    EXPECT_TRUE(roadNetwork->check());
    EXPECT_FALSE(roadNetwork->isEmpty());
    EXPECT_TRUE(roadNetwork->properlyInitialized());
    EXPECT_EQ(1, roadNetwork->nrOfRoads());

    testVehicle = roadNetwork->findCar("ANT-432");
    EXPECT_EQ("AUTO", testVehicle->getType());
    EXPECT_EQ("ANT-432", testVehicle->getLicensePlate());
    EXPECT_EQ("A12", testVehicle->getCurrentRoad()->getName());
    EXPECT_EQ(20.5, testVehicle->getCurrentPosition());
    EXPECT_EQ(0.5, testVehicle->getCurrentSpeed());

    // Hierbij zou het niet mogen crashen
    roadNetwork->automaticSimulation(errStream);

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, FastZoneTest) {
    roadNetwork = new RoadNetwork();
    testRoad = new Road("R10", 50, 5000, NULL);
    testRoad->addZone(50, 100);
    testRoad->addZone(100, 150);
    testRoad->addZone(200, 90);
    testRoad->addZone(150, 70);

    EXPECT_EQ(50, testRoad->getSpeedLimit(10));
    EXPECT_EQ(100, testRoad->getSpeedLimit(50));
    EXPECT_EQ(100, testRoad->getSpeedLimit(52));
    EXPECT_EQ(150, testRoad->getSpeedLimit(100));
    EXPECT_EQ(150, testRoad->getSpeedLimit(101));
    EXPECT_EQ(150, testRoad->getSpeedLimit(149));
    EXPECT_EQ(70, testRoad->getSpeedLimit(150));
    EXPECT_EQ(70, testRoad->getSpeedLimit(175));
    EXPECT_EQ(90, testRoad->getSpeedLimit(2663));

    delete roadNetwork;
    delete testRoad;
}

TEST_F(NetworkDomainTests, SlowZoneTest) {
    roadNetwork = new RoadNetwork();
    testRoad = new Road("R10", 120, 5000, NULL);
    testRoad->addZone(50, 30);

    EXPECT_EQ(120, testRoad->getSpeedLimit(0));
    EXPECT_EQ(30, testRoad->getSpeedLimit(55));

    delete roadNetwork;
    delete testRoad;
}

TEST_F(NetworkDomainTests, EasyBusStop) {

    std::string nameTest = "EasyBusStop";
    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 18, 7, NULL);

    testRoad->addBusStop(6);
    testVehicle = new Bus("DL4884", testRoad, 0, 10.8);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    while (!roadNetwork->isEmpty()) {
        roadNetwork->moveAllCars(errStream);
        EXPECT_TRUE(roadNetwork->check());
    }

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, BusStop) {

    std::string nameTest = "BusStop";
    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 120, 500, NULL);

    testRoad->addBusStop(250);
    testVehicle = new Bus("DL4884", testRoad, 0, 0);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    roadNetwork->automaticSimulation(errStream);

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, AllVehicles) {

    std::string nameTest = "AllVehicles";
    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 120, 500, NULL);
    roadNetwork->addRoad(testRoad);

    testVehicle = new Car("ABC123", testRoad, 0, 0);
    roadNetwork->addCar(testVehicle);
    testVehicle = new Bus("DL4884", testRoad, 20, 0);
    roadNetwork->addCar(testVehicle);
    testVehicle = new Truck("YEEETT", testRoad, 40, 0);
    roadNetwork->addCar(testVehicle);
    testVehicle = new MotorBike("LAMBDACALCULUS", testRoad, 60, 0);
    roadNetwork->addCar(testVehicle);

    EXPECT_EQ(1, roadNetwork->nrOfRoads());
    EXPECT_EQ(4, roadNetwork->nrOfCars());

    testVehicle = roadNetwork->getCars()[0];
    EXPECT_EQ("AUTO", testVehicle->getType());
    testVehicle = roadNetwork->getCars()[1];
    EXPECT_EQ("BUS", testVehicle->getType());
    testVehicle = roadNetwork->getCars()[2];
    EXPECT_EQ("VRACHTWAGEN", testVehicle->getType());
    testVehicle = roadNetwork->getCars()[3];
    EXPECT_EQ("MOTORFIETS", testVehicle->getType());

    while (!roadNetwork->isEmpty()) {
        roadNetwork->moveAllCars(errStream);
        EXPECT_TRUE(roadNetwork->check());
    }

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, CopyingVehicle) {

    // Setting up roadnetwork for tests
//    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL);
    testVehicle = new Car("ANT-432", testRoad, 20, 0);
    Car *copiedVehicle = dynamic_cast<Car *>(testVehicle);

    // The actual tests
    EXPECT_EQ(copiedVehicle->getMinSpeed(), testVehicle->getMinSpeed());
    EXPECT_EQ(copiedVehicle->getMaxSpeedup(), testVehicle->getMaxSpeedup());
    EXPECT_EQ(copiedVehicle->getType(), testVehicle->getType());
    EXPECT_EQ(copiedVehicle->getLicensePlate(), testVehicle->getLicensePlate());
    EXPECT_EQ(copiedVehicle->properlyInitialized(), testVehicle->properlyInitialized());
    EXPECT_EQ(copiedVehicle->getCurrentRoad(), testVehicle->getCurrentRoad());
    EXPECT_EQ(copiedVehicle->getCurrentSpeedup(), testVehicle->getCurrentSpeedup());
    EXPECT_EQ(copiedVehicle->getCurrentSpeed(), testVehicle->getCurrentSpeed());
    EXPECT_EQ(copiedVehicle->getCurrentPosition(), testVehicle->getCurrentPosition());
    EXPECT_EQ(copiedVehicle->getMaxSpeed(), testVehicle->getMaxSpeed());
    EXPECT_EQ(copiedVehicle->getMinSpeedup(), testVehicle->getMinSpeedup());

    delete testRoad;
    delete testVehicle;
}

TEST_F(NetworkDomainTests, StoppedShortBeforeBusStop) {

    std::string nameTest = "StoppedShortBeforeBusStop";
    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 18, 7, NULL);

    testRoad->addBusStop(6);
    testVehicle = new Bus("DL4884", testRoad, 0, 0);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    while (!roadNetwork->isEmpty()) {
        roadNetwork->moveAllCars(errStream);
        EXPECT_TRUE(roadNetwork->check());
    }

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, StoppedLongBeforeBusStop) {

    std::string nameTest = "StoppedLongBeforeBusStop";
    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 120, 500, NULL);

    testRoad->addBusStop(250);
    testVehicle = new Bus("DL4884", testRoad, 151, 0);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    roadNetwork->automaticSimulation(errStream);

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, WaitingOnTheBus) {

    std::string nameTest = "WaitingOnTheBus";
    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 60, 500, NULL);

    testRoad->addBusStop(250);
    testVehicle = new Bus("DL4884", testRoad, 40, 0);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    testVehicle = new Car("AAAVALETESTUDIA", testRoad, 0, 0);
    roadNetwork->addCar(testVehicle);

    while (!roadNetwork->isEmpty()) {
        roadNetwork->moveAllCars(errStream);
        EXPECT_TRUE(roadNetwork->check());
    }

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, TwoBusesCloseToEachOther) {

    std::string nameTest = "TwoBusesCloseToEachOther";
    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 60, 500, NULL);

    testRoad->addBusStop(250);
    testVehicle = new Bus("DL4884", testRoad, 40, 0);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    testVehicle = new Bus("AAAVALETESTUDIA", testRoad, 0, 0);
    roadNetwork->addCar(testVehicle);

    while (!roadNetwork->isEmpty()) {
        roadNetwork->moveAllCars(errStream);
        EXPECT_TRUE(roadNetwork->check());
    }

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, TrafficLight) {

    std::string nameTest = "TrafficLight";
    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 50, 500, NULL);

    testRoad->addTrafficLight(250, 0);
    testVehicle = new Car("AL4764", testRoad, 0, 0);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    roadNetwork->automaticSimulation(errStream);

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, TrafficLightWithALotOfCars) {

    std::string nameTest = "TrafficLightWithALotOfCars";
    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 120, 5000, NULL);
    roadNetwork->addRoad(testRoad);

    testRoad->addTrafficLight(2500, 0);
    testVehicle = new Car("IIIIIEP", testRoad, 0, 0);
    roadNetwork->addCar(testVehicle);
    testVehicle = new Car("TOETTOET", testRoad, 50, 0);
    roadNetwork->addCar(testVehicle);
    testVehicle = new Car("VROEMVROEM", testRoad, 100, 0);
    roadNetwork->addCar(testVehicle);

    roadNetwork->automaticSimulation(errStream);

    EXPECT_TRUE(fileIsEmpty(efname));

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, ImpossibleToStopTrafficLight) {

    std::string nameTest = "ImpossibleToStopTrafficLight";
    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 120, 500, NULL);

    testRoad->addTrafficLight(35, 35);
    testVehicle = new Car("AA3264", testRoad, 0, 120);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    roadNetwork->automaticSimulation(errStream);

    std::string expectedOfname = "tests/domainTests/output/expected/errorLog" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, efname));


    delete roadNetwork;
}

TEST_F(NetworkDomainTests, ImpossibleToStopBusStop) {

    std::string nameTest = "ImpossibleToStopBusStop";
    std::ofstream errStream;
    std::string efname = "tests/domainTests/output/generated/errorLog" + nameTest + ".txt";
    errStream.open(efname.c_str());

    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 120, 500, NULL);

    testRoad->addBusStop(35);
    testVehicle = new Bus("HENK", testRoad, 0, 120);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    roadNetwork->automaticSimulation(errStream);

    std::string expectedOfname = "tests/domainTests/output/expected/errorLog" + nameTest + ".txt";
    EXPECT_TRUE(fileCompare(expectedOfname, efname));


    delete roadNetwork;
}

TEST_F(NetworkDomainTests, WhereIsTheTrafficLight) {

    TrafficLight trafficLight(0, 0);
    EXPECT_EQ(0, trafficLight.getPosition());

    trafficLight = TrafficLight(35, 0);
    EXPECT_EQ(35, trafficLight.getPosition());

    trafficLight = TrafficLight(200, 0);
    EXPECT_EQ(200, trafficLight.getPosition());

    trafficLight = TrafficLight(490, 0);
    EXPECT_EQ(490, trafficLight.getPosition());

    trafficLight = TrafficLight(500, 0);
    EXPECT_EQ(500, trafficLight.getPosition());

}

TEST_F(NetworkDomainTests, WhatIsTheColorOfTheTrafficLight) {

    TrafficLight *trafficLight = new TrafficLight(0, 0);

    EXPECT_EQ('|', trafficLight->getColorChar(37));
    EXPECT_EQ(red, trafficLight->getColor(37));

    EXPECT_EQ('-', trafficLight->getColorChar(0));
    EXPECT_EQ(green, trafficLight->getColor(0));

    EXPECT_EQ('o', trafficLight->getColorChar(31));
    EXPECT_EQ(orange, trafficLight->getColor(31));

}

TEST_F(NetworkDomainTests, Bus) {
    testRoad = new Road("Edegemsesteenweg", 100, 500, NULL);
    testVehicle = new Bus("ABC123", testRoad, 0, 0);

    EXPECT_EQ(-7, testVehicle->getMinSpeedup());
    EXPECT_EQ(1, testVehicle->getMaxSpeedup());

    EXPECT_EQ(0, testVehicle->getMinSpeed());
    EXPECT_EQ(70, testVehicle->getMaxSpeed());

    EXPECT_EQ(10, testVehicle->getLength());
}

TEST_F(NetworkDomainTests, Truck) {
    testRoad = new Road("Edegemsesteenweg", 100, 500, NULL);
    testVehicle = new Truck("ABC123", testRoad, 0, 0);

    EXPECT_EQ(-6, testVehicle->getMinSpeedup());
    EXPECT_EQ(1, testVehicle->getMaxSpeedup());

    EXPECT_EQ(0, testVehicle->getMinSpeed());
    EXPECT_EQ(90, testVehicle->getMaxSpeed());

    EXPECT_EQ(15, testVehicle->getLength());
}

TEST_F(NetworkDomainTests, Car) {
    testRoad = new Road("Edegemsesteenweg", 100, 500, NULL);
    testVehicle = new Car("ABC123", testRoad, 0, 0);

    EXPECT_EQ(-8, testVehicle->getMinSpeedup());
    EXPECT_EQ(2, testVehicle->getMaxSpeedup());

    EXPECT_EQ(0, testVehicle->getMinSpeed());
    EXPECT_EQ(150, testVehicle->getMaxSpeed());

    EXPECT_EQ(3, testVehicle->getLength());
}

TEST_F(NetworkDomainTests, MotorBike) {
    testRoad = new Road("Edegemsesteenweg", 100, 500, NULL);
    testVehicle = new MotorBike("ABC123", testRoad, 0, 0);

    EXPECT_EQ(-10, testVehicle->getMinSpeedup());
    EXPECT_EQ(4, testVehicle->getMaxSpeedup());

    EXPECT_EQ(0, testVehicle->getMinSpeed());
    EXPECT_EQ(180, testVehicle->getMaxSpeed());

    EXPECT_EQ(1, testVehicle->getLength());
}


//int main(int argc, char **argv) {
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}
