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

int main() {

    std::cout << "Hello, World! :)" << std::endl;

    RoadNetwork roadNetwork("tests/DefaulReadFile.xml");
    roadNetwork.generateOutputFile("output.txt");

    return 0;

}