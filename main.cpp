/**
 * @file Vehicle.cpp
 * @brief This file wll contains the initial setup of the system.
 *
 * @author Sien Nuyens
 *
 * @date 28/02/2019
 */

#include <iostream>
#include "Car.h"
#include "Road.h"
#include "RoadNetwork.h"
#include "Intersection.h"

int main() {

    std::cout << "Hello, World! :)" << std::endl;

    RoadNetwork roadNetwork("test.xml");
    roadNetwork.generateOutputFile("output.txt");


    return 0;

}