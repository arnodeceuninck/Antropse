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
    REQUIRE(properlyInitialized(), "Het voertuig moet deftig geinitialiseerd zijn");
    return licensePlate;
}

Road *Vehicle::getCurrentRoad() const {
    REQUIRE(properlyInitialized(), "Het voertuig moet deftig geinitialiseerd zijn");
    return currentRoad;
}

int Vehicle::getCurrentPosition() const {
    REQUIRE(properlyInitialized(), "Het voertuig moet deftig geinitialiseerd zijn");
    return currentPosition;
}

double Vehicle::getCurrentSpeed() const {
    REQUIRE(properlyInitialized(), "Het voertuig moet deftig geinitialiseerd zijn");
    return currentSpeed;
}

bool Vehicle::setLicensePlate(const std::string &newLicensePlate) {
    if(!(newLicensePlate.size() > 0)){
        return false;
    }
    REQUIRE(newLicensePlate.size() > 0, "Je nummerplaat kan niet leeg zijn");
    Vehicle::licensePlate = newLicensePlate;
    ENSURE(getLicensePlate() == licensePlate, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
    return true;
}

bool Vehicle::setCurrentRoad(Road *newCurrentRoad) {
    if(!(currentSpeed <= newCurrentRoad->getSpeedLimit())){
        return false;
    }
    REQUIRE(currentSpeed <= newCurrentRoad->getSpeedLimit(), "De huidige nelheid van je voertuig moet kleiner zijn dan de max toegelaten snelheid op de weg");
    Vehicle::currentRoad = newCurrentRoad;
    ENSURE(getCurrentRoad() == newCurrentRoad, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
    return true;
}

bool Vehicle::setCurrentPosition(int newCurrentPosition) {
    if(!(newCurrentPosition >= 0)){
        return false;
    }
    REQUIRE(newCurrentPosition >= 0, "De positie moet positief zijn");
    if(currentRoad != NULL) {
        if(!(newCurrentPosition <= currentRoad->getLength())){
            return false;
        }
        REQUIRE(newCurrentPosition <= currentRoad->getLength(), "De positie ligt buiten de huidige weg");
    }
    Vehicle::currentPosition = newCurrentPosition;
    ENSURE(getCurrentPosition() == newCurrentPosition, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
    return true;
}

bool Vehicle::setCurrentSpeed(double newCurrentSpeed) {
    if(!(newCurrentSpeed <= CONST::MAX_CAR_SPEED)){
        return false;
    }
    REQUIRE(newCurrentSpeed <= CONST::MAX_CAR_SPEED, "Maximumsnelheid voor wagen overschreven");
    if(currentRoad != NULL) {
        if(newCurrentSpeed <= currentRoad->getSpeedLimit()){
            return false;
        }
        REQUIRE(newCurrentSpeed <= currentRoad->getSpeedLimit(), "Te snel rijden is verboden");
    }
    Vehicle::currentSpeed = newCurrentSpeed;
    ENSURE(getCurrentSpeed() == currentSpeed, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
    return true;
}

bool Vehicle::setCurrentSpeedup(double newCurrentSpeedup) {
    if(!(currentSpeedup >= CONST::MIN_CAR_SPEEDUP) or !(currentSpeedup <= CONST::MAX_CAR_SPEEDUP)){
        return false;
    }
    REQUIRE(currentSpeedup >= CONST::MIN_CAR_SPEEDUP, "Versnelling te traag");
    REQUIRE(currentSpeedup <= CONST::MAX_CAR_SPEEDUP, "Versnelling te hoog");
    Vehicle::currentSpeedup = newCurrentSpeedup;
    ENSURE(getCurrentSpeedup() == newCurrentSpeedup, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
    return true;
}


Vehicle::Vehicle(): licensePlate(""), currentRoad(NULL),
                                 currentPosition(0), currentSpeed(0),
                                 currentSpeedup(0) {
    _initCheck = this;
}

bool Vehicle::move(RoadNetwork *roadNetwork) {
    REQUIRE(roadNetwork->checkPositionCars(), "position");
    REQUIRE(roadNetwork->checkIfCarsOnExistingRoad(), "exist on road");
    REQUIRE(roadNetwork->findCar(licensePlate) != NULL, "De wagen moet in het netwerk zitten");

    double time = CONST::SECONDS_PER_ITERATION;

    updateCurrentPosition(time);
    updateCurrentSpeed(time);
    updateCurrentSpeedup(time, roadNetwork);

    while (!checkCurrentPositionOnRoad()){
        setCurrentPositionOnNewRoad(roadNetwork);
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
    REQUIRE(properlyInitialized(), "Het voertuig moet deftig geinitialiseerd zijn");
    return currentSpeedup;
}

Vehicle::~Vehicle() {

}

bool Vehicle::properlyInitialized() const{
    return Vehicle::_initCheck == this;
}

void Vehicle::updateCurrentPosition(double time) {
    // Bereken nieuwe positie van voertuig
    currentPosition = Convert::kmhToMs(currentSpeed)*time + currentPosition;

}

void Vehicle::updateCurrentSpeed(double time) {
    // Bereken nieuwe snelheid van voertuig
    currentSpeed = currentSpeedup * time + currentSpeed;
    if(currentSpeed > getMaxSpeed()){
        currentSpeed = getMaxSpeed();
    }
    if(currentSpeed > currentRoad->getSpeedLimit()){
        currentSpeed = currentRoad->getSpeedLimit();
    }

}

bool Vehicle::checkCurrentPositionOnRoad() {
    return !(currentRoad != NULL && currentPosition > currentRoad->getLength());
}

void Vehicle::setCurrentPositionOnNewRoad(RoadNetwork *roadNetwork) {
    REQUIRE(!checkCurrentPositionOnRoad(), "De wagen moet buiten de weg vallen");
    currentPosition = currentPosition - currentRoad->getLength();
    if(currentRoad->getIntersection() != NULL) {
        // IF huidige baan heeft verbinding
        // Zet voertuig op verbindingsbaan
        currentRoad = currentRoad->getIntersection();
    } else {
        // ELSE
        // Verwijder voertuig uit simulatie
        roadNetwork->removeVehicle(licensePlate);
        currentRoad = NULL;
    }
}

void Vehicle::updateCurrentSpeedup(const double &time, RoadNetwork *roadNetwork) {
    // Bereken nieuwe versnelling van voertuig;
    Vehicle* previouscar = roadNetwork->findPreviouscar(this);

    if (previouscar != NULL) {

        double ideal_following_distance = (3 * currentSpeed) / 4 + previouscar->getLength() + CONST::MIN_FOLLOWING_DISTANCE;
        double actual_following_distance =
                previouscar->getCurrentPosition() - previouscar->getLength() - currentPosition;


        currentSpeedup = (actual_following_distance-ideal_following_distance)/2;

//        std::cout << "Following " << ideal_following_distance << " " << actual_following_distance << currentSpeedup << std::endl;
        if (currentSpeedup > getMaxSpeedup()){
            currentSpeedup = getMaxSpeedup();
        }

    } else {
        if(currentSpeed < currentRoad->getSpeedLimit()){
            currentSpeedup = getMaxSpeedup();
        } else {
            currentSpeedup = 0;
        }
    }

    // Controle of je aan deze snelheid niet over de max snelheid gaat
    if(currentSpeedup * time + Convert::kmhToMs(currentSpeed) > Convert::kmhToMs(currentRoad->getSpeedLimit())){
        currentSpeedup = (Convert::kmhToMs(currentRoad->getSpeedLimit())- Convert::kmhToMs(currentSpeed))/time; // Bereken de versnelling die nodig is om net de maximaal toegelaten snelheid te bereiken
    }
}
