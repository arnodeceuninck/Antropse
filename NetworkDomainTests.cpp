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
    testRoad = new Road("A12", 120, 5000, NULL);
    testVehicle = new Car("ANT-432", testRoad, 20, 0);
    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

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

    // Hierbijj zou het niet mogen crashen
    roadNetwork->automaticSimulation();

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

//int main(int argc, char **argv) {
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}
