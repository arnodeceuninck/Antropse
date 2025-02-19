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
#include "Bus.h"
#include "Road.h"
#include "NetworkExporter.h"
#include "NetworkImporter.h"
#include "RoadNetwork.h"
#include "GraphicImpressionExporter.h"


int main() {
    srand(time(0));

    std::cout << "Hello, World! :)" << std::endl;

    RoadNetwork *roadNetwork = new RoadNetwork();
    GraphicImpressionExporter exporter;
    //NetworkExporter exporter;

    NetworkImporter::importRoadNetwork("test.xml", std::cerr, roadNetwork);

    exporter.documentStart(std::cout);

    while (!roadNetwork->isEmpty()) {
        roadNetwork->moveAllCars(std::cerr);
        std::cout << std::endl << std::endl;
        exporter.exportOn(std::cout, *roadNetwork);
    }

    exporter.documentEnd(std::cout);

    return 0;

}