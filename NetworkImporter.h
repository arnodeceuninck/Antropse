/**
 * @author Arno Deceuninck
 * @refitem Serge Demeyer
 */
//
// Created by arno on 4/13/19.
//

#ifndef ANTROPSE_NETWORKIMPORTER_H
#define ANTROPSE_NETWORKIMPORTER_H

#include <iostream>
#include <string>
#include "RoadNetwork.h"
#include "TinyXML/tinyxml.h"

enum SuccessEnum {ImportFailed, ImportAborted, PartialImport, Success};

class NetworkImporter {
private:
    static void readRoad(TiXmlElement *current_node, RoadNetwork *roadNetwork, SuccessEnum &endResult,
                         std::ostream &errStream);
    static void readVehicle(TiXmlElement *current_node, RoadNetwork *roadNetwork, SuccessEnum &endResult,
                            std::ostream &errStream, Vehicle *car);
    static void readRoadSign(TiXmlElement *current_node, RoadNetwork *roadNetwork, SuccessEnum &endResult,
                             std::ostream &errStream);
public:
    /**
     * Import a roadnetwork from an xml file
     *
     * @param filename The name of the file you want to import the network from
     * @param errStream The stream to which you want the errors to be written
     * @param roadNetwork The roadnetwork where you want to add the elements from the file to
     *
     * @return A SuccessEnum, which could have one of the following values
     * ImportFailed: Something went wrong and we're not sure what has been changed to roadNetwork (This is an error)
     * ImportAborted: Nothing has been changed to your roadNetwork
     * PartialImport: There were a few errors in the xml file, so we had to leave some elements out to maintain a valid road situation
     * Succes: Everything from the file has been read and is added to the roadnetwork
     *
     * @pre
     * REQUIRE(roadNetwork->properlyInitialized(), "Roadnetwork moet juist geinitialiseerd zijn");
     * REQUIRE(roadNetwork->check(), "The roadnetwork must be valid");
     * REQUIRE(fileExists(filename), "Het bestand dat je wil inlezen moet bestaan");
     *
     * @post
     * ENSURE(roadNetwork->check(), "The roadnetwork is still valid");
     *
     */
    static SuccessEnum importRoadNetwork(const std::string &filename, std::ostream &errStream, RoadNetwork *roadNetwork);
};


#endif //ANTROPSE_NETWORKIMPORTER_H
