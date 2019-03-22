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
    roadNetwork = new RoadNetwork("test.xml");
    EXPECT_EQ(2, roadNetwork->nrOfActiveCars());

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
    roadNetwork = new RoadNetwork("test2.xml");
    EXPECT_EQ(1, roadNetwork->nrOfActiveCars());
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
    EXPECT_EQ(100, testVehicle->getCurrent_speed());
}

TEST_F(AntropseTest, FollowTheLeader) {
    roadNetwork = new RoadNetwork("test3.xml");
    EXPECT_EQ(2, roadNetwork->nrOfActiveCars());
    EXPECT_EQ(static_cast<unsigned int>(1), roadNetwork->getRoads().size());

    EXPECT_EQ("651BUF", roadNetwork->findPreviouscar(roadNetwork->findCar("1THK180"))->getLicense_plate());

    roadNetwork->automatic_simulation();

    EXPECT_EQ(0, roadNetwork->nrOfActiveCars());


}

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
