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

#include "Intersection.h"
#include <string>

/**
 * A class used to describe a Road in a RoadNetwork
 */
class Road {
private:
    std::string name;
public:
    const std::string &getName() const;

    int getSpeed_limit() const;

    double getLength() const;

private:
    /**< A unique name for identification of the road */
    int speed_limit; /**< The maximum legal speed to drive on a road in km / h */
    double length; /**< The length of a road in m */
    Intersections intersecions; /**< Contains the connection: The connecting street is the intersection on position
                                   * Road.length */
};


#endif //ANTROPSE_ROAD_H
