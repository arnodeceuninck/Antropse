/**
 * @file Vehicle.cpp
 * @brief This file will contain the definitions of the functions in Vehicle.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include "Vehicle.h"

Vehicle::Vehicle(const std::string &license_plate, Road *current_road, int current_position, double current_speed,
                 double length) : license_plate(license_plate), current_road(current_road),
                               current_position(current_position), current_speed(current_speed), length(length) {}
