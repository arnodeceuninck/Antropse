//
// Created by Sien Admin on 09/05/2019.
//

#include <cmath>
#include "TrafficLight.h"
#include "DesignByContract.h"

TrafficLight::TrafficLight(double position, double startOffset) : startOffset(startOffset), position(position), _initCheck(this) {}

TrafficLightColor TrafficLight::getColor(double time) {
    REQUIRE(properlyInit(), "Verkeerslicht moet geinitialiseerd zijn");
    double cycletime = std::fmod((time + startOffset),(CONST::GREEN_DURATION + CONST::ORANGE_DURATION + CONST::RED_DURATION));
    if(cycletime < CONST::GREEN_DURATION){
        return green;
    } else if(cycletime < CONST::ORANGE_DURATION + CONST::GREEN_DURATION){
        return orange;
    } else{
        return red;
    }
}

double TrafficLight::getPosition() const {
    return position;
}

bool nextTrafficLight(const TrafficLight* t1, const TrafficLight* t2) {
    return t1->getPosition() < t2->getPosition();
}

bool TrafficLight::properlyInit(){
    return _initCheck == this;
}
