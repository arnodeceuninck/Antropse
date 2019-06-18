//
// Created by Sien Admin on 09/05/2019.
//

#ifndef ANTROPSE_TRAFFICLIGHT_H
#define ANTROPSE_TRAFFICLIGHT_H

#include <iostream>
#include <set>


enum TrafficLightColor {
    green, orange, red
};

/**
 * A class representing a traffic light
 */
class TrafficLight {
public:
    /**
     * Constructor for a trafficlight
     *
     * @param position of the trafficlight on the road
     * @param startoffset is the time of when it should count to see what color it is next (0 is always Green)
     *
     * @pre
     *  REQUIRE(position >= 0, "Huidige positie moet op de weg liggen");
     *  REQUIRE(startoffset >= 0, "De offset mag niet kleiner zijn dan 0");
     */
    TrafficLight(double position, double startOffset);

    /**
     * Get the color of a trafficLight
     *
     * @param the time
     *
     * @return The color of the trafficlight at that specific time
     *
     * @pre
     * REQUIRE(properlyInit(), "Het verkeerslicht moet deftig geinitialiseerd zijn");
     */
    TrafficLightColor getColor(double time);

    /**
     * Get the position of a trafficLight on the road
     *
     * @return The position of the trafficlight on the road;
     *
     */
    double getPosition() const;

    bool properlyInit();

    /**
     * Get the char of the color of a trafficLight
     *
     * @param the time
     *
     * @return The char of the color of the trafficlight at that specific time
     *
     * @pre
     * REQUIRE(properlyInit(), "Het verkeerslicht moet deftig geinitialiseerd zijn");
     */
    char getColorChar(double time);

private:
    double startOffset;
    double position;     /**< The position on the road of where the trafficLight is */
    TrafficLight *_initCheck;
};


#endif //ANTROPSE_TRAFFICLIGHT_H
