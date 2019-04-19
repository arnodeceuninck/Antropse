/**
 * @file Convert.h
 * @brief All functions needed to convert between measurement units.
 *
 * @author Arno Deceuninck
 *
 * @date 14/03/2019
 */

#ifndef ANTROPSE_CONVERT_H
#define ANTROPSE_CONVERT_H

namespace Convert{
    /**
     * Convert a speed in kilometers per hour to meters per second
     *
     * @param kmh The speed in kilometers per hour
     * @return The speed in meters per second
     */
    double kmhToMs(double kmh);
}

#endif //ANTROPSE_CONVERT_H
