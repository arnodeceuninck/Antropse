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

class AntropseTest: public ::testing::Test {
protected:
    // You should make the members protected s.t. they can be
    // accessed from sub-classes.

    // virtual void SetUp() will be called before each test is run.  You
    // should define it if you need to initialize the variables.
    // Otherwise, this can be skipped.
    virtual void SetUp() {
    }

    // virtual void TearDown() will be called after each test is run.
    // You should define it if there is cleanup work to do.  Otherwise,
    // you don't have to provide it.
    virtual void TearDown() {
    }

    // Declares the variables your tests want to use.
    RoadNetwork* roadNetwork;
    Vehicle* testVehicle;
};

// Tests the default constructor.
TEST_F(AntropseTest, DefaultReadFile) {

    roadNetwork = new RoadNetwork("tests/DefaultReadFile.xml");
    EXPECT_EQ(2, roadNetwork->nrOfCars());

    // TODO: met EQ en NE kan ik niet checken op NULL, wat is de manier om dit alsnog correct te doen (want ik heb een
    //  gevoel dat werken met conditions hiervoor niet echt de bedoeling is)
    EXPECT_TRUE(NULL != roadNetwork->findRoad("E19"));
    EXPECT_TRUE(NULL != roadNetwork->findRoad("E313"));
    EXPECT_TRUE(NULL == roadNetwork->findRoad("E33"));

    EXPECT_TRUE(NULL != roadNetwork->retrieveRoad("E313")); // Er is een weg die E313 als verbinding heeft
    EXPECT_TRUE(NULL == roadNetwork->retrieveRoad("E19"));

    // TODO hetzelfde, maar dan voor auto's
    EXPECT_EQ(0, roadNetwork->getCars()[0]->getCurrent_position());
    EXPECT_EQ(20, roadNetwork->getCars()[1]->getCurrent_position());

    EXPECT_TRUE(roadNetwork->check());

    EXPECT_EQ(20, roadNetwork->findCar("651BUF")->getCurrent_position());

    delete roadNetwork;
}

TEST_F(AntropseTest, GoingForward) {
    roadNetwork = new RoadNetwork("tests/GoingForward.xml");
    EXPECT_EQ(1, roadNetwork->nrOfCars());
    EXPECT_EQ(static_cast<unsigned int>(1), roadNetwork->getRoads().size());
    testVehicle = roadNetwork->getCars()[0];
    EXPECT_EQ(20, testVehicle->getCurrent_position());
    EXPECT_EQ(0, testVehicle->getCurrent_speed());
    testVehicle->move(1, roadNetwork);
    EXPECT_EQ(CONST::MAX_CAR_SPEEDUP, testVehicle->getCurrent_speedup());
    EXPECT_EQ(100, roadNetwork->getRoads()[0]->getSpeed_limit());
    EXPECT_EQ(roadNetwork->getRoads()[0], testVehicle->getCurrent_road());

    for (int i = 0; i < 60; ++i) {
        testVehicle->move(1, roadNetwork);
        EXPECT_TRUE(roadNetwork->check());
    }

//    testVehicle->move(60, roadNetwork); // TODO: Als ik ineens move 60 doe is enkel de snelheid geupdated aan de oude
                                        // versnelling, en dus nog niet de nieuwe snelheid
    EXPECT_NEAR(100, testVehicle->getCurrent_speed(), 1);
}

TEST_F(AntropseTest, FollowTheLeader) {
    roadNetwork = new RoadNetwork("tests/FollowTheLeader.xml");
    EXPECT_EQ(2, roadNetwork->nrOfCars());
    EXPECT_EQ(static_cast<unsigned int>(1), roadNetwork->getRoads().size());

    EXPECT_EQ("651BUF", roadNetwork->findPreviouscar(roadNetwork->findCar("1THK180"))->getLicense_plate());

    roadNetwork->automatic_simulation();

    EXPECT_EQ(0, roadNetwork->nrOfCars());
}

TEST_F(AntropseTest, NeedForSpeed){
    roadNetwork = new RoadNetwork("tests/NeedForSpeed.xml");
    EXPECT_EQ(1, roadNetwork->nrOfCars());
    EXPECT_TRUE(roadNetwork->check());
    EXPECT_EQ(150, roadNetwork->getRoads()[0]->getSpeed_limit());

//    testVehicle = roadNetwork->getCars()[0];
//    testVehicle->move(1, roadNetwork);
    roadNetwork->automatic_simulation();
    EXPECT_EQ(0, roadNetwork->nrOfCars());
}

