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
