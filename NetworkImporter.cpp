//
// Created by arno on 4/13/19.
//

#include <sstream>
#include "NetworkImporter.h"
#include "TinyXML/tinyxml.h"
#include "Road.h"
#include "Car.h"

SuccessEnum
NetworkImporter::importRoadNetwork(const std::string filename, std::ostream &errStream, RoadNetwork &roadNetwork) {

        // TODO: opsplitsen in functies

        SuccessEnum  endResult = Success;

        // Open the document
        TiXmlDocument docu;
        if (!docu.LoadFile(filename.c_str())) {
            // Stop the program when an error is raised during opening
            errStream << "Import aborted: " << docu.ErrorDesc() << std::endl;
            return ImportAborted;
        }


        // Get into the root tag of the document
        TiXmlElement *ROOT = docu.FirstChildElement();
        if(!strcmp(ROOT->Value(), "ROOT")){
            errStream << "Import aborted: Expected <ROOT> --- </ROOT> " << std::endl;
        }

        // Get the first Lane/Vehicle
        TiXmlElement *current_node = ROOT->FirstChildElement();

        while (current_node != NULL) {

            std::string type = current_node->Value();

            if (type == "BAAN") {

                Road *road = new Road(); // Deze regel is nodig omdat je anders een uninitialized compiling error krijgt

                // Get the specifications from the specification tags in the xml file
                for (TiXmlElement *elem = current_node->FirstChildElement();
                     elem != NULL; elem = elem->NextSiblingElement()) {

                    std::string elemName = elem->Value();
                    std::string el = elem->FirstChild()->ToText()->Value();

                    if (elemName == "naam") {
                        delete road;
                        if(el == ""){
//                            throw "De naam van de baan is gelijk zijn aan een lege string";
                            endResult = PartialImport;
                            errStream << "Ongeldige informatie" << std::endl;
                            continue;
                        }
                        if (roadNetwork.retrieveRoad(el) == NULL) {
                            road = new Road;
                            road->setName(el);
                        } else {
                            road = roadNetwork.retrieveRoad(el);
                        }
                    } else if (elemName == "snelheidslimiet") {
                        int value = atoi(el.c_str());
                        std::stringstream ss;
                        ss << value;
                        std::string result = ss.str();
                        if(result != el){
//                            throw "De snelheidslimiet is geen integer";
                            endResult = PartialImport;
                            std::cout << "De snelheid is geen integer" << std::endl;
                            continue;
                        }

                        if(atoi(el.c_str()) <= 0){
//                            throw "De snelheidslimiet is kleiner of gelijk aan 0";
                            endResult = PartialImport;
                            errStream << "De snelheidslimiet is kleiner dan of gelijk aan 0" << std::endl;
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
                            endResult = PartialImport;
                            errStream << "De lengte is geen double" << std::endl;
                            continue;
                        }

                        if (strtod(el.c_str(), NULL) < 0) {
//                            throw "De lengte is kleiner dan 0";
                            endResult = PartialImport;
                            errStream << "De lengte is kleiner dan 0" << std::endl;
                            continue;
                        }
                        road->setLength(strtod(el.c_str(), NULL));
                    } else if (elemName == "verbinding") {
                        Road *exit_road = new Road;
                        exit_road->setName(el);
                        road->setIntersection(exit_road);
                    } else {
                        endResult = PartialImport;
                        errStream << "Type not recognized, ignoring" << std::endl;
                        continue;
                    }

                }

                roadNetwork.add_road(road);

            } else if (type == "VOERTUIG") {

                // wtf is x? Duidelijke namen aub, dankje x
                std::string x = current_node->FirstChild()->FirstChild()->ToText()->Value();

                if (x == "AUTO") {

                    Car *car = new Car();

                    for (TiXmlElement *elem = current_node->FirstChildElement();
                         elem != NULL; elem = elem->NextSiblingElement()) {

                        std::string elemName = elem->Value();
                        std::string el = elem->FirstChild()->ToText()->Value();

                        if (elemName == "nummerplaat") {
                            car->setLicense_plate(el);
                        } else if (elemName == "baan") {
                            Road *road = roadNetwork.findRoad(el);
                            if(road == NULL){
                                endResult = PartialImport;
                                errStream << "De baan waarop de auto zou moeten rijden bestaat niet" << std::endl;
                            }
                            car->setCurrent_road(road);
                        } else if (elemName == "positie") {
                            double value = atoi(el.c_str());
                            std::stringstream ss;
                            ss << value;
                            std::string result = ss.str();
                            if(result != el){
//                                throw "De positie is geen double";
                                endResult = PartialImport;
                                errStream << "De positie is geen double" << std::endl;
                                continue;
                            }

                            if(atoi(el.c_str()) < 0){
//                                throw "De positie is kleiner dan 0";
                                endResult = PartialImport;
                                errStream << "De polsitie is kleiner dan 0" << std::endl;
                                continue;
                            }

                            car->setCurrent_position(atoi(el.c_str()));
                        } else if (elemName == "snelheid") {
                            // Check whether the speed is a double
                            double value = strtod(el.c_str(), NULL);
                            std::stringstream ss;
                            ss << value;
                            std::string result = ss.str();
                            if(result != el){
//                                throw "De snelheid is geen double";
                                endResult = PartialImport;
                                errStream << "De snelheid is geen double" << std::endl;
                                continue;
                            }

                            if(strtod(el.c_str(), NULL) < 0){
//                                throw "De snelheid is kleiner dan 0";
                                endResult = PartialImport;
                                errStream << "De snelheid is kleiner dan 0" << std::endl;
                                continue;
                            }
                            car->setCurrent_speed(strtod(el.c_str(), NULL));
                        }

                    }

                    if(car->getCurrent_position() > car->getCurrent_road()->getLength()){
                        endResult = PartialImport;
                        errStream << "De positie van de auto is groter dan de lengte van de baan" << std::endl;
                    }

                    roadNetwork.add_car(car);

                } else {
                    // Vehicle type not (yet) supported
                    endResult = PartialImport;
                    errStream << "Ander type dan auto" << std::endl;
                    continue;
                }

            } else {
                endResult = PartialImport;
                errStream << "Type not recognized, ignoring" << std::endl;
            }

            current_node = current_node->NextSiblingElement();

        }

    // TODO: fix dat de verkeerssituatie altijd consistent is
    if(!roadNetwork.check()){
        errStream <<  "Inconsistente verkeerssituatie" << std::endl;
        return ImportFailed;
    }
    return endResult;
}

