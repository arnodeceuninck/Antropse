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

Car::Car(const std::string &license_plate, Road *current_road, int current_position, double current_speed) :
         Vehicle(license_plate, current_road, current_position, current_speed, CONST::CAR_LENGTH){
}

std::string Car::getType() {
    return "auto";
}
