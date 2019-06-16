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

class NetworkDomainTests: public ::testing::Test {
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
    EXPECT_TRUE(NULL == roadNetwork->retrieveRoad("A12"));
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

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL);

    Road* tempRoad = testRoad;
    roadNetwork->addRoad(testRoad);

    testRoad = new Road("A13", 120, 5000, tempRoad);
    roadNetwork->addRoad(testRoad);

    // The actual tests
    EXPECT_EQ("A13", roadNetwork->getRoads()[0]->getName());
    EXPECT_EQ("A12", roadNetwork->getRoads()[1]->getName());

    // Hierbijj zou het niet mogen crashen
    roadNetwork->automaticSimulation();

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, SortedRoads) {

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();

    testRoad = new Road("A12", 120, 5000, NULL);
    Road* tempRoad = new Road("A11", 120, 5000, testRoad);

    roadNetwork->addRoad(tempRoad);
    roadNetwork->addRoad(testRoad);

    // The actual tests
    EXPECT_EQ("A11", roadNetwork->getRoads()[0]->getName());
    EXPECT_EQ("A12", roadNetwork->getRoads()[1]->getName());

    // Hierbijj zou het niet mogen crashen
    roadNetwork->automaticSimulation();

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
    Road* tempRoad2 = testRoad;
    roadNetwork->addRoad(testRoad);

    testRoad = new Road("A11", 120, 5000, tempRoad2);
    roadNetwork->addRoad(testRoad);
    Road* tempRoad1 = testRoad;

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
    Road* tempRoad2 = testRoad;
    roadNetwork->addRoad(testRoad);

    testRoad = new Road("A11", 120, 5000, tempRoad2);
    roadNetwork->addRoad(testRoad);
    Road* tempRoad1 = testRoad;

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

TEST_F(NetworkDomainTests, BusyDay){

    std::string nameTest = "BusyDay";

    std::ofstream outputFile;
    std::string ofname = "tests/inputTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/inputTests/" + nameTest + ".xml";

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

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars();
        EXPECT_TRUE(roadNetwork->check());
    }

    delete roadNetwork;
}

