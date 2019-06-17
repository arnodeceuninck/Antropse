//
// Created by Sien Admin on 09/05/2019.
//

#include <cmath>
#include "TrafficLight.h"
#include "CONST.h"

TrafficLight::TrafficLight(double startOffset, double position) : startOffset(startOffset), position(position) {}

TrafficLightColor TrafficLight::getColor(double time) {
    double cycletime = std::fmod((time + startOffset),(CONST::GREEN_DURATION + CONST::ORANGE_DURATION + CONST::RED_DURATION));
    if(cycletime < CONST::GREEN_DURATION){
        std::cout << "GREEN" << std::endl;
        return green;
    } else if(cycletime < CONST::ORANGE_DURATION + CONST::GREEN_DURATION){
        std::cout << "ORANGE" << std::endl;
        return orange;
    } else{
        std::cout << "RED" << std::endl;
        return red;
    }
}

double TrafficLight::getPosition() const {
    return position;
}

bool nextTrafficLight(const TrafficLight* t1, const TrafficLight* t2) {
    return t1->getPosition() < t2->getPosition();
}
