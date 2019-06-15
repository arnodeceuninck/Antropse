/**
 * @file Truck.cpp
 * @brief This file will contain the definitions of the functions in Truck.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include "Truck.h"
#include "CONST.h"
#include "DesignByContract.h"
#include "Road.h"

Truck::Truck(const std::string &licensePlate, Road *currentRoad, double currentPosition, double currentSpeed) :

        Vehicle(licensePlate, currentRoad, currentPosition, currentSpeed) {

    REQUIRE(currentPosition > 0, "Huidige positie moet op de zeg liggen");
    REQUIRE(currentRoad != NULL, "De weg moet bestaan en volledig geinitialisseerd zijn");
    REQUIRE(currentPosition <= currentRoad->getLength(), "Huidige positie moet op de weg liggen");
    REQUIRE(currentSpeed <= currentRoad->getSpeedLimit(currentPosition), "De auto mag niet te snel rijden");
    REQUIRE(currentSpeed >= 0, "De auto moet een positieve snelheid hebben");
}

std::string Truck::getType() const {
    return "VRACHTWAGEN";
}

Truck::Truck() : Vehicle() {}

double Truck::getLength() const {
    return CONST::TRUCK_LENGTH;
}

double Truck::getMinSpeed() const {
    return CONST::MIN_TRUCK_SPEED;
}

double Truck::getMaxSpeed() const {
    return CONST::MAX_TRUCK_SPEED;
}

double Truck::getMaxSpeedup() const {
    return CONST::MAX_TRUCK_SPEEDUP;
}

double Truck::getMinSpeedup() const {
    return CONST::MIN_TRUCK_SPEEDUP;
}

