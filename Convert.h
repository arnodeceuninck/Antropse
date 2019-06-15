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

namespace Convert {
    /**
     * Convert a speed in kilometers per hour to meters per second
     *
     * @param kmh The speed in kilometers per hour
     * @return The speed in meters per second
     */
    double kmhToMs(double kmh);

    /**
     * Convert a speed in meters per second to kilometers per hour
     *
     * @param mps The speed in meters per second
     * @return The speed in kilometers per hour
     */
    double msToKmh(double mps);
}

#endif //ANTROPSE_CONVERT_H
