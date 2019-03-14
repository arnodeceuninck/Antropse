/**
 * @file Intersection.h
 * @brief This header file will contain the Intersection class.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#ifndef ANTROPSE_INTERSECTION_H
#define ANTROPSE_INTERSECTION_H

#include <vector>

class Road;

class Intersection;
typedef std::vector<Intersection*> Intersections;

/**
 * An enum used to determine on which side of the main road the exit road is.
 */
enum  Side {left, /**< The exit_road is on the left side of the main road */
            right, /**< The exit_road is on the right side of the main road */
            cross}; /**< The exit_road crosses the main road */

/**
 * A class used to describe an intersection between main_road and exit_road
 */
class Intersection {

public:
    /**
     * Constructor for an intersection
     *
     * @param exit_road The road on which you'll be when leaving the main_road
     * @param main_road The road on which you're driving before coming at an intersection
     * @param position The position from the starting point of the main_road in m.
     * @param side The side of the road on which you can find the intersection.
     */
    Intersection(Road *exit_road, Road *main_road, double position, Side side);

    Road *getExit_road() const;

private:
    Road* exit_road; /**< The road on which you'll be once you've exited the main road through this intersection.*/
    Road* main_road; /**< The road on which you're driving before taking the exit */
    double position; /**< The position on the road from the starting point of the main road on which you can find this
                          intersection. */
    Side side; /**< The side on the road on which the exit road is located. */

};


#endif //ANTROPSE_INTERSECTION_H