// TODO: fix this failing test
TEST_F(NetworkDomainTests, DrivingClose){

    // So baby pull me closer, in the backseat of your rover

    std::string nameTest = "DrivingClose";

    std::ofstream outputFile;
    std::string ofname = "tests/domainTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/domainTests/" + nameTest + ".xml";

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

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars();
//        std::cout << "Moved" << std::endl;
        EXPECT_TRUE(roadNetwork->checkSpaceBetweenCars());
//        EXPECT_TRUE(roadNetwork->check()); // TODO: uncomment (prevent test spam while not working)
    }

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, FollowTheLeader){

    // Follow the leader, leader, leader follow the leader

    std::string nameTest = "FollowTheLeader";

    std::ofstream outputFile;
    std::string ofname = "tests/domainTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/domainTests/" + nameTest + ".xml";

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

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars();
        EXPECT_TRUE(roadNetwork->check());
    }

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, GoingFastForward){

    std::string nameTest = "GoingFastForward";

    std::ofstream outputFile;
    std::string ofname = "tests/domainTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/domainTests/" + nameTest + ".xml";

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

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars();
        EXPECT_TRUE(roadNetwork->check());
    }

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, NeedForSpeed){

    // It's been a long day, without you my friend

    // 2 auto's, 2 afzonderlijke wegen, 1 race, 2 winnaars

    std::string nameTest = "NeedForSpeed";

    std::ofstream outputFile;
    std::string ofname = "tests/domainTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/domainTests/" + nameTest + ".xml";

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

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars();
        EXPECT_TRUE(roadNetwork->check());
    }

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, SlowDown){

    // Despacito, quiero desnudarte a besos despacito

    // Een weg waarop je heel snel mag rijden gevolgd door een weg waarop je gaat moeten vertragen

    std::string nameTest = "SlowDown";

    std::ofstream outputFile;
    std::string ofname = "tests/domainTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/domainTests/" + nameTest + ".xml";

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

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars();
        EXPECT_TRUE(roadNetwork->check());
    }

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, SmallStreets){

    // Een hoop kleine, opeenvolgende straatjes

    std::string nameTest = "SmallStreets";

    std::ofstream outputFile;
    std::string ofname = "tests/domainTests/output/generated/" + nameTest + ".txt";
    std::string ifname = "tests/domainTests/" + nameTest + ".xml";

    roadNetwork = new RoadNetwork();

    outputFile.open(ofname.c_str());
    importResult = NetworkImporter::importRoadNetwork(ifname, outputFile, roadNetwork);
    outputFile.close();

    EXPECT_EQ(1, roadNetwork->nrOfCars());
    EXPECT_TRUE(roadNetwork->check());
    EXPECT_FALSE(roadNetwork->isEmpty());
    EXPECT_TRUE(roadNetwork->properlyInitialized());
    EXPECT_EQ(5, roadNetwork->nrOfRoads());

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars();
        EXPECT_TRUE(roadNetwork->check());
    }

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, FloatingPoints) {

    // I believe I can fly

    // Setting up roadnetwork for tests
    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL);
    testVehicle = new Car("ANT-432", testRoad, 20.5, 0.5);
    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    EXPECT_EQ(20.5, testVehicle->getCurrentPosition());

    // The actual tests
    EXPECT_EQ(testRoad, roadNetwork->findRoad("A12"));
    EXPECT_TRUE(NULL == roadNetwork->retrieveRoad("A12"));
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
    roadNetwork->automaticSimulation();

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, FastZoneTest){
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

TEST_F(NetworkDomainTests, SlowZoneTest){
    roadNetwork = new RoadNetwork();
    testRoad = new Road("R10", 120, 5000, NULL);
    testRoad->addZone(50, 30);

    EXPECT_EQ(120, testRoad->getSpeedLimit(0));
    EXPECT_EQ(30, testRoad->getSpeedLimit(55));

    delete roadNetwork;
    delete testRoad;
}

TEST_F(NetworkDomainTests, EasyBusStop){
    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 18, 7, NULL);

    testRoad->addBusStop(6);
    testVehicle = new Bus("DL4884", testRoad, 0, 10.8);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars();
        EXPECT_TRUE(roadNetwork->check());
    }

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, BusStop){

    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 120, 500, NULL);

    testRoad->addBusStop(250);
    testVehicle = new Bus("DL4884", testRoad, 0, 0);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    roadNetwork->automaticSimulation();

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, AllVehicles){
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

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars();
        EXPECT_TRUE(roadNetwork->check());
    }

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, CopyingVehicle) {

    // Setting up roadnetwork for tests
//    roadNetwork = new RoadNetwork();
    testRoad = new Road("A12", 120, 5000, NULL);
    testVehicle = new Car("ANT-432", testRoad, 20, 0);
    Car* copiedVehicle = dynamic_cast<Car*>(testVehicle);

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

TEST_F(NetworkDomainTests, StoppedShortBeforeBusStop){
    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 18, 7, NULL);

    testRoad->addBusStop(6);
    testVehicle = new Bus("DL4884", testRoad, 0, 0);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars();
        EXPECT_TRUE(roadNetwork->check());
    }

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, StoppedLongBeforeBusStop){

    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 120, 500, NULL);

    testRoad->addBusStop(250);
    testVehicle = new Bus("DL4884", testRoad, 151, 0);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    roadNetwork->automaticSimulation();

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, WaitingOnTheBus){
    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 60, 500, NULL);

    testRoad->addBusStop(250);
    testVehicle = new Bus("DL4884", testRoad, 40, 0);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    testVehicle = new Car("AAAVALETESTUDIA", testRoad, 0, 0);
    roadNetwork->addCar(testVehicle);

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars();
        EXPECT_TRUE(roadNetwork->check());
    }

    delete roadNetwork;
}

TEST_F(NetworkDomainTests, TwoBusesCloseToEachOther){
    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 60, 500, NULL);

    testRoad->addBusStop(250);
    testVehicle = new Bus("DL4884", testRoad, 40, 0);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    testVehicle = new Bus("AAAVALETESTUDIA", testRoad, 0, 0);
    roadNetwork->addCar(testVehicle);

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars();
        EXPECT_TRUE(roadNetwork->check());
    }

    delete roadNetwork;
}

//int main(int argc, char **argv) {
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}
