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
//#include "Car.h"
#include "Road.h"
//#include "RoadNetwork.h"
#include "NetworkExporter.h"
#include "NetworkImporter.h"
#include "Bus.h"
#include "Convert.h"

int main() {

    std::cout << "Hello, World! :)" << std::endl;

    RoadNetwork* roadNetwork = new RoadNetwork();
    NetworkExporter exporter;

//    NetworkImporter::importRoadNetwork("test.xml", std::cerr, roadNetwork);
    Road* testRoad;
    Vehicle* testVehicle;
    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 70, 70, NULL);

    testRoad->addBusStop(50);
    testVehicle = new Bus("DL4884", testRoad, 20, 0);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);

    testVehicle = new Bus("AAAVALETESTUDIA", testRoad, 0, 0);
    roadNetwork->addCar(testVehicle);

    exporter.documentStart(std::cout);

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars();
        std::cout << std::endl << std::endl;
        exporter.exportOn(std::cout, *roadNetwork);
    }

    exporter.documentEnd(std::cout);

    return 0;

}