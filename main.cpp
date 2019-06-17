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
#include "NetworkExporter.h"
#include "NetworkImporter.h"
#include "RoadNetwork.h"
#include "GraphicImpressionExporter.h"


int main() {
    srand(time(0));

    std::cout << "Hello, World! :)" << std::endl;

    RoadNetwork* roadNetwork = new RoadNetwork();
    GraphicImpressionExporter exporter;
    //NetworkExporter exporter;

    //NetworkImporter::importRoadNetwork("test.xml", std::cerr, roadNetwork);

    Road* testRoad;
    Vehicle* testVehicle;


    roadNetwork = new RoadNetwork();
    testRoad = new Road("N173", 50, 500, NULL);

    testRoad->addTrafficLight(250);
    testVehicle = new Car("AL4764", testRoad, 0, 0);

    roadNetwork->addRoad(testRoad);
    roadNetwork->addCar(testVehicle);




    exporter.documentStart(std::cout);

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars(std::cerr);
        std::cout << std::endl << std::endl;
        exporter.exportOn(std::cout, *roadNetwork);
    }

    exporter.documentEnd(std::cout);

    return 0;

}