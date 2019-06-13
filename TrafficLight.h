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

    double getPosition() const;

    bool properlyInit();
private:
    /**< The position on the road of where the trafficLight is */
    double startOffset;
    double position;
    TrafficLight* _initCheck;
};



#endif //ANTROPSE_TRAFFICLIGHT_H
