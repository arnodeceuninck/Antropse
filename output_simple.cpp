/**
 * @file output_simple.cpp
 * @brief Contains the code for creating a simple txt output for the system.
 *
 * @author Arno Deceuninck
 *
 * @date 07/03/2019
 */

#include "output_simple.h"
#include <fstream>
#include "Road.h"
#include "Vehicle.h"

// basic file operations
#include <iostream>
#include <fstream>
using namespace std;

void output_simple(const RoadNetwork &roadNetwork, const string& filename) {
    ofstream output_file;
    output_file.open(filename.c_str());

    std::vector<Road*> roads = roadNetwork.getRoads();
    for(std::vector<Road*>::const_iterator road = roads.begin(); road != roads.end(); road++){
        output_file << "Baan: " << (*road)->getName() << std::endl;
        string tab = "\t-> ";
        output_file << tab << "snelheidslimiet: " << (*road)->getSpeed_limit() << std::endl;
        output_file << tab << "lengte: " << (*road)->getLength() << std::endl;
        output_file << std::endl;
    }

    std::vector<Vehicle*> vehicles = roadNetwork.getCars();
    for(std::vector<Vehicle*>::const_iterator vehicle = vehicles.begin(); vehicle != vehicles.end(); vehicle++){
        output_file << "Voertuig: " << (*vehicle)->getType() << " (" << (*vehicle)->getLicense_plate() << ")" << std::endl;
        string tab = "\t-> ";
        output_file << tab << "baan: " << (*vehicle)->getCurrent_road() << std::endl;
        output_file << tab << "positie: " << (*vehicle)->getCurrent_position() << std::endl;
        output_file << tab << "snelheid: " << (*vehicle)->getCurrent_speed() << std::endl;
        output_file << std::endl;
    }

    output_file.close();
}

