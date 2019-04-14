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
    REQUIRE(ProperlyInit(), "De weg moet deftig geinitialiseerd zijn");
    return name;
}

int Road::getSpeed_limit() const {
    REQUIRE(ProperlyInit(), "De weg moet deftig geinitialiseerd zijn");
    return speed_limit;
}

double Road::getLength() const {
    REQUIRE(ProperlyInit(), "De weg moet deftig geinitialiseerd zijn");
    return length;
}

void Road::setName(const std::string &newName) {
    REQUIRE(newName.size() > 0, "De naam mag niet leeg zijn");
    Road::name = newName;
}

void Road::setSpeed_limit(int newSpeed_limit) {
    REQUIRE(newSpeed_limit > 0, "De speedlimit moet strikt groter zijn dan 0"); // Strikt groter
    Road::speed_limit = newSpeed_limit;
}

void Road::setLength(double newLength) {
    REQUIRE(newLength > 0, "De lengte moet strikt groter zijn dan 0");
    Road::length = newLength;
}


Road::Road() : name(), speed_limit(), length(), intersection(){ _initCheck = this; }

Road *Road::getIntersection() const {
    return intersection;
}

void Road::setIntersection(Road *newIntersection) {
    Road::intersection = newIntersection;
}

bool Road::ProperlyInit() const {
    return _initCheck == this;
}


