/**
 * @file Car.cpp
 * @brief This file will contain the definitions of the functions in Car.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include "Car.h"
#include "CONST.h"
#include "DesignByContract.h"

Car::Car(const std::string &license_plate, Road *current_road, int current_position, double current_speed) :

         Vehicle(license_plate, current_road, current_position, current_speed, CONST::CAR_LENGTH){

    REQUIRE(current_position > 0);
    REQUIRE(current_road != NULL);
    REQUIRE(current_position <= current_road->getLength());
    REQUIRE(current_speed < current_road->getSpeed_limit());
    REQUIRE(current_speed >= 0);
}

std::string Car::getType() {
    return "auto";
}

Car::Car() : Vehicle(CONST::CAR_LENGTH) {}

