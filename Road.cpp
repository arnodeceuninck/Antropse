/**
 * @file Road.cpp
 * @brief This file will contain the definitions of the functions in Road.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include <limits>
#include "Road.h"
#include "DesignByContract.h"

const std::string &Road::getName() const {
    REQUIRE(properlyInit(), "De weg moet deftig geinitialiseerd zijn");
    return name;
}

int Road::getSpeedLimit(double position) const {
    REQUIRE(properlyInit(), "De weg moet deftig geinitialiseerd zijn");
    int speed = speedLimit;
    for(std::map<double, int>::const_iterator zone = zones.begin(); zone != zones.end(); ++zone){
        //TODO: controleer of we de doubles op volgorde overlopen
        if((*zone).first > position){
            return speed;
        }
        speed = (*zone).second;
    }
    std::cerr << "Eine Seefahrt was yeeting speedlimit is 0" << std::endl;
    return 0;
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
    ENSURE(getSpeedLimit(0) == newSpeedLimit, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
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

Road::Road() : name(), speedLimit(), length(), intersection(){
    _initCheck = this;
    length = std::numeric_limits<double>::infinity();
    speedLimit = std::numeric_limits<int>::max();
    intersection = NULL;
    ENSURE(properlyInit(), "The road is properly initialized");}

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
    REQUIRE(properlyInit(), "The road must be correctly initialized");
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

bool Road::addTrafficLight(double position) {
    if(position > 0 &&
       position < getLength() &&
       "Er is nog geen verkeerslicht op de postie"){// TODO check for bus stop at position
        trafficLights[position] = new TrafficLight(position, random());
        // TODO: ensure
        return true;
    }
    return false;
}

double Road::getNextBusStop(double busPosition) {
    double position = -1;
    for (std::set<double>::iterator stop = busStops.begin(); stop != busStops.end(); ++stop){
        if(*stop > busPosition and (position == -1 or position > *stop)){
            position = *stop;
        }
    }
    return position;
}

double Road::getNextTrafficLight(double position) {
    double nextLight = -1;
    for (std::map<double, TrafficLight*>::iterator light = trafficLights.begin(); light != trafficLights.end(); ++light){
        if((*light).first > position and (nextLight == -1 or nextLight > (*light).first)){
            nextLight = (*light).first;
        }
    }
    return nextLight;
}

TrafficLight *Road::getTrafficLight(double position) {
    return trafficLights[position];
}


