/**
 * @file Vehicle.h
 * @brief This header file will contain the Vehicle class.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#ifndef ANTROPSE_VEHICLE_H
#define ANTROPSE_VEHICLE_H


#include <string>

class Road;
class RoadNetwork;

/**
 * A class used to describe a Vehicle on a RoadNetwork
 */
class Vehicle {
public:
    
    /**
     * Constructor for a Vehicle.
     * 
     * @param license_plate A unique string identifying the vehicle. 
     * @param current_road  The current road the vehicle is positioned on.
     * @param current_position The current distance from the starting point of the road in m.
     * @param current_speed The speed by which the vehicle is driving on the road in km / h.
     * @param length The length of the vehicle in m.
     */
    Vehicle(const std::string &license_plate, Road *current_road, int current_position, double current_speed,
            double length);

    /**
     * Empty constructor for a Vehicle.
     *
     * @param length The length of the vehicle in m.
     */
    Vehicle(double length);

    /**
     * Change/set the license plate for a Vehicle.
     *
     * @param license_plate A string containing a unique ID for this Vehicle.
     */
    void setLicense_plate(const std::string &license_plate);

    /**
     * A function to set a Vehicle on a/another road
     *
     * @param current_road A pointer to the road the car will be driving on.
     */
    void setCurrent_road(Road *current_road);

    /**
     * Change the position on the road of a Vehicle
     *
     * @param current_position The distance from the starting point of the road (in m)
     */
    void setCurrent_position(int current_position);

    /**
     * Change the speed of a Vehicle
     *
     * @param current_speed The new speed in km/h
     */
    void setCurrent_speed(double current_speed);

    /**
     * Change the speedup of a Vehicle.
     *
     * This value will determine how fast the speed of the car is changing. A positive value means an increasing  speed,
     * while a negative speedup makes the Vehicle go slower.
     *
     * @param current_speedup The speedup in m/s^2
     */
    void setCurrent_speedup(double current_speedup);

    /**
     * Move the vehicle
     * 
     * The vehicle position will be updated to his position after a specific time. When the vehicle reaches the end of the road,
     * he will automatically take the next road.
     * 
     * @param time The time between the current position and the new position in s. This determines the distance moved.
     * @param roadNetwork The network containing the traffic situation.
     *
     * @pre The systems contains a scheme of the virtual traffic situation. There is a vehicle on a road.
     * @post The vehicle has a new position.
     *
     * @return true when successfully added, false when the prerequisites weren't met
     */
    bool move(const double& time, const RoadNetwork* roadNetwork);

    /**
     * Get the type of the Vehicle as a string.
     *
     * A Car is the only type supported (other types are still W.I.P.
     *
     * @return The type of the vehicle as string
     */
    virtual std::string getType()=0;

    /**
     * A function to get the unique identification string of a vehicle.
     *
     * @return The license plate of the Vehicle as string.
     */
    const std::string &getLicense_plate() const;

    /**
     * Get the road the Vehicle is driving on
     * @return a pointer to the road the vehicle is driving on
     */
    Road *getCurrent_road() const;

    /**
     * Get the position on the road of the current Vehicle
     * @return The position of the Vehicle (in m) from the starting point of the road
     */
    int getCurrent_position() const;

    /**
     * Get the speed at which the car is driving
     * @return A double containing the current speed of the car
     */
    double getCurrent_speed() const;

    /**
     * The length of the vehicle in m.
     */
    double getLength() const;

    /**
     * Bool is vehicle is active or not
     */
    bool isActive() const;


private:
    std::string license_plate; /**< A unique ID for each vehicle */
    Road* current_road; /**< The road on which the vehicle is currently positioned */
    double current_position; /**< The position on the current road. This is the distance from the start of this road in m.
                             */
    double current_speed; /**< The current driving speed in km / h. A vehicle can't drive backwards in our system, so this number must
                                * be greater than or equal to 0.*/
    double current_speedup; /**< The speed by which the vehicle is accelerated in m / s^2. */
    double length;
    bool active;

};


#endif //ANTROPSE_VEHICLE_H
