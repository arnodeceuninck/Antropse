/**
 * @file Road.cpp
 * @brief This file will contain the definitions of the functions in Road.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include "Road.h"

void Road::setName(const std::string &name) {
    Road::name = name;
}

void Road::setSpeed_limit(int speed_limit) {
    Road::speed_limit = speed_limit;
}

void Road::setLength(double length) {
    Road::length = length;
}

void Road::setIntersecions(const Intersections &intersecions) {
    Road::intersecions = intersecions;
}

const std::string &Road::getName() const {
    return name;
}
