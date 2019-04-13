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

enum SuccessEnum {ImportFailed, ImportAborted, PartialImport, Success};

class NetworkImporter {
public:
    static SuccessEnum importRoadNetwork(const std::string filename, std::ostream& errStream, RoadNetwork& roadNetwork);
};


#endif //ANTROPSE_NETWORKIMPORTER_H
