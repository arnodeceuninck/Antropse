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
     *
     * @param nameRoad A string containing the unique name of the road
     *
     * @return A pointer to the road with nameRoad as name
     *
     * @pre
     * REQUIRE(properlyInitialized(), "The road must be properly initialized");
     */
    Road* findRoad(std::string nameRoad);

    /**
     * Get a list of all roads on the RoadNetwork
     *
     * @return A list containing all roads on the RoadNetwork
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Het netwerk moet deftig geinitialiseerd zijn");
     */
    const std::vector<Road *> &getRoads() const;

    /**
     * Get a list of all cars on the RoadNetwork
     *
     * @return A vector containing all registered cars, inside and outside the network, active or inactive
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Het netwerk moet deftig geinitialiseerd zijn");
     */
    const std::vector<Vehicle *> &getCars() const;

    /**
     * check if the road already exists as an intersection of another road
     *
     * @param nameRoad The name of the road you're about to check
     *
     * @return NULL if there's no road with this name as an intersection of another road
     * @return Road* if there's a road with this name as an intersection of another road
     *
     * @pre
     * REQUIRE(properlyInitialized(), "Het netwerk moet deftig geinitialiseerd zijn");
     */
    Road* retrieveIntersectionRoad(std::string nameRoad);

    /**
     * Check whether a car with the specified license plate is in the roadnetwork
     *
     * @return NULL when the car isn't in the network
     * @return A pointer to the car when he is in the roadNetwork
     *
     * @pre
     * REQUIRE(properlyInitialized(), "The roadnetwork must be properly initialized");
     */
    Vehicle* findCar(std::string license_plate) const;

    /**
     * Get the number of cars
     *
     * @return The number of cars currently positioned on one of the streets of the roadnetwork
     *
     * @pre
     * REQUIRE(properlyInitialized(), "The roadnetwork must be properly initialized");
     */
    int nrOfCars();

    /**
     * Find the car in front of another car on the same road
     *
     * @param car
     *
     * @pre
     * REQUIRE(car != NULL, "De wagen moet bestaan");
     *
     * @return NULL if there is no car in front, else a pointer to this car
     */
    Vehicle* findPreviouscar(const Vehicle* car) const;

    /**
     * Automatische simulatie van het netwerk die door blijft gaan tot er geen wagens meer in het netwerk zitten.
     *
     * @pre
     *  REQUIRE(check(), "Roadnetwork not valid");
     *
     * @post
     *  ENSURE(nrOfCars() == 0, "alle auto's zijn buiten hun wegen gereden, er zijn geen auto's meer in het netwerk");
     *  ENSURE(check(), "Valid roadnnetwork");
     */
    void automaticSimulation(std::ofstream &errStream);

    /**
     * Check to know if a car is on a road that exists in the network
     *
     * @param car
     *
     * @pre
     *  REQUIRE(car != NULL, "De auto moet bestaan");
     *  REQUIRE(findCar(car->getLicense_plate()) != NULL, "De auto moet in het netwerk zitten");
     *
     * @return
     */
    bool carOnExistingRoad(Vehicle *car);

    /**
     * All checks for a valid roadnetwork
     *
     * @return true when the roadnetwork is valid
     */
    bool check();

    /**
     *  check whether all cars on the roadnetwork are on an existing road
     *
     * @pre
     * REQUIRE(properlyInitialized(), "The network must be properly initialized");
     */
    bool checkIfCarsOnExistingRoad();

    /**
     * check whether all car positions are inside the borders of the roads they're driving on
     *
     * @pre
     * REQUIRE(properlyInitialized(), "The network must be properly initialized");
     */
    bool checkPositionCars();

    /**
     * Check whether there is enough space between cars
     *
     * @pre
     * REQUIRE(properlyInitialized(), "The network must be properly initialized");
     */
    bool checkSpaceBetweenCars();

    /**
     * Check whether all intersections exist
     *
     * @pre
     * REQUIRE(properlyInitialized(), "The network must be properly initialized");
     */
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
     *
     * @pre
     * REQUIRE(properlyInitialized(), "The network must be properly initialized");
     */
    bool isEmpty();

    /**
     * Move all cars on the road one time unit and increase the current iteration
     *
     * @pre
     * REQUIRE(properlyInitialized(), "The network must be properly initialized");
     * REQUIRE(check(), "The roadnetwork must be valid);
     *
     * @post
     * Ensure(check(), "The roadnetwork must still be valid");
     */
    void moveAllCars(std::ostream &errStream);

    /**
     * Check whether all cars and roads in the network are properly initialised
     *
     * @return true when everything properly initialised
     */
    bool properlyInitialized() const;

    /**
    * Get the number of roads
    *
    * @return The number of roads in the roadnetwork
    *
    * @pre
    * REQUIRE(properlyInitialized(), "The roadnetwork must be properly initialized");
    */
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
    int findRoadIndex(std::string roadName) const;
};



#endif //ANTROPSE_ROADNETWORK_H
