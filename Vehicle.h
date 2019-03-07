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

/**
 * A class used to describe a Vehicle on a RoadNetwork
 */
class Vehicle {
public:
    
    /**
     * Constructor for a Vehicle
     * 
     * @param license_plate A unique string identifying the vehicle. 
     * @param current_road  The current road the vehicle is positioned on.
     * @param current_position The current distance from the starting point of the road in m.
     * @param current_speed The speed by which the vehicle is driving on the road in km / h.
     * @param length The length of the vehicle in m.
     */
    Vehicle(const std::string &license_plate, Road *current_road, int current_position, double current_speed,
            double length);

    Vehicle();

    void setLicense_plate(const std::string &license_plate);

    void setCurrent_road(Road *current_road);

    void setCurrent_position(int current_position);

    void setCurrent_speed(double current_speed);

    void setCurrent_speedup(double current_speedup);

    void setLength(double length);
    
    /**
     * Move the vehicle
     * 
     * The vehicle position will be updated to his position after a specific time. When the vehicle reaches the end of the road,
     * he will automatically take the next road.
     * 
     * @param time The time between the current position and the new position in s. This determines the distance moved. 
     * @return true when successfully added, false when the prerequisites weren't met
     */
    bool move(const double& time);
private:
    std::string license_plate; /**< A unique ID for each vehicle */
    Road* current_road; /**< The road on which the vehicle is currently positioned */
    int current_position; /**< The position on the current road. This is the distance from the start of this road in m.
                             */
    double current_speed; /**< The current driving speed in km / h. A vehicle can't drive backwards in our system, so this number must
                             * be greater than or equal to 0.*/
    double current_speedup; /**< The speed by which the vehicle is accelerated in km / s^2. */
    double length;

    /**< The length of the vehicle in m. */
};


#endif //ANTROPSE_VEHICLE_H
