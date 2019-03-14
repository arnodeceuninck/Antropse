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
class Vehicle;

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
    bool add_car(Vehicle* car);

    /**
     * Find a specific road in the RoadNetwork
     * @param nameRoad A string containing the unique name of the road
     * @return A pointer to the road with nameRoad as name
     */
    Road* findRoad(std::string nameRoad);

    /**
     * A constructor making a RoadNetwork from an xml file
     * @param filename The path of the file containing the xml description of the road network
     */
    RoadNetwork(std::string filename);

    /**
     * Generate a simple output to a file containing an overview of the roads and vehicles on the road
     * @param filename The path to the file where you'd like to write the output to.
     */
    void generateOutputFile(const std::string& filename);

    /**
     * Get a list of all roads on the RoadNetwork
     * @return A list containing all roads on the RoadNetwork
     */
    const std::vector<Road *> &getRoads() const;

    /**
     * Get a list of all cars on the RoadNetwork
     * @return A vector containing all registered cars, inside and outside the network, active or inactive
     */
    const std::vector<Vehicle *> &getCars() const;

    /**
     * check if the road already exists as an intersection of another road
     * @param nameRoad The name of the road you're about to check
     * @return NULL if there's no road with this name as an intersection of another road
     * @return Road* if there's a road with this name as an intersection of another road
     */
    Road* retrieveRoad(std::string nameRoad);

    int nrOfCars();

    RoadNetwork();

    Vehicle* findPreviouscar(const Vehicle* car) const;

private:
    std::vector<Road*> roads; /**< A vector containing all roads in the network */
    std::vector<Vehicle*> cars; /**< A vector containing all registered cars, inside and outside the network, active or
                               * inactive. */
};


#endif //ANTROPSE_ROADNETWORK_H
