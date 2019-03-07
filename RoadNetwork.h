/**
 * @file RoadNetwork.h
 * @brief This header file will contain the RoadNetwork class.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#ifndef ANTROPSE_ROADNETWORK_H
#define ANTROPSE_ROADNETWORK_H

#include "vector"

class Road;
class Car;

/**
 * A class used to keep track of all roads and cars in a city
 *
 * A RoadNetwork consists of some roads and some cars. All roads are accessible and so are all cars, regardless of their
 * activity. Once a car has left a road network, it wil stay in the list of cars.
 */
class RoadNetwork {
public:

    /**
     * Add a new road to the network
     *
     * @param road The road you want to add
     * // TODO: Prerequisites
     * @return true when successfully added, false when the prerequisites weren't met
     */
    bool add_road(Road* road);

    /**
     * Add a new car to the network
     *
     * @param road The car you want to add
     * // TODO: Prerequisites
     * @return true when successfully added, false when the prerequisites weren't met
     */
    bool add_car(Car* car);
    Road* findRoad(std::string nameRoad);

private:
    std::vector<Road*> roads; /**< A vector containing all roads in the network */
    std::vector<Car*> cars; /**< A vector containing all registered cars, inside and outside the network, active or
                               * inactive. */
};


#endif //ANTROPSE_ROADNETWORK_H
