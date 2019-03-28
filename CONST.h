/**
 * @file CONST.h
 * @brief This header file will contain all const variables.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#ifndef ANTROPSE_CONSTVARIABLES_H
#define ANTROPSE_CONSTVARIABLES_H

/**
 * Constant variables using in the program
 *
 * This namespace contains a collection of all constant variables in the system. The only exception are the numbers 0
 * and 1. Any other number is forbidden to be hardcoded, except in this file.
 */
namespace CONST{

    // VEHICLE SPECIFICATIONS
    const double CAR_LENGTH = 3; /**< The length of a car. */

    const double MAX_CAR_SPEED = 150;
    const double MIN_CAR_SPEED = 0;

    const double MAX_CAR_SPEEDUP = 2;
    const double MIN_CAR_SPEEDUP = -8;

    const double MIN_FOLLOWING_DISTANCE = 2;

}
#endif //ANTROPSE_CONSTVARIABLES_H
