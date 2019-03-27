/**
 * @file Road.cpp
 * @brief This file will contain the definitions of the functions in Road.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include "Road.h"
#include "DesignByContract.h"

const std::string &Road::getName() const {
    return name;
}

int Road::getSpeed_limit() const {
    return speed_limit;
}

double Road::getLength() const {
    return length;
}

void Road::setName(const std::string &name) {
    Road::name = name;
}

void Road::setSpeed_limit(int speed_limit) {
    REQUIRE(speed_limit > 0, "De speedlimit moet strikt groter zijn dan 0"); // Strikt groter
    Road::speed_limit = speed_limit;
}

void Road::setLength(double length) {
    REQUIRE(length > 0, "De lengte moet strikt groter zijn dan 0");
    Road::length = length;
}


Road::Road() : name(), speed_limit(), length(), intersection(){}

Road *Road::getIntersection() const {
    return intersection;
}

void Road::setIntersection(Road *intersection) {
    Road::intersection = intersection;
}


