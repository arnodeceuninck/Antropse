//
// Created by Sien Admin on 09/05/2019.
//

#ifndef ANTROPSE_TRAFFICLIGHT_H
#define ANTROPSE_TRAFFICLIGHT_H

#include <iostream>
#include <set>



enum TrafficLightColor{green, orange, red};

class TrafficLight {
public:
    TrafficLight(double startOffset, double position);

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
