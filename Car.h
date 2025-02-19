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
class Car : public Vehicle {

public:
    /**
     * Constructor for a car
     *
     * @param licensePlate Unique ID for each vehicle
     * @param currentRoad The road on which the car is driving
     * @param currentPosition The position on the current road from the starting point of this road in m.
     * @param currentSpeed The speed by which the car is driving on the current_road
     *
     * @pre
     *  REQUIRE(currentPosition >= 0, "Huidige positie moet op de zeg liggen");
     *  REQUIRE(currentRoad != NULL, "De weg moet bestaan en volledig geinitialisseerd zijn");
     *  REQUIRE(currentPosition <= currentRoad->getLength(), "Huidige positie moet op de weg liggen");
     *  REQUIRE(currentSpeed <= currentRoad->getSpeed_limit(), "De auto mag niet te snel rijden");
     *  REQUIRE(currentSpeed >= 0, "De auto moet een positieve snelheid hebben");
     */
    Car(const std::string &licensePlate, Road *currentRoad, double currentPosition, double currentSpeed);

    /**
     * An empty constructor for a car
     */
    Car();

    /**
     * Returns the type of this vehicle
     * @return A string representing the type of the vehicle, being a car for this subclass
     */
    virtual std::string getType() const;

    virtual double getLength() const;

    virtual double getMinSpeed() const;

    virtual double getMaxSpeed() const;

    virtual double getMaxSpeedup() const;

    virtual double getMinSpeedup() const;

    virtual char getShortName();

};


#endif //ANTROPSE_CAR_H
