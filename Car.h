/**
 * @file Car.h
 * @brief This header file will contain the Car class.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#ifndef ANTROPSE_CAR_H
#define ANTROPSE_CAR_H


#include "Vehicle.h"

/**
 * A class to describe a Car on a RoadNetwork
 */
class Car : public Vehicle{

public:
    /**
     * Constructor for a car
     *
     * @param license_plate Unique ID for each vehicle
     * @param current_road The road on which the car is driving
     * @param current_position The position on the current road from the starting point of this road in m.
     * @param current_speed The speed by which the car is driving on the current_road
     */
    Car(const std::string &license_plate, Road *current_road, int current_position, double current_speed);

    /**
     * An empty constructor for a car
     */
    Car();

    /**
     * Returns the type of this vehicle
     * @return A string representing the type of the vehicle, being a car for this subclass
     */
    virtual std::string getType();

};


#endif //ANTROPSE_CAR_H
