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

bool Road::setName(const std::string &newName) {
    if(!(newName.size() > 0)){
        return false;
    }
    REQUIRE(newName.size() > 0, "De naam mag niet leeg zijn");
    Road::name = newName;
    ENSURE(getName() == newName, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
    return true;
}

bool Road::setSpeedLimit(int newSpeedLimit) {
    if(!(newSpeedLimit > 0)){
        return false;
    }
    REQUIRE(newSpeedLimit > 0, "De speedlimit moet strikt groter zijn dan 0"); // Strikt groter
    Road::speedLimit = newSpeedLimit;
    ENSURE(getSpeedLimit() == newSpeedLimit, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
    return true;
}

bool Road::setLength(double newLength) {
    if(!(newLength > 0)){
        return false;
    }
    REQUIRE(newLength > 0, "De lengte moet strikt groter zijn dan 0");
    Road::length = newLength;
    ENSURE(getLength() == newLength, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
    return true;
}

Road::Road() : name(), speedLimit(), length(), intersection(){ _initCheck = this; }

Road *Road::getIntersection() const {
    return intersection;
}

bool Road::setIntersection(Road *newIntersection) {
    if(!(newIntersection->properlyInit())){
        return false;
    }
    REQUIRE(newIntersection->properlyInit(), "De intersectie moet deftig geinitialiseerd zijn");
    Road::intersection = newIntersection;
    ENSURE(getIntersection() == newIntersection, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
    return true;
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

bool Road::addBusStop(int position) {
    if(position > 0 &&
        position < getLength() &&
        "Er is nog geen bushalte op de postie"){// TODO check for bus stop at position
        busStops.insert(position);
        // TODO: ensure
        return true;
    }
    return false;
}

bool Road::addZone(double position, int zoneSpeedLimit) {
    if(position > 0 &&
       position < getLength() &&
       "Er is nog geen zonebord op de postie"){// TODO check for bus stop at position
        zones[position] = zoneSpeedLimit;
        // TODO: ensure
        return true;
    }
    return false;
}

bool Road::addTrafficLight(int position) {
    if(position > 0 &&
       position < getLength() &&
       "Er is nog geen verkeerslicht op de postie"){// TODO check for bus stop at position
        trafficLights.insert(position);
        // TODO: ensure
        return true;
    }
    return false;
}

// TODO: continue
double Road::getNextBusStop(double busPosition) {
    double position = -1;
    for (std::set<double>::iterator stop = busStops.begin(); stop != busStops.end(); ++stop){
        if(*stop > busPosition and (position == -1 or position > *stop)){
            position = *stop;
        }
    }
    return position;
}


