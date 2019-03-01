/**
 * @file Intersection.cpp
 * @brief This file will contain the definitions of the functions in Intersection.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include "Intersection.h"

Intersection::Intersection(Road *exit_road, Road *main_road, double position, Side side) : exit_road(exit_road),
                           main_road(main_road), position(position), side(side) {}
