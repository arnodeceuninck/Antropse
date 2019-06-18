//
// Created by arno on 4/13/19.
//

#include <sstream>
#include "NetworkImporter.h"
#include "TinyXML/tinyxml.h"
#include "Road.h"
#include "Car.h"
#include "MotorBike.h"
#include "Bus.h"
#include "Truck.h"
#include "AntropseUtils.h"
#include "DesignByContract.h"

SuccessEnum
NetworkImporter::importRoadNetwork(const std::string &filename, std::ostream &errStream, RoadNetwork *roadNetwork) {

    if (!(roadNetwork->properlyInitialized() and roadNetwork->check() and fileExists(filename))) {
        errStream << "Import aborted: Not all preconditions met" << std::endl;
        return ImportAborted;
    }

    REQUIRE(roadNetwork->properlyInitialized(), "Roadnetwork moet juist geinitialiseerd zijn");
    REQUIRE(roadNetwork->check(), "The roadnetwork must be valid");
    REQUIRE(fileExists(filename), "Het bestand dat je wil inlezen moet bestaan");

    SuccessEnum endResult = Success;

    // Open the document
    TiXmlDocument docu;
    if (!docu.LoadFile(filename.c_str())) {
        // Stop the program when an error is raised during opening
        errStream << "Import aborted: Error raised when opening the file" << docu.ErrorDesc() << std::endl;
        return ImportAborted;
    }


    // Get into the root tag of the document
    TiXmlElement *ROOT = docu.FirstChildElement();
    std::string rootValue = ROOT->Value();
    if (rootValue != "ROOT") {
        errStream << "Import aborted: Expected <ROOT> --- </ROOT> " << std::endl;
        return ImportAborted;
    }

    // Get the first Lane/Vehicle
    TiXmlElement *current_node = ROOT->FirstChildElement();

    while (current_node != NULL) {

        std::string type = current_node->Value();

        if (type == "BAAN") {
            readRoad(current_node, roadNetwork, endResult, errStream);
        } else if (type == "VOERTUIG") {

            std::string vehicleType = current_node->FirstChild()->FirstChild()->ToText()->Value();

            Vehicle *car;

            if (vehicleType == "AUTO") {
                car = new Car();
            } else if (vehicleType == "MOTORFIETS") {
                car = new MotorBike();
            } else if (vehicleType == "BUS") {
                car = new Bus();
            } else if (vehicleType == "VRACHTWAGEN") {
                car = new Truck();
            } else {
                endResult = PartialImport;
                errStream << "Partial Import: Vehicle type not recognized, ignoring" << std::endl;
                current_node = current_node->NextSiblingElement();
                continue;
            }

            readVehicle(current_node, roadNetwork, endResult, errStream, car);

        } else if (type == "VERKEERSTEKEN") {
            readRoadSign(current_node, roadNetwork, endResult, errStream);
        } else {
            endResult = PartialImport;
            errStream << "Partial Import: Type not recognized, ignoring" << std::endl;

        }

        current_node = current_node->NextSiblingElement();

    }

    if (!roadNetwork->check()) {
        errStream << "Import Failed: Something unknown went wrong :-(" << std::endl;
        return ImportFailed;
    }

    ENSURE(roadNetwork->check(), "The roadnetwork is still valid");
    return endResult;
}

