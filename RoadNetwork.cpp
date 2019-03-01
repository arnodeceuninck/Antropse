/**
 * @file RoadNetwork.cpp
 * @brief This file will contain the definitions of the functions in RoadNetwork.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include "RoadNetwork.h"

bool RoadNetwork::add_road(Road *road) {
    // TODO: controleer of de road een uniek ID heeft
    roads.push_back(road);
    return true;
}

bool RoadNetwork::add_car(Car *car) {
    // TODO: controleer of er nog geen auto op dezelfde plaats / met dezelfde nummerplaat is
    cars.push_back(car);
    return true;
}
