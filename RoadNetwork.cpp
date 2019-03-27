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


RoadNetwork::RoadNetwork(std::string filename) {
    // Open the document
    TiXmlDocument docu;
    if(!docu.LoadFile(filename.c_str())) {
        // Stop the program when an error is raised during opening
        std::cerr << docu.ErrorDesc() << std::endl;
        return;
    }


    // Get into the root tag of the document
    TiXmlElement* ROOT = docu.FirstChildElement();
    // Get the first Lane/Vehicle
    TiXmlElement* current_node = ROOT->FirstChildElement();

    while(current_node != NULL) {

        std::string type = current_node->Value();

        if(type == "BAAN"){

            Road* road = new Road(); // Deze regel is nodig omdat je anders een uninitialized compiling error krijgt
                                     // TODO: alternatief zoeken

            // Get the specifications from the specification tags in the xml file
            for(TiXmlElement* elem = current_node->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {

                std::string elemName = elem->Value();
                std::string el = elem->FirstChild()->ToText()->Value();

                if(elemName == "naam"){
                    delete road;
                    if(retrieveRoad(el) == NULL){
                        road = new Road;
                        road->setName(el);
                    } else{
                        road = retrieveRoad(el);
                    }
                } else if(elemName == "snelheidslimiet"){
                    road->setSpeed_limit(atoi(el.c_str()));
                } else if(elemName == "lengte"){
                    road->setLength(strtod(el.c_str(), NULL));
                } else if(elemName == "verbinding"){
                    Road* exit_road = new Road;
                    exit_road->setName(el);
                    road->setIntersection(exit_road);
                }

            }

            add_road(road);

        }

        if(type == "VOERTUIG"){

            std::string x = current_node->FirstChild()->FirstChild()->ToText()->Value();

            if(x == "AUTO"){

                Car* car = new Car;

                for(TiXmlElement* elem = current_node->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {

                    std::string elemName = elem->Value();
                    std::string el = elem->FirstChild()->ToText()->Value();

                    if(elemName == "nummerplaat"){
                        car->setLicense_plate(el);
                    } else if(elemName == "baan"){
                        Road* road = findRoad(el);
                        car->setCurrent_road(road);
                    } else if(elemName == "positie"){
                        double position = atoi(el.c_str());
                        car->setCurrent_position(position);
                    } else if(elemName == "snelheid"){
                        car->setCurrent_speed(std::strtod(el.c_str(), NULL));
                    }

                }

                add_car(car);

            } else{
                // Vehicle type not (yet) supported
                std::cout << "Ander type dan auto" << std::endl;
            }

        }

        current_node = current_node->NextSiblingElement();

    }

    ENSURE(check(), "Roadnetwork not valid");
}

void RoadNetwork::generateOutputFile(const std::string& filename) {

    // Open the output file
    std::ofstream output_file;
    output_file.open(filename.c_str());

    std::string listItem = "\t-> ";

    // List all roads
    std::vector<Road*> roads = getRoads();
    for(std::vector<Road*>::const_iterator road = roads.begin(); road != roads.end(); road++){

        output_file << "Baan: " << (*road)->getName() << std::endl;

        output_file << listItem << "snelheidslimiet: " << (*road)->getSpeed_limit() << std::endl;
        output_file << listItem << "lengte: " << (*road)->getLength() << std::endl;

        output_file << std::endl;

    }

    // List all vehicles
    std::vector<Vehicle*> vehicles = getCars();
    for(std::vector<Vehicle*>::const_iterator vehicle = vehicles.begin(); vehicle != vehicles.end(); vehicle++){

        output_file << "Voertuig: " << (*vehicle)->getType() << " (" << (*vehicle)->getLicense_plate() << ")" << std::endl;

        output_file << listItem << "baan: " << (*vehicle)->getCurrent_road()->getName() << std::endl;
        output_file << listItem << "positie: " << (*vehicle)->getCurrent_position() << std::endl;
        output_file << listItem << "snelheid: " << (*vehicle)->getCurrent_speed() << std::endl;

        output_file << std::endl;

    }

    output_file.close();
}


// TODO: is het niet logischer dat dit een boolean zou returnen en je de weg dan kan opvragen met findRoad()? ~ Arno
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
    REQUIRE(check(), "Roadnetwork not valid");
    return cars.size();
}


void RoadNetwork::automatic_simulation() {
    REQUIRE(check(), "Roadnetwork not valid");
    while(nrOfCars() > 0){
        int n = nrOfCars();
        for (int i = 0; i < n; ++i) {

            cars[i]->move(1, this);

            // Enkel mogelijk indien de wagen verwijjderd is uit het netwerk
            if(n != nrOfCars()){
                i--;
                n = nrOfCars();
            }
        }
    }

    ENSURE(nrOfCars() == 0, "alle auto's zijn buiten hun wegen gereden, er zijn geen auto's meer in het netwerk");
    ENSURE(check(), "Valid roadnnetwork");
}

RoadNetwork::RoadNetwork() {}

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
    for(unsigned int i = 0; i < cars.size()-1; i++) {
        if(abs(cars[i+1]->getCurrent_position() - cars[i]->getCurrent_position()) < 5){
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

    for (unsigned int i = 0; i < cars.size(); ++i) {
        if(cars[i]->getLicense_plate() == license_plate){
            delete cars[i];
            cars.erase(cars.begin()+i);
        }
    }

    ENSURE(findCar(license_plate) == NULL, "De auto zit niet meer in het netwerk");

}