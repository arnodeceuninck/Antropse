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
    static SuccessEnum importRoadNetwork(const std::string &filename, std::ostream &errStream, RoadNetwork *roadNetwork);
};


#endif //ANTROPSE_NETWORKIMPORTER_H
