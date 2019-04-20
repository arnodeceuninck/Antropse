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

Car::Car(const std::string &licensePlate, Road *currentRoad, int currentPosition, double currentSpeed) :

        Vehicle(licensePlate, currentRoad, currentPosition, currentSpeed) {

    REQUIRE(currentPosition > 0, "Huidige positie moet op de zeg liggen");
    REQUIRE(currentRoad != NULL, "De weg moet bestaan en volledig geinitialisseerd zijn");
    REQUIRE(currentPosition <= currentRoad->getLength(), "Huidige positie moet op de weg liggen");
    REQUIRE(currentSpeed < currentRoad->getSpeedLimit(), "De auto mag niet te snel rijden");
    REQUIRE(currentSpeed >= 0, "De auto moet een positieve snelheid hebben");
}

std::string Car::getType() const {
    return "AUTO";
}

Car::Car() : Vehicle() {}

double Car::getLength() const {
    return CONST::CAR_LENGTH;
}