void NetworkImporter::readRoad(TiXmlElement *current_node, RoadNetwork *roadNetwork, SuccessEnum &endResult,
                               std::ostream &errStream) {
    Road *road = new Road(); // Deze regel is nodig omdat je anders een uninitialized compiling error krijgt

    // Get the specifications from the specification tags in the xml file
    for (TiXmlElement *elem = current_node->FirstChildElement();
         elem != NULL; elem = elem->NextSiblingElement()) {

        std::string elemName = elem->Value();

        TiXmlNode *text = elem->FirstChild();
        if (text == NULL) {
            errStream << "Partial Import: De xml node mag niet leeg zijn" << std::endl;
            endResult = PartialImport;
            return;
        }

        std::string el = elem->FirstChild()->ToText()->Value();

        if (elemName == "naam") {
            delete road;
            if (roadNetwork->retrieveIntersectionRoad(el) == NULL) {
                road = new Road;
                if (!road->setName(el)) {
                    endResult = PartialImport;
                    errStream << "Partial Import: Ongeldige informatie bij de naam van de weg" << std::endl;
                    return;
                }
            } else {
                road = roadNetwork->retrieveIntersectionRoad(el);
            }
        } else if (elemName == "snelheidslimiet") {
            if (!road->setSpeedLimit(atoi(el.c_str()))) {
                endResult = PartialImport;
                errStream << "Partial Import: Ongeldige informatie bij de snelheidslimiet van de weg" << std::endl;
                return;
            }
        } else if (elemName == "lengte") {
            if (!road->setLength(strtod(el.c_str(), NULL))) {
                endResult = PartialImport;
                errStream << "Ongeldige informatie" << std::endl;
                return;
            }
        } else if (elemName == "verbinding") {
            Road *exit_road = new Road;
            exit_road->setName(el);
            if (!road->setIntersection(exit_road)) {
                endResult = PartialImport;
                errStream << "Partial Import: Ongeldige informatie bij de verbinding van de weg" << std::endl;
                return;
            }
        } else {
            endResult = PartialImport;
            errStream << "Partial Import: Attribute from road not recognized, ignoring" << std::endl;
//            return;
        }

    }

    if (!roadNetwork->addRoad(road)) {
        endResult = PartialImport;
        errStream << "Partial Import: Ongeldige informatie bij de weg toevoegen aan het netwerk" << std::endl;
        return;
    }

}

void NetworkImporter::readVehicle(TiXmlElement *current_node, RoadNetwork *roadNetwork, SuccessEnum &endResult,
                                  std::ostream &errStream, Vehicle *car) {
    for (TiXmlElement *elem = current_node->FirstChildElement();
         elem != NULL; elem = elem->NextSiblingElement()) {

        std::string elemName = elem->Value();
        std::string el = elem->FirstChild()->ToText()->Value();

        if (elemName == "nummerplaat") {
            if (!car->setLicensePlate(el)) {
                endResult = PartialImport;
                errStream << "Partial Import: Ongeldige informatie bij het toevoegen van de nummerplaat aan de wagen"
                          << std::endl;
                return;
            }
        } else if (elemName == "baan") {
            Road *road = roadNetwork->findRoad(el);
            if (!car->setCurrentRoad(road)) {
                endResult = PartialImport;
                errStream << "Partial Import: Ongeldige informatie bij het toevoegen van de weg aan de auto"
                          << std::endl;
                return;
            }
        } else if (elemName == "positie") {

            if (!car->setCurrentPosition(atoi(el.c_str()))) {
                endResult = PartialImport;
                errStream << "Partial Import: Ongeldige informatie bij het toevoegen van de positie aan auto"
                          << std::endl;
                return;
            }
        } else if (elemName == "snelheid") {
            if (!car->setCurrentSpeed(strtod(el.c_str(), NULL))) {
                endResult = PartialImport;
                errStream << "Partial Import: Ongeldige informatie bij het instellen van de snelheid van de auto"
                          << std::endl;
                return;
            }
        }

    }

//    if(car->getCurrentPosition() > car->getCurrentRoad()->getLength()){
//        endResult = PartialImport;
//        errStream << "De positie van de auto is groter dan de lengte van de baan" << std::endl;
//    }

    if (!roadNetwork->addCar(car)) {
        endResult = PartialImport;
        errStream << "Partial Import: Ongeldige informatie bij het toevoegen van de wagen aan het netwerk" << std::endl;
        return;
    }

}

