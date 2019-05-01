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
     *
     * @pre
     * All REQUIRES from setLicensePlate, setCurrentRoad, setCurrentPosition and setCurrentSpeed apply (except properlyInit)
     *
     * @post
     * ENSURE(properlyInitialized(), "The vehicle has been properly initialized")
     * All ENSURES from setLicensePlate, setCurrentRoad, setCurrentPosition and setCurrentSpeed apply
     */
    Vehicle(const std::string &license_plate, Road *current_road, double current_position, double current_speed);

    /**
     * Empty constructor for a Vehicle.
     *
     *.ENSURE(properlyInitialized(), "The vehicle has been properly initialized");
     */
    Vehicle();

    Vehicle(const Vehicle* vehicle);

    /**
     * Change/set the license plate for a Vehicle.
     *
     * @param newLicensePlate A string containing a unique ID for this Vehicle.
     *
     * @pre
     * REQUIRE(license_plate.size() > 0, "Je nummerplaat kan niet leeg zijn");
     *
     * @post
     * ENSURE(getLicensePlate() == licensePlate, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
     *
     * @return true when all precoditions are satisfied
     *
     */
    bool setLicensePlate(const std::string &newLicensePlate);

    /**
     * A function to set a Vehicle on a/another road
     *
     * @param newCurrentRoad A pointer to the road the car will be driving on.
     *
     * @pre
     * REQUIRE(currentSpeed <= newCurrentRoad->getSpeedLimit(), "De huidige nelheid van je voertuig moet kleiner zijn dan de max toegelaten snelheid op de weg");
     *
     * @post
     * ENSURE(getCurrentRoad() == newCurrentRoad, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
     *
     * @return true when all precoditions are satisfied
     *
     */
    bool setCurrentRoad(Road *newCurrentRoad);

    /**
     * Change the position on the road of a Vehicle
     *
     * @param current_position The distance from the starting point of the road (in m)
     *
     * @pre
     * REQUIRE(current_position >= 0, "De positie moet positief zijn");
     * if(current_road != NULL) {
     * REQUIRE(current_position <= current_road->getLength(), "De positie ligt buiten de huidige weg");
     * }
     *
     * @post
     * ENSURE(getCurrentPosition() == newCurrentPosition, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
     *
     * @return true when all precoditions are satisfied
     *
     */
    bool setCurrentPosition(int current_position);

    /**
     * Change the speed of a Vehicle
     *
     * @param current_speed The new speed in km/h
     *
     * @pre
     * REQUIRE(current_speed <= CONST::MAX_CAR_SPEED, "Maximumsnelheid voor wagen overschreven");
     * if(current_road != NULL) {
     *    REQUIRE(current_speed <= current_road->getSpeed_limit(), "Te snel rijden is verboden");
     * }
     *
     * @post
     * ENSURE(getCurrentSpeed() == currentSpeed, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
     *
     * @return true when all precoditions are satisfied
     *
     */
    bool setCurrentSpeed(double current_speed);

    /**
     * Change the speedup of a Vehicle.
     *
     * This value will determine how fast the speed of the car is changing. A positive value means an increasing  speed,
     * while a negative speedup makes the Vehicle go slower.
     *
     * @param current_speedup The speedup in m/s^2
     *
     * @pre
     * REQUIRE(current_speedup >= CONST::MIN_CAR_SPEEDUP, "Versnelling te traag");
     * REQUIRE(current_speedup <= CONST::MAX_CAR_SPEEDUP, "Versnelling te hoog");
     *
     * @post
     * ENSURE(getCurrentSpeedup() == newCurrentSpeedup, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
     *
     * @return true when all precoditions are satisfied
     *
     */
    bool setCurrentSpeedup(double current_speedup);

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
     *  REQUIRE(roadNetwork->check_position_cars(), "position");
     *  REQUIRE(roadNetwork->check_if_cars_on_existing_road(), "exist on road");
     *  REQUIRE(time >= 0, "Tijd moet positief zijn");
     *  REQUIRE(roadNetwork->findCar(license_plate) != NULL, "De wagen moet in het netwerk zitten");
     *
     * @post The vehicle has a new position.
     *  ENSURE(roadNetwork->check_position_cars(), "position");
     *  ENSURE(roadNetwork->check_if_cars_on_existing_road(), "exist on road");
     *  // Space between cars is not guaranteed, because all cars have to be moved for this.
     *
     * @return true when successfully added, false when the prerequisites weren't met
     */
    bool move(RoadNetwork *roadNetwork);

    /**
     * Get the type of the Vehicle as a string.
     *
     * @return The type of the vehicle as string
     *
     * @pre
     * REQUIRE(properlyInitialized(), "The vehicle must be properly initialized");
     */
    virtual std::string getType() const=0;

    /**
     * A function to get the unique identification string of a vehicle.
     *
     * @return The license plate of the Vehicle as string.
     *
     * @pre
     * REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
     */
    const std::string &getLicensePlate() const;

    /**
     * Get the road the Vehicle is driving on
     * @return a pointer to the road the vehicle is driving on
     *
     * @pre
     * REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
     */
    Road *getCurrentRoad() const;

    /**
     * Get the position on the road of the current Vehicle
     * @return The position of the Vehicle (in m) from the starting point of the road
     *
     * @pre
     * REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
     */
    double getCurrentPosition() const;

    /**
     * Get the speed at which the car is driving
     * @return A double containing the current speed of the car
     *
     * @pre
     * REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
     */
    double getCurrentSpeed() const;

    /**
     * The length of the vehicle in m.
     *
     * @pre
     * REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
     */
    virtual double getLength() const=0;

    virtual ~Vehicle();

    /**
     * Check whether a vehicle is properly initialised
     *
     * @return true when everything properly initialised
     */
    bool properlyInitialized() const;

    /**
     * The speedup of the vehicle in m/s^2.
     *
     * @pre
     * REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
     */
    double getCurrentSpeedup() const;

    /**
     * Get the minimum speed of the vehicle in m/s
     *
     * @return a double containing the minimum speed of the vehicle
     *
     * @pre
     * REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
     */
    virtual double getMinSpeed() const=0;

    /**
     * Get the maximum speed of the vehicle in m/s
     *
     * @return a double containing the maximum speed of the vehicle
     *
     * @pre
     * REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
     */
    virtual double getMaxSpeed() const=0;

    /**
     * Get the minimum speedup of the vehicle in m/s^2
     *
     * @return a double containing the minimum speedup of the vehicle
     *
     * @pre
     * REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
     */
    virtual double getMaxSpeedup() const=0;

    /**
     * Get the maximum speedup of the vehicle in m/s^2
     *
     * @return a double containing the maximum speedup of the vehicle
     *
     * @pre
     * REQUIRE(ProperlyInit(), "Het voertuig moet deftig geinitialiseerd zijn");
     */
    virtual double getMinSpeedup() const=0;


private:
    std::string licensePlate; /**< A unique ID for each vehicle */
    Road* currentRoad; /**< The road on which the vehicle is currently positioned */
    double currentPosition; /**< The position on the current road. This is the distance from the start of this road in m.
                             */
    double currentSpeed; /**< The current driving speed in km / h. A vehicle can't drive backwards in our system, so this number must
                                * be greater than or equal to 0.*/
    double currentSpeedup; /**< The speed by which the vehicle is accelerated in m / s^2. */

    Vehicle* _initCheck;

    void updateCurrentPosition(double time);

    void updateCurrentSpeed(double time);

    bool checkCurrentPositionOnRoad();

    void setCurrentPositionOnNewRoad(RoadNetwork* roadNetwork);

    void updateCurrentSpeedup(const double &time, RoadNetwork *roadNetwork);
};


#endif //ANTROPSE_VEHICLE_H
