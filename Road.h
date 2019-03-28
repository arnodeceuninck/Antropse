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

/**
 * A class used to describe a Road in a RoadNetwork
 */
class Road {

public:

    /**
     * Get the name of a road
     * @return The unique identification name of this road
     */
    const std::string &getName() const;

    /**
     * Get the speed limit of a road
     * @return The maximum legal speed to drive on this road in km/h
     */
    int getSpeed_limit() const;

    /**
     * Get the length of the road
     * @return A double containing the length of the road in m
     */
    double getLength() const;

    /**
     * empty constructor of road
     */
    Road();

    /**
     * Change the name of a road
     * @param name The unique name for identifying the road
     *
     * @pre
     *  REQUIRE(name.size() > 0, "De naam mag niet leeg zijn");
     */
    void setName(const std::string &name);

    /**
     * Change the speed limit of a road
     * @param speed_limit The maximum allowed speed to drive on the road in km/h
     *
     * @pre
     *  REQUIRE(speed_limit > 0, "De speedlimit moet strikt groter zijn dan 0"); // Strikt groter
     */
    void setSpeed_limit(int speed_limit);

    /**
     * Change the length of the road
     * @param length The length of a road in m
     *
     * @pre
     *  REQUIRE(length > 0, "De lengte moet strikt groter zijn dan 0");
     */
    void setLength(double length);

    /**
     *
     * @return the intersection of the road
     */

    Road *getIntersection() const;

    void setIntersection(Road *intersection);

    /**
     * Check whether the road is properly initialised
     * @return true when properly initialised
     */
    bool ProperlyInit();


private:


    std::string name; /**< A unique name for identification of the road */
    int speed_limit; /**< The maximum legal speed to drive on a road in km / h */
    double length; /**< The length of a road in m */
    Road* intersection; /**< Contains the connection: The connecting street is the intersection on position
                                   * Road.length */

};


#endif //ANTROPSE_ROAD_H