TEST_F(AntropseTest, BusyDay){
    roadNetwork = new RoadNetwork("tests/BusyDay.xml");
    EXPECT_EQ(7, roadNetwork->nrOfCars());
    EXPECT_TRUE(roadNetwork->check());
    roadNetwork->automatic_simulation();
    EXPECT_TRUE(roadNetwork->check());
}

TEST_F(AntropseTest, DrivingClose){
    roadNetwork = new RoadNetwork("tests/DrivingClose.xml");
    EXPECT_EQ(7, roadNetwork->nrOfCars());
    EXPECT_TRUE(roadNetwork->check());
    roadNetwork->automatic_simulation();
    EXPECT_TRUE(roadNetwork->check());
}

TEST_F(AntropseTest, ContractViolationsVehicle){
    EXPECT_DEATH(testVehicle->setCurrent_position(-1), "Assertion.*failed");
    EXPECT_DEATH(testVehicle->setCurrent_speed(932), "Assertion.*failed");
    EXPECT_DEATH(testVehicle->setLicense_plate(""), "Assertion.*failed");
    EXPECT_NO_THROW(testVehicle->setCurrent_position(0));
}

// TODO's: Foute input herkennen
//TEST_F(AntropseTest, SomeoneFloating){
//    roadNetwork = new RoadNetwork("tests/SomeoneFloating.xml");
//    EXPECT_EQ(90.5, roadNetwork->getRoads()[0]->getSpeed_limit());
//    EXPECT_EQ(30.3, roadNetwork->getRoads()[0]->getLength());
//
//    testVehicle = roadNetwork->getCars()[0];
//    EXPECT_EQ(20.2, testVehicle->getCurrent_position());
//    EXPECT_EQ(0.4, testVehicle->getCurrent_speed());
//
//    EXPECT_TRUE(roadNetwork->check());
//    roadNetwork->automatic_simulation();
//    EXPECT_TRUE(roadNetwork->check());
//}

//TEST_F(AntropseTest, NoPersonalSpace){
//    roadNetwork = new RoadNetwork("tests/SomeoneFloating.xml");
//
//    // TODO: Error bij input verwachten
//    EXPECT_FALSE(roadNetwork->check_space_between_cars());
//    EXPECT_TRUE(roadNetwork->check_if_cars_on_existing_road());
//    EXPECT_TRUE(roadNetwork->check_intersections());
//    EXPECT_TRUE(roadNetwork->check_position_cars());
//}
//
//TEST_F(AntropseTest, RocketHigh){
//    roadNetwork = new RoadNetwork("tests/RocketHigh.xml");
//
//    // TODO: Error bij input verwachten (snelheid > CONST::MAX_CAR_SPEED)
//    EXPECT_FALSE(roadNetwork->check_position_cars());
//}
//
//TEST_F(AntropseTest, WayTooLow){
//    roadNetwork = new RoadNetwork("tests/WayToLow.xml");
//
//    // TODO: Error bij input verwachten (snelheid > CONST::MAX_CAR_SPEED)
//    EXPECT_FALSE(roadNetwork->check_position_cars());
//}
//
//TEST_F(AntropseTest, SmallStreets){
//    roadNetwork = new RoadNetwork("tests/SmallStreets.xml");
//    EXPECT_EQ(static_cast<unsigned int>(5), roadNetwork->getRoads().size());
//    EXPECT_TRUE(roadNetwork->check());
//
//    EXPECT_EQ(static_cast<unsigned int>(1), roadNetwork->getCars().size());
//    roadNetwork->automatic_simulation();
//    EXPECT_EQ((unsigned) 0, roadNetwork->getCars().size());
//}
//
//TEST_F(AntropseTest, SlowDown){
//    roadNetwork = new RoadNetwork("tests/SlowDown.xml");
//    EXPECT_EQ(10, roadNetwork->findRoad("E11")->getIntersection()->getSpeed_limit());
//
//    EXPECT_TRUE(roadNetwork->check());
//    roadNetwork->automatic_simulation();
//    EXPECT_TRUE(roadNetwork->check());
//
//}


//-- Tests the "happy day" scenario
//TEST_F(TicTactToeTest, HappyDay) {
//    EXPECT_EQ(0, ttt_.nrOfMoves());
//    EXPECT_TRUE(ttt_.notDone());
//    while (ttt_.notDone()) {
//        ttt_.doMove();
//    };
//    EXPECT_FALSE(ttt_.notDone());
//    EXPECT_EQ(9, ttt_.nrOfMoves());
//}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
