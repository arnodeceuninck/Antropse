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


const std::string &Vehicle::getLicense_plate() const {
    return license_plate;
}

Road *Vehicle::getCurrent_road() const {
    return current_road;
}

int Vehicle::getCurrent_position() const {
    return current_position;
}

double Vehicle::getCurrent_speed() const {
    return current_speed;
}

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


Vehicle::Vehicle(double length): current_speedup(0), length(length) {}
