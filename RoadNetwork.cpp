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

bool RoadNetwork::add_road(Road *road) {
    // TODO: controleer of de road een uniek ID heeft
    roads.push_back(road);
    return true;
}

bool RoadNetwork::add_car(Vehicle *car) {
    // TODO: controleer of er nog geen auto op dezelfde plaats / met dezelfde nummerplaat is
    cars.push_back(car);
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
}


RoadNetwork::RoadNetwork(std::string filename) {
    // Open the document
    TiXmlDocument docu;
    if(!docu.LoadFile("test.xml")) {
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

            Road* road = new Road;

            // Get the specifications from the specification tags in the xml file
            for(TiXmlElement* elem = current_node->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {

                std::string elemName = elem->Value();
                std::string el = elem->FirstChild()->ToText()->Value();

                if(elemName == "naam"){
                    road->setName(el);
                } else if(elemName == "snelheidslimiet"){
                    road->setSpeed_limit(atoi(el.c_str()));
                } else if(elemName == "lengte"){
                    road->setLength(strtod(el.c_str(), NULL));
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
                        car->setCurrent_position(atoi(el.c_str()));
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

Road *RoadNetwork::retrieveRoad(std::string nameRoad) {
    for(std::vector<Road*>::iterator road = roads.begin(); road != roads.end(); road++){
        for(std::vector<Intersection*>::const_iterator intersecion = (*road)->getIntersecions().begin(); intersecion != (*road)->getIntersecions().end(); intersecion++){
            if((*intersecion)->getExit_road()->getName() == nameRoad){
                return (*intersecion)->getExit_road();
            }
        }
    }
    return NULL;
}

