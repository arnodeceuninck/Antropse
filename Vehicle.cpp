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

void Vehicle::setLicense_plate(const std::string &license_plate) {
    Vehicle::license_plate = license_plate;
}

void Vehicle::setCurrent_road(Road *current_road) {
    Vehicle::current_road = current_road;
}

void Vehicle::setCurrent_position(int current_position) {
    Vehicle::current_position = current_position;
}

void Vehicle::setCurrent_speed(double current_speed) {
    Vehicle::current_speed = current_speed;
}

void Vehicle::setCurrent_speedup(double current_speedup) {
    Vehicle::current_speedup = current_speedup;
}

void Vehicle::setLength(double length) {
    Vehicle::length = length;
}

Vehicle::Vehicle() {}
