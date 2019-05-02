/**
 * @file RoadNetwork.cpp
 * @brief This file will contain the definitions of the functions in RoadNetwork.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include <string>
#include <iostream>
#include <fstream>
#include "TinyXML/tinyxml.h"
#include "RoadNetwork.h"
#include "Road.h"
#include "Car.h"
#include "DesignByContract.h"
#include "sstream"

#include "CONST.h"
bool RoadNetwork::addRoad(Road *road) {
    REQUIRE(road != NULL, "De weg moet bestaan");
    REQUIRE(findRoad(road->getName()) == NULL, "De weg mag nog niet in het netwerk zitten");

    roads.push_back(road);

    ENSURE(findRoad(road->getName()) == road, "De weg moet nu wel in het netwerk zitten, omdat hij nu is toegevoegd");
    return true;
}

bool RoadNetwork::addCar(Vehicle *car) {
    REQUIRE(car != NULL, "De auto moet bestaan");
    REQUIRE(findCar(car->getLicensePlate()) == NULL, "De auto mag nog niet in het netwerk zitten");

    cars.push_back(car);

    if(!checkSpaceBetweenCars()){
        cars.pop_back();
        return false;
    }

    ENSURE(findCar(car->getLicensePlate()) == car, "De auto moet nu wel in het netwerk zitten, omdat hij nu is toegevoegd");
    ENSURE(check(), "Het netwerk moet nog werken achteraf");
    return true;
}

const std::vector<Road *> &RoadNetwork::getRoads() const {
    REQUIRE(properlyInitialized(), "Het netwerk moet deftig geinitialiseerd zijn");
    return roads;
}

const std::vector<Vehicle *> &RoadNetwork::getCars() const {
    REQUIRE(properlyInitialized(), "Het netwerk moet deftig geinitialiseerd zijn");
    return cars;
}

Road *RoadNetwork::findRoad(std::string nameRoad) {
    REQUIRE(properlyInitialized(), "The road must be properly initialized");
    for(std::vector<Road*>::iterator r = roads.begin(); r != roads.end(); r++){
        if((*r)->getName() == nameRoad){
            return *r;
        }
    }

    // Indien niet gevonden
    return NULL;
}

// TODO: Kan deze functie aub van naam veranderen
Road *RoadNetwork::retrieveRoad(std::string nameRoad) {
    for(std::vector<Road*>::iterator road = roads.begin(); road != roads.end(); road++){
        if((*road)->getIntersection() != NULL){
            if((*road)->getIntersection()->getName() == nameRoad){
                return (*road)->getIntersection();
            }
        }
    }
    return NULL;
}

Vehicle *RoadNetwork::findPreviouscar(const Vehicle *car) const {
    REQUIRE(car != NULL, "De wagen moet bestaan");

    std::vector<Vehicle*> previousCars;
    for(std::vector<Vehicle*>::const_iterator vehicle = cars.begin(); vehicle != cars.end(); vehicle++){
        if((*vehicle)->getCurrentRoad() == car->getCurrentRoad() && (*vehicle)->getCurrentPosition() >
                                                                              car->getCurrentPosition()){
            previousCars.push_back(*vehicle);
        }
    }
    if(previousCars.size() > 0){
        Vehicle* previousCar = previousCars[0];
        for(std::vector<Vehicle*>::iterator vehicle = previousCars.begin(); vehicle != previousCars.end(); vehicle++){
            if(previousCar->getCurrentPosition() > (*vehicle)->getCurrentPosition()){
                previousCar = (*vehicle);
            }
        }
        return previousCar;
    } else{
        return NULL;
    }

}

int RoadNetwork::nrOfCars() {
    return cars.size();
}


void RoadNetwork::automaticSimulation() {
    REQUIRE(check(), "Roadnetwork not valid");
    while(nrOfCars() > 0){
        moveAllCars();
    }

    ENSURE(nrOfCars() == 0, "alle auto's zijn buiten hun wegen gereden, er zijn geen auto's meer in het netwerk");
    ENSURE(check(), "Valid roadnnetwork");
}

RoadNetwork::RoadNetwork() { _initCheck = this; iteration = 0; }



bool RoadNetwork::carOnExistingRoad(Vehicle *car) {
    REQUIRE(car != NULL, "De auto moet bestaan");
    REQUIRE(findCar(car->getLicensePlate()) != NULL, "De auto moet in het netwerk zitten");
    for(std::vector<Road*>::iterator road = roads.begin(); road != roads.end(); road++){
        if(car->getCurrentRoad() == (*road)){
            return true;
        }
    }
    return false;
}


bool RoadNetwork::checkIfCarsOnExistingRoad() {
    for(std::vector<Vehicle*>::iterator car = cars.begin(); car != cars.end(); car++) {
        if (carOnExistingRoad((*car)) == false) {
            return false;
        }
    }
    return true;
}

bool RoadNetwork::checkPositionCars() {
    REQUIRE(properlyInitialized(), "Must be properly initialized");
    for(std::vector<Vehicle*>::iterator car = cars.begin(); car != cars.end(); car++) {
        if((*car)->getCurrentPosition() > (*car)->getCurrentRoad()->getLength()){
            return false;
        }
    }
    return true;
}

bool RoadNetwork::checkSpaceBetweenCars() {
//    for(unsigned int i = 0; i < cars.size()-1; i++) {
//        if(abs(cars[i+1]->getCurrent_position() - cars[i+1]->getLength() - cars[i]->getCurrentPosition()) < 5){
//            return false;
//        }
//    }
    if(cars.size() == 0){ return true; }
    for(unsigned int i = 0; i < cars.size(); i++) {
        Vehicle* previouscar = findPreviouscar(cars[i]);
        if(previouscar != NULL &&
                previouscar->getCurrentPosition() - previouscar->getLength() - cars[i]->getCurrentPosition() < CONST::MIN_FOLLOWING_DISTANCE){
//            std::cout << previouscar->getLicensePlate() << " " << previouscar->getCurrentPosition() << " " << cars[i]->getLicensePlate() << " " << cars[i]->getCurrentPosition() << std::endl;
//            std::cout << previouscar->getCurrentPosition() - previouscar->getLength() - cars[i]->getCurrentPosition() << std::endl;
            return false;
        }
    }
    return true;
}

bool RoadNetwork::checkIntersections() {
    for(std::vector<Road*>::iterator road = roads.begin(); road != roads.end(); road++){
        if((*road)->getIntersection() != NULL && retrieveRoad((*road)->getIntersection()->getName()) == NULL){
            return false;
        }

    }
    return true;
}

bool RoadNetwork::check() {
    if(checkIfCarsOnExistingRoad() && checkPositionCars() && checkSpaceBetweenCars() && checkIntersections()){
        return true;
    } else{
        return false;
    }
}

Vehicle *RoadNetwork::findCar(std::string license_plate) const {
    REQUIRE(properlyInitialized(), "The roadnetwork must be properly initialized");
    for(std::vector<Vehicle*>::const_iterator car = cars.begin(); car != cars.end(); car++){
        if((*car)->getLicensePlate() == license_plate){
            return (*car);
        }
    }
    return NULL;
}

void RoadNetwork::removeVehicle(std::string licensePlate) {
    REQUIRE(findCar(licensePlate) != NULL, "De auto moet in het netwerk zitten");
    REQUIRE(cars.size() > 0, "De lijst met auto's mag niet leeg zijn");
    unsigned int cars_size = cars.size();

    for (unsigned int i = 0; i < cars.size(); ++i) {
        if(cars[i]->getLicensePlate() == licensePlate){
            delete cars[i];
            cars.erase(cars.begin()+i);
        }
    }

    ENSURE(findCar(licensePlate) == NULL, "De auto zit niet meer in het netwerk");
    ENSURE(cars_size-1 == cars.size(), "Er is een element verwijderd uit de lijst");

}

bool RoadNetwork::properlyInitialized() const {
    return RoadNetwork::_initCheck == this;
}

bool RoadNetwork::isEmpty() {
    return cars.empty();
}

void RoadNetwork::moveAllCars() {
    iteration++;
    int n = nrOfCars(); // Value to check wether a car has been removed
//    generateOutputFile("simulation.txt");
    for (int i = 0; i < nrOfCars(); ++i) {

        cars[i]->move(this);

        // Enkel mogelijk indien de wagen verwijjderd is uit het netwerk
        if(n != nrOfCars()){
            i--;
            n = nrOfCars();
        }
    }
}

int RoadNetwork::nrOfRoads() {
    return roads.size();
}

int RoadNetwork::getIteration() const {
    REQUIRE(properlyInitialized(), "Het netwerk moet deftig geinitialiseerd zijn");
    return iteration;
}

// TODO: Delete the lines below this
//bool RoadNetwork::addBusStop(std::string roadName, int position) {
//    Road* road = findRoad(roadName);
//    return false;
//}
//
//bool RoadNetwork::addZone(std::string roadName, double position, int speedLimit) {
//    return false;
//}
//
//bool RoadNetwork::addTrafficLight(std::string roadName, int position) {
//    return false;
//}
//
