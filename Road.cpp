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
    REQUIRE(properlyInit(), "De weg moet deftig geinitialiseerd zijn");
    return name;
}

int Road::getSpeedLimit() const {
    REQUIRE(properlyInit(), "De weg moet deftig geinitialiseerd zijn");
    return speedLimit;
}

double Road::getLength() const {
    REQUIRE(properlyInit(), "De weg moet deftig geinitialiseerd zijn");
    return length;
}

void Road::setName(const std::string &newName) {
    REQUIRE(newName.size() > 0, "De naam mag niet leeg zijn");
    Road::name = newName;
}

void Road::setSpeedLimit(int newSpeedLimit) {
    REQUIRE(newSpeedLimit > 0, "De speedlimit moet strikt groter zijn dan 0"); // Strikt groter
    Road::speedLimit = newSpeedLimit;
}

void Road::setLength(double newLength) {
    REQUIRE(newLength > 0, "De lengte moet strikt groter zijn dan 0");
    Road::length = newLength;
}

Road::Road() : name(), speedLimit(), length(), intersection(){ _initCheck = this; }

Road *Road::getIntersection() const {
    return intersection;
}

void Road::setIntersection(Road *newIntersection) {
    Road::intersection = newIntersection;
}

bool Road::properlyInit() const {
    return _initCheck == this;
}

Road::Road(const std::string &name, int speedLimit, double length, Road *intersection) : name(name),
                                                                                         speedLimit(speedLimit),
                                                                                         length(length),
                                                                                         intersection(intersection) {
    _initCheck = this;
}


