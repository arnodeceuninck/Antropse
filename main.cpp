/**
 * @file Vehicle.cpp
 * @brief This file wll contains the initial setup of the system.
 *
 * @author Sien Nuyens
 *
 * @date 28/02/2019
 */

#include <iostream>
#include "tinyxml.h"
#include "Car.h"
#include "Road.h"
#include "RoadNetwork.h"

int main() {

    TiXmlDocument docu;
    if(!docu.LoadFile("test.xml")) {
        std::cerr << docu.ErrorDesc() << std::endl;
        return 1;
    }

    RoadNetwork RoadNetwork;

    TiXmlElement* ROOT = docu.FirstChildElement();
    TiXmlElement* current_node = ROOT->FirstChildElement();

    while(current_node != NULL) {
        std::string type = current_node->Value();
        std::cout<< type<<std::endl;
        if(type == "BAAN"){
            Road road;
            for(TiXmlElement* elem = current_node->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
                std::string elemName = elem->Value();
                std::string el = elem->FirstChild()->ToText()->Value();
                if(elemName == "naam"){
                    road.setName(el);
                } else if(elemName == "snelheidslimiet"){
                    road.setSpeed_limit(stoi(el));
                } else if(elemName == "lengte"){
                    road.setLength(stod(el));
                }
            }
            RoadNetwork.add_road(&road);
        }

        if(type == "VOERTUIG"){
            std::string x = current_node->FirstChild()->FirstChild()->ToText()->Value();
            std::cout << x;
            if(x == "AUTO"){
                Car car;
                for(TiXmlElement* elem = current_node->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
                    std::string elemName = elem->Value();
                    std::string el = elem->FirstChild()->ToText()->Value();
                    if(elemName == "nummerplaat"){
                        car.setLicense_plate(el);
                    } else if(elemName == "baan"){
                        Road* road = RoadNetwork.findRoad(el);
                        car.setCurrent_road(road);
                    } else if(elemName == "positie"){
                        car.setCurrent_position(stoi(el));
                    } else if(elemName == "snelheid"){
                        car.setCurrent_speed(std::stod(el));
                    }
                }
                RoadNetwork.add_car(&car);
            } else{
                std::cout << "Ander type dan auto";
            }
        }
        current_node = current_node->NextSiblingElement();

    }
    std::cout << "Hello, World! :)" << std::endl;
    return 0;
}