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
#include "NetworkExporter.h"
#include "NetworkImporter.h"

int main() {

    std::cout << "Hello, World! :)" << std::endl;

    RoadNetwork* roadNetwork = new RoadNetwork();
    NetworkExporter exporter;

    NetworkImporter::importRoadNetwork("test.xml", std::cerr, roadNetwork);

    exporter.documentStart(std::cout);

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars();
        std::cout << std::endl << std::endl;
        exporter.exportOn(std::cout, *roadNetwork);
    }

    exporter.documentEnd(std::cout);

    return 0;

}