/**
 * @file RoadNetwork.cpp
 * @brief This file will contain the definitions of the functions in RoadNetwork.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include <string>
#include "RoadNetwork.h"
#include "Road.h"

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

Road *RoadNetwork::findRoad(std::string nameRoad) {
    for(std::vector<Road*>::iterator r = roads.begin(); r != roads.end(); r++){
        if((*r)->getName() == nameRoad){
            return *r;
        }
    }
}
