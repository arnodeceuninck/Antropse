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
    testRoad = new Road("N173", 120, 5000, NULL);
    roadNetwork->addRoad(testRoad);

    testRoad->addTrafficLight(2500, 0);
    testVehicle = new Car("IIIIIEP", testRoad, 0, 0);
    roadNetwork->addCar(testVehicle);
    testVehicle = new Car("TOETTOET", testRoad, 50, 0);
    roadNetwork->addCar(testVehicle);
    testVehicle = new Car("VROEMVROEM", testRoad, 100, 0);
    roadNetwork->addCar(testVehicle);


    //qugogeqogiw

    exporter.documentStart(std::cout);

    while(!roadNetwork->isEmpty()){
        roadNetwork->moveAllCars(std::cerr);
        std::cout << std::endl << std::endl;
        exporter.exportOn(std::cout, *roadNetwork);
    }

    exporter.documentEnd(std::cout);

    return 0;

}