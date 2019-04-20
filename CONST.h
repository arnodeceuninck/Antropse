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

    // AUTO
    const double MAX_CAR_SPEED = 150;
    const double MIN_CAR_SPEED = 0;

    const double MAX_CAR_SPEEDUP = 2;
    const double MIN_CAR_SPEEDUP = -8;

    const double CAR_LENGTH = 3; /**< The length of a car. */

    // MOTORFIETS
    const double MAX_MOTOR_SPEED = 180;
    const double MIN_MOTOR_SPEED = 0;

    const double MAX_MOTOR_SPEEDUP = 4;
    const double MIN_MOTOR_SPEEDUP = -10;

    const double MOTOR_LENGTH = 1;

    // BUS
    const double MAX_BUS_SPEED = 70;
    const double MIN_BUS_SPEED = 0;

    const double MAX_BUS_SPEEDUP = 1;
    const double MIN_BUS_SPEEDUP = -7;

    const double BUS_LENGTH = 15;

    // VRACHTWAGEN
    const double MAX_TRUCK_SPEED = 90;
    const double MIN_TRUCK_SPEED = 0;

    const double MAX_TRUCK_SPEEDUP = 1;
    const double MIN_TRUCK_SPEEDUP = -6;

    const double TRUCK_LENGTH = 15;


    // Other specifications
    const double MIN_FOLLOWING_DISTANCE = 2;

}
#endif //ANTROPSE_CONSTVARIABLES_H
