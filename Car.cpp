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

Car::Car(const std::string &licensePlate, Road *currentRoad, double currentPosition, double currentSpeed) :

        Vehicle(licensePlate, currentRoad, currentPosition, currentSpeed) {

    REQUIRE(currentPosition > 0, "Huidige positie moet op de zeg liggen");
    REQUIRE(currentRoad != NULL, "De weg moet bestaan en volledig geinitialisseerd zijn");
    REQUIRE(currentPosition <= currentRoad->getLength(), "Huidige positie moet op de weg liggen");
    REQUIRE(currentSpeed <= currentRoad->getSpeedLimit(currentPosition), "De auto mag niet te snel rijden");
    REQUIRE(currentSpeed >= 0, "De auto moet een positieve snelheid hebben");
}

std::string Car::getType() const {
    return "AUTO";
}

Car::Car() : Vehicle() {}

double Car::getLength() const {
    return CONST::CAR_LENGTH;
}

double Car::getMinSpeed() const {
    return CONST::MIN_CAR_SPEED;
}

double Car::getMaxSpeed() const {
    return CONST::MAX_CAR_SPEED;
}

double Car::getMaxSpeedup() const {
    return CONST::MAX_CAR_SPEEDUP;
}

double Car::getMinSpeedup() const {
    return CONST::MIN_CAR_SPEEDUP;
}

