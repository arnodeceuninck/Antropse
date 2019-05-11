//
// Created by Sien Admin on 09/05/2019.
//

#ifndef ANTROPSE_TRAFFICLIGHT_H
#define ANTROPSE_TRAFFICLIGHT_H

#include <iostream>
#include <set>
#include "CONST.h"


enum TrafficLightColor{green, orange, red};

class TrafficLight;

bool nextTrafficLight(const TrafficLight* t1, const TrafficLight* t2);

class TrafficLight {
public:
    TrafficLight(double position, double startOffset);

    TrafficLightColor getColor(double time);


private:
    double position;
public:
    double getPosition() const;

private:
    /**< The position on the road of where the trafficLight is */
    double startOffset;

};



#endif //ANTROPSE_TRAFFICLIGHT_H
