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
#include "Road.h"

Car::Car(const std::string &license_plate, Road *current_road, int current_position, double current_speed) :

         Vehicle(license_plate, current_road, current_position, current_speed, CONST::CAR_LENGTH){

    REQUIRE(current_position > 0, "Huidige positie moet op de zeg liggen");
    REQUIRE(current_road != NULL, "De weg moet bestaan en volledig geinitialisseerd zijn");
    REQUIRE(current_position <= current_road->getLength(), "Huidige positie moet op de weg liggen");
    REQUIRE(current_speed < current_road->getSpeed_limit(), "De auto mag niet te snel rijden");
    REQUIRE(current_speed >= 0, "De auto moet een positieve snelheid hebben");
}

std::string Car::getType() {
    return "auto";
}

Car::Car() : Vehicle(CONST::CAR_LENGTH) {}

