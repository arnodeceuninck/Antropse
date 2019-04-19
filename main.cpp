/**
 * @file Vehicle.cpp
 * @brief This file wll contains the initial setup of the system.
 *
 * @author Sien Nuyens
 *
 * @date 28/02/2019
 */

#include <iostream>
#include <fstream>
#include "Car.h"
#include "Road.h"
#include "RoadNetwork.h"
#include "RoadNetworkExporter.h"
#include "NetworkImporter.h"

int main() {

    std::cout << "Hello, World! :)" << std::endl;

    RoadNetwork roadNetwork;
    RoadNetworkExporter exporter;

    NetworkImporter::importRoadNetwork("tests/inputTests/DefaultReadFile.xml", std::cerr, roadNetwork);

    exporter.documentStart(std::cout);
//    roadNetwork.importNetwork("tests/DefaultReadFile.xml");

    while(!roadNetwork.isEmpty()){
        roadNetwork.moveAllCars(1);
        std::cout << std::endl << std::endl;
        exporter.exportOn(std::cout, roadNetwork);
    }

    exporter.documentEnd(std::cout);
//    roadNetwork.generateOutputFile("output.txt");

    return 0;

}