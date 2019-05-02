/**
 * @file MotorBike.cpp
 * @brief This file will contain the definitions of the functions in MotorBike.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include "MotorBike.h"
#include "CONST.h"
#include "DesignByContract.h"
#include "Road.h"

MotorBike::MotorBike(const std::string &licensePlate, Road *currentRoad, double currentPosition, double currentSpeed) :

        Vehicle(licensePlate, currentRoad, currentPosition, currentSpeed) {

    REQUIRE(currentPosition > 0, "Huidige positie moet op de zeg liggen");
    REQUIRE(currentRoad != NULL, "De weg moet bestaan en volledig geinitialisseerd zijn");
    REQUIRE(currentPosition <= currentRoad->getLength(), "Huidige positie moet op de weg liggen");
    REQUIRE(currentSpeed < currentRoad->getSpeedLimit(), "De auto mag niet te snel rijden");
    REQUIRE(currentSpeed >= 0, "De auto moet een positieve snelheid hebben");
}

std::string MotorBike::getType() const {
    return "MOTORFIETS";
}

MotorBike::MotorBike() : Vehicle() {}

double MotorBike::getLength() const {
    return CONST::MOTOR_LENGTH;
}

double MotorBike::getMinSpeed() const {
    return CONST::MIN_MOTOR_SPEED;
}

double MotorBike::getMaxSpeed() const {
    return CONST::MAX_MOTOR_SPEED;
}

double MotorBike::getMaxSpeedup() const {
    return CONST::MAX_MOTOR_SPEEDUP;
}

double MotorBike::getMinSpeedup() const {
    return CONST::MIN_MOTOR_SPEEDUP;
}

