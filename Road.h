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
#include <map>
#include <set>
#include "TrafficLight.h"


/**
 * A class used to describe a Road in a RoadNetwork
 */
class Road {

public:

    /**
     * Get the name of a road
     *
     * @return The unique identification name of this road
     *
     * @pre
     * REQUIRE(ProperlyInit(), "De weg moet deftig geinitialiseerd zijn");
     */
    const std::string &getName() const;

    /**
     * Get the speed limit of a road
     *
     * @return The maximum legal speed to drive on this road in km/h
     *
     * @pre
     * REQUIRE(ProperlyInit(), "De weg moet deftig geinitialiseerd zijn");
     */
    int getSpeedLimit(double position) const;

    /**
     * Get the length of the road
     *
     * @return A double containing the length of the road in m
     *
     * @pre
     * REQUIRE(ProperlyInit(), "De weg moet deftig geinitialiseerd zijn");
     */
    double getLength() const;

    /**
     * empty constructor of road
     *
     * @post
     * ENSURE(properlyInit(), "The road is properly initialized");
     */
    Road();

    Road(const std::string &name, int speedLimit, double length, Road *intersection);

    /**
     * Change the name of a road
     *
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
     *
     * @param newSpeedLimit The maximum allowed speed to drive on the road in km/h
     *
     * @pre
     * REQUIRE(speed_limit > 0, "De speedlimit moet strikt groter zijn dan 0"); // Strikt groter
     *
     * @post
     * ENSURE(getSpeedLimit() == newSpeedLimit, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
     *
     * @return true when all preconditions are satisfied
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
     * Get the road which follows on this road
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
     *
     * @return true when properly initialised
     */
    bool properlyInit() const;

    /**
     * Add a bus stop to the road
     *
     * @param position The position on the road to add the bus stop
     *
     * @return true when succesfully added, false when there was an error adding the stop
     *
     * @pre
     * REQUIRE(properlyInit(), "The road must be correctly initialized");
     * REQUIRE(validPositionOnRoad(position), "The position must be on the road");
     * REQUIRE(numberInSet(position, busStops), "There cant be another bus stop at the same postition")
     *
     * @post
     * ENSURE(numberInSet(position, busStops), "The bus stop has been placed at the specified position")
     */
    bool addBusStop(int position);

    /**
     * Add a zone to the road
     *
     * @param position The position on the road where the zone starts
     * @param speedLimit The speed limit in the zone
     *
     * @return true when succesfully added, false when there was an error adding the zone
     *
     * @pre
     * REQUIRE(properlyInit(), "The road must be correctly initialized");
     * REQUIRE(validPositionOnRoad(position), "The position must be on the road");
     * REQUIRE(zoneSpeedLimit > 0, "The position must be strict positive");
     * REQUIRE(numberInSet(position, trafficLights), "There cant be another zone start at the same postition");
     *
     * @post
     * ENSURE(for(i=0;i<nextZone()): getSpeedLimit(i)==zoneSpeedLimit, "The bus stop has been placed at the specified position")
     */
    bool addZone(double position, int zoneSpeedLimit);

    /**
     * Add a traffic light to the road with a random startOffset
     *
     * @param position The position on the road to add the light
     *
     * @return true when succesfully added, false when there was an error adding the stop
     *
     * @pre
     * REQUIRE(properlyInit(), "The road must be correctly initialized");
     * REQUIRE(validPositionOnRoad(position), "The position must be on the road");
     * REQUIRE(numberInSet(position, trafficLights), "There cant be another traffic light at the same postition")
     *
     * @post
     * ENSURE(numberInSet(position, trafficLights), "The traffic light stop has been placed at the specified position")
     */
    bool addTrafficLight(double position);

    /**
      * Add a traffic light to the road with a chosen startOffset
      *
      * @param position The position on the road to add the light
      *
      * @return true when succesfully added, false when there was an error adding the stop
      *
      * @pre
      * REQUIRE(properlyInit(), "The road must be correctly initialized");
      * REQUIRE(validPositionOnRoad(position), "The position must be on the road");
      * REQUIRE(numberInSet(position, trafficLights), "There cant be another traffic light at the same postition")
      *
      * @post
      * ENSURE(numberInSet(position, trafficLights), "The traffic light stop has been placed at the specified position")
      */
    bool addTrafficLight(double position, int time);


    double getNextTrafficLight(double position);
    /**
     *
     * @param position = the exact position of the trafficlight
     * @return a pointer of the Trafficlight on that position
     */
    TrafficLight* getTrafficLight(double position);

    /**
     * Find the next busStop
     *
     * @return the position of the next bus stop on the road or -1 when the last bus stop on the on the road has been passed;
     *
     * @pre
     * REQUIRE(properlyInit(), "The road must be correctly initialized")
     */
    double getNextBusStop(double busPosition);

    /**
     * @return a set containing the positions of all busstops on the road
     * @pre
     * REQUIRE(properlyInit(), de weg moet deftig geinitialiseerd zijn)
     */
    const std::set<double> &getBusStops() const;

    /**
    * @return a map containing all TrafficLights on the road
    * @pre
    * REQUIRE(properlyInit(), de weg moet deftig geinitialiseerd zijn)
    */
    const std::map<double, TrafficLight *> &getTrafficLights() const;

    /**
    * @return a map containing the positions and speed limits of all zones on the road
    * @pre
    * REQUIRE(properlyInit(), de weg moet deftig geinitialiseerd zijn)
    */
    const std::map<double, int> &getZones() const;

    /**
     * @return true when the invalid data of the empty constructor is updated
     * @pre
     * REQUIRE(properlyInit(), de weg moet deftig geinitialiseerd zijn)
     */
    bool emptyConstructorParametersUpdated();

private:

    Road* _initCheck;
    std::string name; /**< A unique name for identification of the road */
    int speedLimit; /**< The maximum legal speed to drive on a road in km / h */
    double length; /**< The length of a road in m */
    Road* intersection; /**< Contains the connection: The connecting street is the intersection on position
                                   * Road.length */
    std::set<double> busStops; // I'm using a set instead of a vector to keep it all sorted
    std::map<double, TrafficLight*> trafficLights;
    std::map<double, int> zones; // first number is the position, second number the speed_limit
};


#endif //ANTROPSE_ROAD_H
