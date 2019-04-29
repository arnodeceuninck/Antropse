/**
 * @file Road.h
 * @brief This header file will contain the Road class.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#ifndef ANTROPSE_ROAD_H
#define ANTROPSE_ROAD_H

#include <string>
//#include <vector>
#include <map>
#include <set>


/**
 * A class used to describe a Road in a RoadNetwork
 */
class Road {

public:

    /**
     * Get the name of a road
     * @return The unique identification name of this road
     *
     * @pre
     * REQUIRE(ProperlyInit(), "De weg moet deftig geinitialiseerd zijn");
     */
    const std::string &getName() const;

    /**
     * Get the speed limit of a road
     * @return The maximum legal speed to drive on this road in km/h
     *
     * @pre
     * REQUIRE(ProperlyInit(), "De weg moet deftig geinitialiseerd zijn");
     */
    int getSpeedLimit() const;

    /**
     * Get the length of the road
     * @return A double containing the length of the road in m
     *
     * @pre
     * REQUIRE(ProperlyInit(), "De weg moet deftig geinitialiseerd zijn");
     */
    double getLength() const;

    /**
     * empty constructor of road
     */
    Road();

    Road(const std::string &name, int speedLimit, double length, Road *intersection);

    /**
     * Change the name of a road
     * @param name The unique name for identifying the road
     *
     * @pre
     * REQUIRE(name.size() > 0, "De naam mag niet leeg zijn");
     *
     * @post
     * ENSURE(getName() == newName, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
     *
     * @return true when all precoditions are satisfied
     */
    bool setName(const std::string &name);

    /**
     * Change the speed limit of a road
     * @param newSpeedLimit The maximum allowed speed to drive on the road in km/h
     *
     * @pre
     * REQUIRE(speed_limit > 0, "De speedlimit moet strikt groter zijn dan 0"); // Strikt groter
     *
     * @post
     * ENSURE(getSpeedLimit() == newSpeedLimit, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
     *
     * @return true when all precoditions are satisfied
     */
    bool setSpeedLimit(int newSpeedLimit);

    /**
     * Change the length of the road
     * @param length The length of a road in m
     *
     * @pre
     * REQUIRE(length > 0, "De lengte moet strikt groter zijn dan 0");
     *
     * @post
     * ENSURE(getLength() == newLength, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
     *
     * @return true when all precoditions are satisfied
     */
    bool setLength(double length);

    /**
     *
     * @return the intersection of the road
     */

    Road *getIntersection() const;

    /**
     * Change the intersection of the road
     * @param intersection
     *
     * @pre
     * REQUIRE(newIntersection->properlyInit(), "De intersectie moet deftig geinitialiseerd zijn");
     *
     * @post
     * ENSURE(getIntersection() == newIntersection, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
     *
     * @return true when all precoditions are satisfied
     */
    bool setIntersection(Road *intersection);

    /**
     * Check whether the road is properly initialised
     * @return true when properly initialised
     */
    bool properlyInit() const;

    /**
     * Add a bus stop to the road
     *
     * @param position The position on the road to add the bus stop
     * @return true when succesfully added, false when there was an error adding the stop
     */
    bool addBusStop(int position);

    /**
     * Add a zone to the road
     *
     * @param position The position on the road where the zone starts
     * @param speedLimit The speed limit in the zone
     * @return true when succesfully added, false when there was an error adding the zone
     */
    bool addZone(double position, int zoneSpeedLimit);

    /**
     * Add a traffic light to the road
     *
     * @param position The position on the road to add the light
     * @return true when succesfully added, false when there was an error adding the stop
     */
    bool addTrafficLight(int position);

    /**
     * Returns the position of the next bus stop on the road or -1 when the last bus stop on the on the road has been passed
     * @return
     */
    double getNextBusStop(double busPosition);


private:

    Road* _initCheck;
    std::string name; /**< A unique name for identification of the road */
    int speedLimit; /**< The maximum legal speed to drive on a road in km / h */
    double length; /**< The length of a road in m */
    Road* intersection; /**< Contains the connection: The connecting street is the intersection on position
                                   * Road.length */
    std::set<double> busStops; // I'm using a set instead of a vector to keep it all sorted
    std::set<double> trafficLights;
    std::map<double, int> zones; // first number is the position, second number the speed_limit
};


#endif //ANTROPSE_ROAD_H
