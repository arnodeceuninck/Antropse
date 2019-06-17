/**
 * @file Bus.h
 * @brief This header file will contain the Bus class.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#ifndef ANTROPSE_BUS_H
#define ANTROPSE_BUS_H


#include "Vehicle.h"

/**
 * A class to describe a Bus on a RoadNetwork
 */
class Bus : public Vehicle{

public:
    /**
     * Constructor for a bus
     *
     * @param licensePlate Unique ID for each vehicle
     * @param currentRoad The road on which the bus is driving
     * @param currentPosition The position on the current road from the starting point of this road in m.
     * @param currentSpeed The speed by which the bus is driving on the current_road
     *
     * @pre
     *  REQUIRE(currentPosition >= 0, "Huidige positie moet op de zeg liggen");
     *  REQUIRE(currentRoad != NULL, "De weg moet bestaan en volledig geinitialisseerd zijn");
     *  REQUIRE(currentPosition <= currentRoad->getLength(), "Huidige positie moet op de weg liggen");
     *  REQUIRE(currentSpeed <= currentRoad->getSpeed_limit(), "De auto mag niet te snel rijden");
     *  REQUIRE(currentSpeed >= 0, "De auto moet een positieve snelheid hebben");
     */
    Bus(const std::string &licensePlate, Road *currentRoad, double currentPosition, double currentSpeed);

    /**
     * An empty constructor for a bus
     */
    Bus();

    /**
     * Returns the type of this vehicle
     * @return A string representing the type of the vehicle, being a bus for this subclass
     */
    virtual std::string getType() const ;

    virtual double getLength() const;

    virtual double getMinSpeed() const;
    virtual double getMaxSpeed() const;
    virtual double getMaxSpeedup() const;
    virtual double getMinSpeedup() const;


//    double getWaitingTime() const;
//
//    void setWaitingTime(double waitingTime);

    /**
     * Checks whether there is a bus stop nearby and slows down when required
     * @param roadNetwork The roadnetwork on which the bus is driving
     * @param errStream The output stream to which errors should be sent
     *
     * @pre
     * REQUIRE(properlyInitialized(), "The vehicle must be properly initialized");
     * REQUIRE(roadNetwork->properlyInitialized(), "The roadnetwork must be properly initialized");
     * REQUIRE(roadNetwork->check(), "The roadnetwork must be in a valid state");
     *
     * @post
     * ENSURE(roadNetwork->check(), "The roadnetwork stays correct");
     */
    virtual void checkVehicleSpecificMove(RoadNetwork *roadNetwork, std::ostream &errStream);

    virtual char getShortName();

private:
    int waitingTime;
};


#endif //ANTROPSE_BUS_H
