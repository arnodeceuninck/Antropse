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

Vehicle::Vehicle(const std::string &license_plate, Road *current_road, int current_position, double current_speed)
        : licensePlate(license_plate), currentRoad(current_road),
          currentPosition(current_position), currentSpeed(current_speed), currentSpeedup(0)
                               { _initCheck = this; }


const std::string &Vehicle::getLicensePlate() const {
    REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
    return licensePlate;
}

Road *Vehicle::getCurrentRoad() const {
    REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
    return currentRoad;
}

int Vehicle::getCurrentPosition() const {
    REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
    return currentPosition;
}

double Vehicle::getCurrentSpeed() const {
    REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
    return currentSpeed;
}

void Vehicle::setLicensePlate(const std::string &newLicensePlate) {
    REQUIRE(newLicensePlate.size() > 0, "Je nummerplaat kan niet leeg zijn");
    Vehicle::licensePlate = newLicensePlate;
    ENSURE(getLicensePlate() == licensePlate, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
}

void Vehicle::setCurrentRoad(Road *newCurrentRoad) {
    // TODO: check max speed
    Vehicle::currentRoad = newCurrentRoad;
    ENSURE(getCurrentRoad() == newCurrentRoad, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
}

void Vehicle::setCurrentPosition(int newCurrentPosition) {
    REQUIRE(newCurrentPosition >= 0, "De positie moet positief zijn");
    if(currentRoad != NULL) {
        REQUIRE(newCurrentPosition <= currentRoad->getLength(), "De positie ligt buiten de huidige weg");
    }
    Vehicle::currentPosition = newCurrentPosition;
    ENSURE(getCurrentPosition() == newCurrentPosition, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
}

void Vehicle::setCurrentSpeed(double newCurrentSpeed) {
    REQUIRE(newCurrentSpeed <= CONST::MAX_CAR_SPEED, "Maximumsnelheid voor wagen overschreven");
    if(currentRoad != NULL) {
        REQUIRE(newCurrentSpeed <= currentRoad->getSpeedLimit(), "Te snel rijden is verboden");
    }
    Vehicle::currentSpeed = newCurrentSpeed;
    ENSURE(getCurrentSpeed() == currentSpeed, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
}

void Vehicle::setCurrentSpeedup(double newCurrentSpeedup) {
    REQUIRE(currentSpeedup >= CONST::MIN_CAR_SPEEDUP, "Versnelling te traag");
    REQUIRE(currentSpeedup <= CONST::MAX_CAR_SPEEDUP, "Versnelling te hoog");
    Vehicle::currentSpeedup = newCurrentSpeedup;
    ENSURE(getCurrentSpeedup() == newCurrentSpeedup, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
}


Vehicle::Vehicle(): licensePlate(""), currentRoad(NULL),
                                 currentPosition(0), currentSpeed(0),
                                 currentSpeedup(0) {
    _initCheck = this;
}

bool Vehicle::move(const double &time, RoadNetwork *roadNetwork) {
    REQUIRE(roadNetwork->checkPositionCars(), "position");
    REQUIRE(roadNetwork->checkIfCarsOnExistingRoad(), "exist on road");
    REQUIRE(time >= 0, "Tijd moet positief zijn");
    REQUIRE(roadNetwork->findCar(licensePlate) != NULL, "De wagen moet in het netwerk zitten");

    // Bereken nieuwe positie van voertuig
    currentPosition = Convert::kmhToMs(currentSpeed)*time + currentPosition;

    // Bereken nieuwe snelheid van voertuig
    currentSpeed = currentSpeedup * time + currentSpeed;
    if(currentSpeed > getMaxSpeed()){
        currentSpeed = getMaxSpeed();
    }
    if(currentSpeed > currentRoad->getSpeedLimit()){
        currentSpeed = currentRoad->getSpeedLimit();
    }

    // Bereken nieuwe versnelling van voertuig;
    Vehicle* previouscar = roadNetwork->findPreviouscar(this);

    if (previouscar != NULL) {

        double ideal_following_distance = (3 * currentSpeed) / 4 + previouscar->getLength() + CONST::MIN_FOLLOWING_DISTANCE;
        double actual_following_distance =
                previouscar->getCurrentPosition() - previouscar->getLength() - currentPosition;


        currentSpeedup = (actual_following_distance-ideal_following_distance)/2;

//        std::cout << "Following " << ideal_following_distance << " " << actual_following_distance << currentSpeedup << std::endl;
        if (currentSpeedup > getMaxSpeed()){
            currentSpeedup = getMaxSpeed();
        }

    } else {
        if(currentSpeed < currentRoad->getSpeedLimit()){
            currentSpeedup = getMaxSpeed();
        } else {
            currentSpeedup = 0;
        }
    }

    // Controle of je aan deze snelheid niet over de max snelheid gaat
    if(currentSpeedup * time + Convert::kmhToMs(currentSpeed) > Convert::kmhToMs(currentRoad->getSpeedLimit())){
        currentSpeedup = (Convert::kmhToMs(currentRoad->getSpeedLimit())- Convert::kmhToMs(currentSpeed))/time; // Bereken de versnelling die nodig is om net de maximaal toegelaten snelheid te bereiken
    }

    // IF nieuwe positie valt buiten huidige baan
    while (currentRoad != NULL && currentPosition > currentRoad->getLength()){
        currentPosition = currentPosition - currentRoad->getLength();
        if(currentRoad->getIntersection() != NULL) {
            // IF huidige baan heeft verbinding
            // Zet voertuig op verbindingsbaan
            currentRoad = currentRoad->getIntersection();
        } else {
            // ELSE
            // Verwijder voertuig uit simulatie
            roadNetwork->removeVehicle(licensePlate);
            break;
        }
    }

//    std::cout << "Car " << licensePlate << " " << currentPosition << std::endl;
    ENSURE(roadNetwork->checkPositionCars(), "position");
    ENSURE(roadNetwork->checkIfCarsOnExistingRoad(), "exist on road");
    // Space between cars is not guaranteed, because all cars have to be moved for this.
    return true;
}

Vehicle::Vehicle(const Vehicle* vehicle) : licensePlate(vehicle->getLicensePlate()),
                                                                       currentRoad(vehicle->getCurrentRoad()),
                                                                       currentPosition(vehicle->getCurrentPosition()),
                                                                       currentSpeed(vehicle->getCurrentSpeed()),
                                                                       currentSpeedup(vehicle->getCurrentSpeedup()) {}


double Vehicle::getCurrentSpeedup() const {
    REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
    return currentSpeedup;
}

Vehicle::~Vehicle() {

}

bool Vehicle::ProperlyInit() const{
//    return (licensePlate.size() > 0 &&
//            currentRoad != NULL &&
//            currentSpeed > CONST::MIN_CAR_SPEED &&
//            currentSpeed < currentRoad->getSpeedLimit() &&
//            currentSpeed < CONST::MAX_CAR_SPEED &&
//            currentSpeedup < CONST::MAX_CAR_SPEEDUP &&
//            currentSpeedup > CONST::MIN_CAR_SPEEDUP);

    return Vehicle::_initCheck == this;
}
