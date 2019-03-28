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


RoadNetwork::RoadNetwork(std::string filename) {
    try {


        // Open the document
        TiXmlDocument docu;
        if (!docu.LoadFile(filename.c_str())) {
            // Stop the program when an error is raised during opening
            std::cerr << docu.ErrorDesc() << std::endl;
            return;
        }


        // Get into the root tag of the document
        TiXmlElement *ROOT = docu.FirstChildElement();
        // Get the first Lane/Vehicle
        TiXmlElement *current_node = ROOT->FirstChildElement();

        while (current_node != NULL) {

            std::string type = current_node->Value();

            if (type == "BAAN") {

                Road *road = new Road(); // Deze regel is nodig omdat je anders een uninitialized compiling error krijgt
                // TODO: alternatief zoeken

                // Get the specifications from the specification tags in the xml file
                for (TiXmlElement *elem = current_node->FirstChildElement();
                     elem != NULL; elem = elem->NextSiblingElement()) {

                    std::string elemName = elem->Value();
                    std::string el = elem->FirstChild()->ToText()->Value();

                    if (elemName == "naam") {
                        delete road;
                        if(el == ""){
//                            throw "De naam van de baan is gelijk zijn aan een lege string";
                            std::cout << "Ongeldige informatie" << std::endl;
                            continue;
                        }
                        if (retrieveRoad(el) == NULL) {
                            road = new Road;
                            road->setName(el);
                        } else {
                            road = retrieveRoad(el);
                        }
                    } else if (elemName == "snelheidslimiet") {
                        int value = atoi(el.c_str());
                        std::stringstream ss;
                        ss << value;
                        std::string result = ss.str();
                        if(result != el){
//                            throw "De snelheidslimiet is geen integer";
                            std::cout << "De snelheid is geen integer" << std::endl;
                            continue;
                        }

                        if(atoi(el.c_str()) <= 0){
//                            throw "De snelheidslimiet is kleiner of gelijk aan 0";
                            std::cout << "De snelheidslimiet is kleiner dan of gelijk aan 0" << std::endl;
                            continue;
                        }

                        road->setSpeed_limit(atoi(el.c_str()));
                    } else if (elemName == "lengte") {
                        double value = strtod(el.c_str(), NULL);
                        std::stringstream ss;
                        ss << value;
                        std::string result = ss.str();
                        if(result != el){
//                            throw "De lengte is geen double";
                            std::cout << "De lengte is geen double" << std::endl;
                            continue;
                        }

                        if (strtod(el.c_str(), NULL) < 0) {
//                            throw "De lengte is kleiner dan 0";
                            std::cout << "De lengte is kleiner dan 0" << std::endl;
                            continue;
                        }
                        road->setLength(strtod(el.c_str(), NULL));
                    } else if (elemName == "verbinding") {
                        Road *exit_road = new Road;
                        exit_road->setName(el);
                        road->setIntersection(exit_road);
                    } else {
                        std::cout << "Type not recognized, ignoring" << std::endl;
                    }

                }

                add_road(road);

            } else if (type == "VOERTUIG") {

                std::string x = current_node->FirstChild()->FirstChild()->ToText()->Value();

                if (x == "AUTO") {

                    Car *car = new Car;

                    for (TiXmlElement *elem = current_node->FirstChildElement();
                        elem != NULL; elem = elem->NextSiblingElement()) {

                        std::string elemName = elem->Value();
                        std::string el = elem->FirstChild()->ToText()->Value();

                        if (elemName == "nummerplaat") {
                            car->setLicense_plate(el);
                        } else if (elemName == "baan") {
                            Road *road = findRoad(el);
                            if(road == NULL){
                                throw "De baan waarop de auto zou moeten rijden bestaat niet";
                            }
                            car->setCurrent_road(road);
                        } else if (elemName == "positie") {
                            double value = atoi(el.c_str());
                            std::stringstream ss;
                            ss << value;
                            std::string result = ss.str();
                            if(result != el){
//                                throw "De positie is geen double";
                                std::cout << "De positie is geen double" << std::endl;
                                continue;
                            }

                            if(atoi(el.c_str()) < 0){
//                                throw "De positie is kleiner dan 0";
                                std::cout << "De polsitie is kleiner dan 0" << std::endl;
                                continue;
                            }

                            car->setCurrent_position(atoi(el.c_str()));
                        } else if (elemName == "snelheid") {
                            double value = strtod(el.c_str(), NULL);
                            std::stringstream ss;
                            ss << value;
                            std::string result = ss.str();
                            if(result != el){
//                                throw "De snelheid is geen double";
                                std::cout << "De snelheid is geen double" << std::endl;
                                continue;
                            }

                            if(strtod(el.c_str(), NULL) < 0){
//                                throw "De snelheid is kleiner dan 0";
                                std::cout << "De snelheid is kleiner dan 0" << std::endl;
                                continue;
                            }
                            car->setCurrent_speed(strtod(el.c_str(), NULL));
                        }

                    }

                    if(car->getCurrent_position() > car->getCurrent_road()->getLength()){
                        throw "De positie van de auto is groter dan de lengte van de baan";
                    }

                    add_car(car);

                } else {
                    // Vehicle type not (yet) supported
                    std::cout << "Ander type dan auto" << std::endl;
                }

            } else {
                std::cout << "Type not recognized, ignoring" << std::endl;
            }

            current_node = current_node->NextSiblingElement();

        }
    } catch (std::string s){

        // Open the output file
        std::ofstream output_file;
        output_file.open("error.log");

        output_file << s << std::endl;

        std::cout << "See error.log for more information" << std::endl;
    }


    if(!check()){
        throw "Incosistente verkeerssituatie";
    }

    _initCheck = this;
    ENSURE(check_position_cars(), "position");
    ENSURE(check_intersections(), "intersections");
    ENSURE(check_if_cars_on_existing_road(), "exist on road");
    ENSURE(check_space_between_cars(), "rip");
    ENSURE(check(), "Roadnetwork not valid");
}

void RoadNetwork::generateOutputFile(const std::string& filename) {

    // TODO: optie om tijdens automatic simulation een groot bestand te creeren met alle data na elke update

    // Open the output file
    std::ofstream output_file;
    output_file.open(filename.c_str());

    std::string listItem = "\t-> ";

    // List all roads
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

    output_file << "-----------------------------------------------------------" << std::endl;

    output_file.close();
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
        int n = nrOfCars(); // Value to check wether a car has been removed
        generateOutputFile("simulation.txt");
        for (int i = 0; i < nrOfCars(); ++i) {

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

RoadNetwork::RoadNetwork() { _initCheck = this; }



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

bool RoadNetwork::ProperlyInitialised() {
//    for (int i = 0; i < cars.size(); ++i) {
//        if(!cars[i]->ProperlyInit()){
//            return false;
//        }
//    }
//
//    for (int i = 0; i < roads.size(); ++i) {
//        if(!roads[i]->ProperlyInit()){
//            return false;
//        }
//    }

    return _initCheck == this;
}

