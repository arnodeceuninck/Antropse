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
    RoadNetwork();

    /**
     * Add a new road to the network
     *
     * @param road The road you want to add
     *
     * @pre
     *  REQUIRE(road != NULL, "De weg moet bestaan");
     *  REQUIRE(findRoad(road->getName()) == NULL, "De weg mag nog niet in het netwerk zitten");
     *
     *  @post
     *   ENSURE(findRoad(road->getName()) == road, "De weg moet nu wel in het netwerk zitten, omdat hij nu is toegevoegd");
     *
     * @return true when successfully added, false when the prerequisites weren't met
     */
    bool addRoad(Road *road);

    /**
     * Add a new car to the network
     *
     * @param road The car you want to add
     *
     * @pre
     *  REQUIRE(car != NULL, "De auto moet bestaan");
     *  REQUIRE(findCar(car->getLicense_plate()) == NULL, "De auto mag nog niet in het netwerk zitten");
     *
     * @post
     *  ENSURE(findCar(car->getLicense_plate()) == car, "De auto moet nu wel in het netwerk zitten, omdat hij nu is toegevoegd");
     *
     * @return true when successfully added, false when the prerequisites weren't met
     */
    bool addCar(Vehicle *car);

    /**
     * Find a specific road in the RoadNetwork
     * @param nameRoad A string containing the unique name of the road
     * @return A pointer to the road with nameRoad as name
     */
    Road* findRoad(std::string nameRoad);

//    /**
//     * A constructor making a RoadNetwork from an xml file
//     * @pre
//     *  REQUIRE(fileExists(filename)
//     * @param filename The path of the file containing the xml description of the road network
//     */
//    void importNetwork(std::string filename);
//
//    /**
//     * Generate a simple output to a file containing an overview of the roads and vehicles on the road
//     * @param filename The path to the file where you'd like to write the output to.
//     */
//    void generateOutputFile(const std::string& filename);

    /**
     * Get a list of all roads on the RoadNetwork
     * @return A list containing all roads on the RoadNetwork
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Het netwerk moet deftig geinitialiseerd zijn");
     */
    const std::vector<Road *> &getRoads() const;

    /**
     * Get a list of all cars on the RoadNetwork
     * @return A vector containing all registered cars, inside and outside the network, active or inactive
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Het netwerk moet deftig geinitialiseerd zijn");
     */
    const std::vector<Vehicle *> &getCars() const;

    /**
     * check if the road already exists as an intersection of another road
     * @param nameRoad The name of the road you're about to check
     * @return NULL if there's no road with this name as an intersection of another road
     * @return Road* if there's a road with this name as an intersection of another road
     */
    Road* retrieveRoad(std::string nameRoad);

    Vehicle* findCar(std::string license_plate) const;

    int nrOfCars();

    /**
     * Find the car in front of another car on the same road
     * @param car
     * @pre
     *  REQUIRE(car != NULL, "De wagen moet bestaan");
     * @return
     */
    Vehicle* findPreviouscar(const Vehicle* car) const;

    /**
     * Automatische simulatie van het netwerk die door blijft gaan tot er geen wagens meer in het netwerk zitten.
     * @pre
     *  REQUIRE(check(), "Roadnetwork not valid");
     * @post
     *  ENSURE(nrOfCars() == 0, "alle auto's zijn buiten hun wegen gereden, er zijn geen auto's meer in het netwerk");
     *  ENSURE(check(), "Valid roadnnetwork");
     */
    void automaticSimulation();

    /**
     * Check to know if a car is on a road that exists in the network
     * @param car
     * @pre
     *  REQUIRE(car != NULL, "De auto moet bestaan");
     *  REQUIRE(findCar(car->getLicense_plate()) != NULL, "De auto moet in het netwerk zitten");
     * @return
     */
    bool carOnExistingRoad(Vehicle *car);

    /**
     * All checks for a valid roadnetwork
     * @return
     */
    bool check();

    bool checkIfCarsOnExistingRoad();

    bool checkPositionCars();

    bool checkSpaceBetweenCars();

    bool checkIntersections();


    /**
     * Deletes a vehicle from the roadnetwork
     * @param licensePlate
     * @pre
     *  REQUIRE(findCar(license_plate) != NULL, "De auto moet in het netwerk zitten");
     *  REQUIRE(cars.size() > 0, "De lijst met auto's mag niet leeg zijn");
     *  unsigned int cars_size = cars.size();
     *
     * @post
     *  ENSURE(findCar(license_plate) == NULL, "De auto zit niet meer in het netwerk");
     *  ENSURE(cars_size-1 == cars.size(), "Er is een element verwijderd uit de lijst");
     */
    void removeVehicle(std::string licensePlate);

    /**
     * Checks whether there are still cars driving on the roadNetwork
     *
     * @return true when there are still cars left in the roadnetwork
     */
    bool isEmpty();

    void moveAllCars(int time);

    /**
     * Check whether all cars and roads in the network are properly initialised
     * @return true when everything properly initialised
     */
    bool properlyInitialized() const;

    int nrOfRoads();

    /**
     * Get the iterations of the roadnetwork
     * @return number of iterations of the roadnetwork
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Het netwerk moet deftig geinitialiseerd zijn");
     */
    int getIteration() const;


private:
    int iteration;
    std::vector<Road*> roads; /**< A vector containing all roads in the network */
    std::vector<Vehicle*> cars; /**< A vector containing all registered cars, inside and outside the network, active or
                               * inactive. */
    RoadNetwork* _initCheck;
};



#endif //ANTROPSE_ROADNETWORK_H
