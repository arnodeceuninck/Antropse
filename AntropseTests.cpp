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

#include "RoadNetwork.h"

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
    RoadNetwork roadNetwork;
};

// Tests the default constructor.
TEST_F(AntropseTest, DefaultReadFile) {
    roadNetwork = RoadNetwork("test.xml");
    EXPECT_EQ(2, roadNetwork.nrOfCars());

    // TODO: met EQ en NE kan ik niet checken op NULL, wat is de manier om dit alsnog correct te doen (want ik heb een
    //  gevoel dat werken met conditions hiervoor niet echt de bedoeling is)
    EXPECT_TRUE(NULL != roadNetwork.findRoad("E19"));
    EXPECT_TRUE(NULL != roadNetwork.findRoad("E313"));
    EXPECT_TRUE(NULL == roadNetwork.findRoad("E33"));

    EXPECT_TRUE(NULL != roadNetwork.retrieveRoad("E313")); // Er is een weg die E313 als verbinding heeft
    EXPECT_TRUE(NULL == roadNetwork.retrieveRoad("E19"));

    // TODO hetzelfde, maar dan voor auto's
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
