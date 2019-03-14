/**
 * @file Road.cpp
 * @brief This file will contain the definitions of the functions in Road.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include "Road.h"

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
    Road::speed_limit = speed_limit;
}

void Road::setLength(double length) {
    Road::length = length;
}


Road::Road() : name(""), speed_limit(0), length(0.0), intersecions(){}

const Intersections &Road::getIntersecions() const {
    return intersecions;
}

