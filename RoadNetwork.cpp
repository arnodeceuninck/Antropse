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
bool RoadNetwork::add_road(Road *road) {
    REQUIRE(road != NULL, "De weg moet bestaan");
    REQUIRE(findRoad(road->getName()) == NULL, "De weg mag nog niet in het netwerk zitten");

    roads.push_back(road);

    ENSURE(findRoad(road->getName()) == road, "De weg moet nu wel in het netwerk zitten, omdat hij nu is toegevoegd");
    return true;
}

bool RoadNetwork::add_car(Vehicle *car) {
    REQUIRE(car != NULL, "De auto moet bestaan");
    REQUIRE(findCar(car->getLicense_plate()) == NULL, "De auto mag nog niet in het netwerk zitten");

    cars.push_back(car);

    ENSURE(findCar(car->getLicense_plate()) == car, "De auto moet nu wel in het netwerk zitten, omdat hij nu is toegevoegd");
    return true;
}

const std::vector<Road *> &RoadNetwork::getRoads() const {
    return roads;
}

const std::vector<Vehicle *> &RoadNetwork::getCars() const {
    return cars;
}

Road *RoadNetwork::findRoad(std::string nameRoad) {
    for(std::vector<Road*>::iterator r = roads.begin(); r != roads.end(); r++){
        if((*r)->getName() == nameRoad){
            return *r;
        }
    }

    // Indien niet gevonden
    return NULL;
}

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
        if((*vehicle)->getCurrent_road() == car->getCurrent_road() && (*vehicle)->getCurrent_position() > car->getCurrent_position()){
            previousCars.push_back(*vehicle);
        }
    }
    if(previousCars.size() > 0){
        Vehicle* previousCar = previousCars[0];
        for(std::vector<Vehicle*>::iterator vehicle = previousCars.begin(); vehicle != previousCars.end(); vehicle++){
            if(previousCar->getCurrent_position() > (*vehicle)->getCurrent_position()){
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


void RoadNetwork::automatic_simulation() {
    REQUIRE(check(), "Roadnetwork not valid");
    while(nrOfCars() > 0){
        moveAllCars(1);
    }

    ENSURE(nrOfCars() == 0, "alle auto's zijn buiten hun wegen gereden, er zijn geen auto's meer in het netwerk");
    ENSURE(check(), "Valid roadnnetwork");
}

RoadNetwork::RoadNetwork() { _initCheck = this; iteration = 0; }



bool RoadNetwork::car_on_existing_road(Vehicle *car) {
    REQUIRE(car != NULL, "De auto moet bestaan");
    REQUIRE(findCar(car->getLicense_plate()) != NULL, "De auto moet in het netwerk zitten");
    for(std::vector<Road*>::iterator road = roads.begin(); road != roads.end(); road++){
        if(car->getCurrent_road() == (*road)){
            return true;
        }
    }
    return false;
}


bool RoadNetwork::check_if_cars_on_existing_road() {
    for(std::vector<Vehicle*>::iterator car = cars.begin(); car != cars.end(); car++) {
        if (car_on_existing_road((*car)) == false) {
            return false;
        }
    }
    return true;
}

bool RoadNetwork::check_position_cars() {
    for(std::vector<Vehicle*>::iterator car = cars.begin(); car != cars.end(); car++) {
        if((*car)->getCurrent_position() > (*car)->getCurrent_road()->getLength()){
            return false;
        }
    }
    return true;
}

bool RoadNetwork::check_space_between_cars() {
//    for(unsigned int i = 0; i < cars.size()-1; i++) {
//        if(abs(cars[i+1]->getCurrent_position() - cars[i+1]->getLength() - cars[i]->getCurrent_position()) < 5){
//            return false;
//        }
//    }
    if(cars.size() == 0){ return true; }
    for(unsigned int i = 0; i < cars.size()-1; i++) {
        Vehicle* previouscar = findPreviouscar(cars[i]);
        if(previouscar != NULL &&
           previouscar->getCurrent_position() - previouscar->getLength() - cars[i]->getCurrent_position() < CONST::MIN_FOLLOWING_DISTANCE){
            std::cout << previouscar->getLicense_plate() << " " << previouscar->getCurrent_position() << " " << cars[i]->getLicense_plate() << " " << cars[i]->getCurrent_position() << std::endl;
            std::cout << previouscar->getCurrent_position() - previouscar->getLength() - cars[i]->getCurrent_position() << std::endl;
            return false;
        }
    }
    return true;
}

bool RoadNetwork::check_intersections() {
    for(std::vector<Road*>::iterator road = roads.begin(); road != roads.end(); road++){
        if((*road)->getIntersection() != NULL && retrieveRoad((*road)->getIntersection()->getName()) == NULL){
            return false;
        }

    }
    return true;
}

bool RoadNetwork::check() {
    if(check_if_cars_on_existing_road() && check_position_cars() && check_space_between_cars() && check_intersections()){
        return true;
    } else{
        return false;
    }
}

Vehicle *RoadNetwork::findCar(std::string license_plate) const {
    for(std::vector<Vehicle*>::const_iterator car = cars.begin(); car != cars.end(); car++){
        if((*car)->getLicense_plate() == license_plate){
            return (*car);
        }
    }
    return NULL;
}

void RoadNetwork::removeVehicle(std::string license_plate) {
    REQUIRE(findCar(license_plate) != NULL, "De auto moet in het netwerk zitten");
    REQUIRE(cars.size() > 0, "De lijst met auto's mag niet leeg zijn");
    unsigned int cars_size = cars.size();

    for (unsigned int i = 0; i < cars.size(); ++i) {
        if(cars[i]->getLicense_plate() == license_plate){
            delete cars[i];
            cars.erase(cars.begin()+i);
        }
    }

    ENSURE(findCar(license_plate) == NULL, "De auto zit niet meer in het netwerk");
    ENSURE(cars_size-1 == cars.size(), "Er is een element verwijderd uit de lijst");

}

bool RoadNetwork::properlyInitialized() {
    return _initCheck == this;
}

bool RoadNetwork::isEmpty() {
    return cars.empty();
}

void RoadNetwork::moveAllCars(int time) {
    iteration++;
    int n = nrOfCars(); // Value to check wether a car has been removed
//    generateOutputFile("simulation.txt");
    for (int i = 0; i < nrOfCars(); ++i) {

        cars[i]->move(1, this);

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
    return iteration;
}

