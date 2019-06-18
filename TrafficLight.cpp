//
// Created by Sien Admin on 09/05/2019.
//

#include <cmath>
#include "TrafficLight.h"
#include "CONST.h"
#include "DesignByContract.h"

TrafficLight::TrafficLight(double position, double startOffset) : startOffset(startOffset), position(position),
                                                                  _initCheck(this) {
    REQUIRE(position >= 0, "Huidige positie moet op de weg liggen");
    REQUIRE(startOffset >= 0, "De offset mag niet kleiner zijn dan 0");
}

TrafficLightColor TrafficLight::getColor(double time) {
    REQUIRE(properlyInit(), "Het verkeerslicht moet deftig geinitialiseerd zijn");
    double cycletime = std::fmod((time + startOffset),
                                 (CONST::GREEN_DURATION + CONST::ORANGE_DURATION + CONST::RED_DURATION));
    if (cycletime < CONST::GREEN_DURATION) {
        return green;
    } else if (cycletime < CONST::ORANGE_DURATION + CONST::GREEN_DURATION) {
        return orange;
    } else {
        return red;
    }
}

double TrafficLight::getPosition() const {
    return position;
}

bool TrafficLight::properlyInit() {
    return _initCheck == this;
}

char TrafficLight::getColorChar(double time) {
    REQUIRE(properlyInit(), "Het verkeerslicht moet deftig geinitialiseerd zijn");
    TrafficLightColor trafficLightColor = getColor(time);
    if (trafficLightColor == red) {
        return '|';
    } else if (trafficLightColor == orange) {
        return 'o';
    } else {
        return '-';
    }
}