void NetworkImporter::readRoadSign(TiXmlElement *current_node, RoadNetwork *roadNetwork, SuccessEnum &endResult,
                                   std::ostream &errStream) {
    std::string signType = current_node->FirstChild()->FirstChild()->ToText()->Value();

    if (signType == "BUSHALTE") {

        if (current_node->FirstChildElement("positie") == NULL or current_node->FirstChildElement("baan") == NULL) {
            endResult = PartialImport;
            errStream << "Partial Import: Ontbrekend element bij bushalte" << std::endl;
            return;
        }

        TiXmlNode *roadNode = current_node->FirstChildElement("baan")->FirstChild();
        TiXmlNode *positionNode = current_node->FirstChildElement("positie")->FirstChild();

        std::string roadName = roadNode->ToText()->Value();
        std::string positionStr = positionNode->ToText()->Value();
        if (!checkInt(positionStr)) {
            endResult = PartialImport;
            errStream << "Partial Import: De positie van de bushalte is geen integer" << std::endl;
            return;
        }
        int position = std::strtod(positionStr.c_str(), NULL);

        Road *road = roadNetwork->findRoad(roadName);
        if (road == NULL) {
            endResult = PartialImport;
            errStream << "Partial Import: Geen weg gevonden met naam " << roadName << " voor de bushalte" << std::endl;
            return;
        }

        if (!road->addBusStop(position)) {
            endResult = PartialImport;
            errStream << "Partial Import: Ongeldige gegevens bij de positie van de busstop" << std::endl;
            return;
        }
    } else if (signType == "ZONE") {

        if (current_node->FirstChildElement("snelheidslimiet") == NULL or
            current_node->FirstChildElement("positie") == NULL or current_node->FirstChildElement("baan") == NULL) {
            endResult = PartialImport;
            errStream << "Partial Import: Ontbrekend element bij zone" << std::endl;
            return;
        }

        TiXmlNode *roadNode = current_node->FirstChildElement("baan")->FirstChild();
        TiXmlNode *positionNode = current_node->FirstChildElement("positie")->FirstChild();
        std::string positionStr = positionNode->ToText()->Value();
        if (!checkInt(positionStr)) {
            endResult = PartialImport;
            errStream << "Partial Import: De snelheid van de zone is geen integer" << std::endl;
            return;
        }
        int position = std::strtod(positionStr.c_str(), NULL);

        TiXmlNode *speedLimitNode = current_node->FirstChildElement("snelheidslimiet")->FirstChild();


        std::string roadName = roadNode->ToText()->Value();
//        double position = std::strtod(positionNode->ToText()->Value(), NULL);
        double speedLimit = std::strtod(speedLimitNode->ToText()->Value(), NULL);

        Road *road = roadNetwork->findRoad(roadName);
        if (road == NULL) {
            endResult = PartialImport;
            errStream << "Partial Import: Geen weg gevonden met naam " << roadName << " voor zone" << std::endl;
            return;
        }

        if (!road->addZone(position, speedLimit)) {
            endResult = PartialImport;
            errStream << "Partial Import: Ongeldige gegevens bij zone" << std::endl;
            return;
        }
    } else if (signType == "VERKEERSLICHT") {

        if (current_node->FirstChildElement("positie") == NULL or current_node->FirstChildElement("baan") == NULL) {
            endResult = PartialImport;
            errStream << "Partial Import: Ontbrekend element bij verkeerslicht" << std::endl;
            return;
        }

        TiXmlNode *roadNode = current_node->FirstChildElement("baan")->FirstChild();
        TiXmlNode *positionNode = current_node->FirstChildElement("positie")->FirstChild();
        std::string positionStr = positionNode->ToText()->Value();
        if (!checkInt(positionStr)) {
            endResult = PartialImport;
            errStream << "Partial Import: De snelheid bij verkeerslicht is geen integer" << std::endl;
            return;
        }
        int position = std::strtod(positionStr.c_str(), NULL);

        std::string roadName = roadNode->ToText()->Value();
        //double position = std::strtod(positionNode->ToText()->Value(), NULL);

        Road *road = roadNetwork->findRoad(roadName);
        if (road == NULL) {
            endResult = PartialImport;
            errStream << "Partial Import: Geen weg gevonden met naam " << roadName << " voor verkeerslicht"
                      << std::endl;
            return;
        }

        if (!road->addTrafficLight(position)) {
            endResult = PartialImport;
            errStream << "Partial Import: Ongeldige informatie bij het toevoegen van het verkeerslicht aan de weg"
                      << std::endl;
            return;
        }
    } else {
        endResult = PartialImport;
        errStream << "Partial Import: Verkeersteken niet herkend, overslaan" << std::endl;
    }

}

