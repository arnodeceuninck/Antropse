/**
 * @file Vehicle.cpp
 * @brief This file will contain the definitions of the functions in Vehicle.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include <iostream>
#include "Vehicle.h"
#include "Convert.h"
#include "CONST.h"
#include "Road.h"
#include "RoadNetwork.h"
#include "DesignByContract.h"

Vehicle::Vehicle(const std::string &license_plate, Road *current_road, int current_position, double current_speed,
                 double length) : license_plate(license_plate), current_road(current_road),
                               current_position(current_position), current_speed(current_speed), current_speedup(0), length(length){}


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


Vehicle::Vehicle(double length): license_plate(""), current_road(NULL),
                                 current_position(0), current_speed(0),
                                 current_speedup(0), length(length) {}

bool Vehicle::move(const double &time, RoadNetwork *roadNetwork) {
    REQUIRE(roadNetwork->check(), "Roadnetwork moet correct geinitialiseerd zijn");
    REQUIRE(time >= 0, "Tijd moet positief zijn");

    // Bereken nieuwe positie van voertuig
    current_position = Convert::kmh_to_ms(current_speed)*time + current_position;

    // Bereken nieuwe snelheid van voertuig
    current_speed = current_speedup * time + current_speed;
    if(current_speed > CONST::MAX_CAR_SPEED){
        current_speed = CONST::MAX_CAR_SPEED;
    }
    if(current_speed > current_road->getSpeed_limit()){
        current_speed = current_road->getSpeed_limit();
    }

    // Bereken nieuwe versnelling van voertuig;
    Vehicle* previouscar = roadNetwork->findPreviouscar(this);
    if (previouscar != NULL) {

        double ideal_following_distance = (3 * current_speed) / 4 + previouscar->getLength() + 2;
        double actual_following_distance =
                previouscar->getCurrent_position() - previouscar->getLength() - current_position;


        current_speedup = (actual_following_distance-ideal_following_distance)/2;

        if (current_speedup > CONST::MAX_CAR_SPEEDUP){
            current_speedup = CONST::MAX_CAR_SPEEDUP;
        }

    } else {
        if(current_speed < current_road->getSpeed_limit()){
            current_speedup = CONST::MAX_CAR_SPEEDUP;
        } else {
            if(current_speedup >= CONST::MIN_CAR_SPEEDUP-1)
            current_speedup -= 1;
        }
    }

    // Controle of je aan deze snelheid niet over de max snelheid gaat
    if(current_speedup * time + current_speed > current_road->getSpeed_limit()){
        current_speedup = (current_road->getSpeed_limit()-current_speed)/time; // Bereken de versnelling die nodig is om net de maximaal toegelaten snelheid te bereiken
    }

    // IF nieuwe positie valt buiten huidige baan
    while (current_road != NULL && current_position > current_road->getLength()){
        current_position = current_position - current_road->getLength();
        if(current_road->getIntersection() != NULL) {
            // IF huidige baan heeft verbinding
            // Zet voertuig op verbindingsbaan
            current_road = current_road->getIntersection();
        } else {
            // ELSE
            // Verwijder voertuig uit simulatie
            roadNetwork->removeVehicle(license_plate);
        }
    }

    ENSURE(roadNetwork->check(), "Roadnetwork blijft geldig na de verplaatsing");
    return true;
}

double Vehicle::getLength() const {
    return length;
}

Vehicle::Vehicle(const Vehicle* vehicle) : license_plate(vehicle->getLicense_plate()),
                                                                       current_road(vehicle->getCurrent_road()),
                                                                       current_position(vehicle->getCurrent_position()),
                                                                       current_speed(vehicle->getCurrent_speed()),
                                                                       current_speedup(vehicle->getCurrent_speedup()), length(getLength()) {}


double Vehicle::getCurrent_speedup() const {
    return current_speedup;
}

Vehicle::~Vehicle() {

}
