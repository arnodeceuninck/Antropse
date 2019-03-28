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
    REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
    return license_plate;
}

Road *Vehicle::getCurrent_road() const {
    REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
    return current_road;
}

int Vehicle::getCurrent_position() const {
    REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
    return current_position;
}

double Vehicle::getCurrent_speed() const {
    REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
    return current_speed;
}

void Vehicle::setLicense_plate(const std::string &license_plate) {
    REQUIRE(license_plate.size() > 0, "Je nummerplaat kan niet leeg zijn");
    Vehicle::license_plate = license_plate;
}

void Vehicle::setCurrent_road(Road *current_road) {
    Vehicle::current_road = current_road;
}

void Vehicle::setCurrent_position(int current_position) {
    REQUIRE(current_position >= 0, "De positie moet positief zijn");
    if(current_road != NULL) {
        REQUIRE(current_position <= current_road->getLength(), "De positie ligt buiten de huidige weg");
    }
    Vehicle::current_position = current_position;
}

void Vehicle::setCurrent_speed(double current_speed) {
    REQUIRE(current_speed <= CONST::MAX_CAR_SPEED, "Maximumsnelheid voor wagen overschreven");
    if(current_road != NULL) {
        REQUIRE(current_speed <= current_road->getSpeed_limit(), "Te snel rijden is verboden");
    }
    Vehicle::current_speed = current_speed;
}

void Vehicle::setCurrent_speedup(double current_speedup) {
    REQUIRE(current_speedup >= CONST::MIN_CAR_SPEEDUP, "Versnelling te traag");
    REQUIRE(current_speedup <= CONST::MAX_CAR_SPEEDUP, "Versnelling te hoog");
    Vehicle::current_speedup = current_speedup;
}


Vehicle::Vehicle(double length): license_plate(""), current_road(NULL),
                                 current_position(0), current_speed(0),
                                 current_speedup(0), length(length) {}

bool Vehicle::move(const double &time, RoadNetwork *roadNetwork) {
    REQUIRE(roadNetwork->check_position_cars(), "position");
    REQUIRE(roadNetwork->check_if_cars_on_existing_road(), "exist on road");
    REQUIRE(time >= 0, "Tijd moet positief zijn");
    REQUIRE(roadNetwork->findCar(license_plate) != NULL, "De wagen moet in het netwerk zitten");

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

        double ideal_following_distance = (3 * current_speed) / 4 + previouscar->getLength() + CONST::MIN_FOLLOWING_DISTANCE;
        double actual_following_distance =
                previouscar->getCurrent_position() - previouscar->getLength() - current_position;


        current_speedup = (actual_following_distance-ideal_following_distance)/2;

//        std::cout << "Following " << ideal_following_distance << " " << actual_following_distance << current_speedup << std::endl;
        if (current_speedup > CONST::MAX_CAR_SPEEDUP){
            current_speedup = CONST::MAX_CAR_SPEEDUP;
        }

    } else {
        if(current_speed < current_road->getSpeed_limit()){
            current_speedup = CONST::MAX_CAR_SPEEDUP;
        } else {
            current_speedup = 0;
        }
    }

    // Controle of je aan deze snelheid niet over de max snelheid gaat
    if(current_speedup * time + Convert::kmh_to_ms(current_speed) > Convert::kmh_to_ms(current_road->getSpeed_limit())){
        current_speedup = (Convert::kmh_to_ms(current_road->getSpeed_limit())-Convert::kmh_to_ms(current_speed))/time; // Bereken de versnelling die nodig is om net de maximaal toegelaten snelheid te bereiken
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
            break;
        }
    }

//    std::cout << "Car " << license_plate << " " << current_position << std::endl;
    ENSURE(roadNetwork->check_position_cars(), "position");
    ENSURE(roadNetwork->check_if_cars_on_existing_road(), "exist on road");
    // Space between cars is not guaranteed, because all cars have to be moved for this.
    return true;
}

double Vehicle::getLength() const {
    REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
    return length;
}

Vehicle::Vehicle(const Vehicle* vehicle) : license_plate(vehicle->getLicense_plate()),
                                                                       current_road(vehicle->getCurrent_road()),
                                                                       current_position(vehicle->getCurrent_position()),
                                                                       current_speed(vehicle->getCurrent_speed()),
                                                                       current_speedup(vehicle->getCurrent_speedup()), length(getLength()) {}


double Vehicle::getCurrent_speedup() const {
    REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
    return current_speedup;
}

Vehicle::~Vehicle() {

}

bool Vehicle::ProperlyInit() const{
    return (license_plate.size() > 0 &&
            current_road != NULL &&
            current_speed > CONST::MIN_CAR_SPEED &&
            current_speed < current_road->getSpeed_limit() &&
            current_speed < CONST::MAX_CAR_SPEED &&
            current_speedup < CONST::MAX_CAR_SPEEDUP &&
            current_speedup > CONST::MIN_CAR_SPEEDUP);
}
