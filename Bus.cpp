/**
 * @file Bus.cpp
 * @brief This file will contain the definitions of the functions in Bus.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include "Bus.h"
#include "CONST.h"
#include "DesignByContract.h"
#include "Road.h"

Bus::Bus(const std::string &licensePlate, Road *currentRoad, int currentPosition, double currentSpeed) :

        Vehicle(licensePlate, currentRoad, currentPosition, currentSpeed) {

    REQUIRE(currentPosition > 0, "Huidige positie moet op de zeg liggen");
    REQUIRE(currentRoad != NULL, "De weg moet bestaan en volledig geinitialisseerd zijn");
    REQUIRE(currentPosition <= currentRoad->getLength(), "Huidige positie moet op de weg liggen");
    REQUIRE(currentSpeed < currentRoad->getSpeedLimit(), "De auto mag niet te snel rijden");
    REQUIRE(currentSpeed >= 0, "De auto moet een positieve snelheid hebben");
}

std::string Bus::getType() const {
    return "BUS";
}

Bus::Bus() : Vehicle() {}

double Bus::getLength() const {
    return CONST::BUS_LENGTH;
}

double Bus::getMinSpeed() const {
    return CONST::MIN_BUS_SPEED;
}

double Bus::getMaxSpeed() const {
    return CONST::MAX_BUS_SPEED;
}

double Bus::getMaxSpeedup() const {
    return CONST::MAX_BUS_SPEEDUP;
}

double Bus::getMinSpeedup() const {
    return CONST::MIN_BUS_SPEEDUP;
}

