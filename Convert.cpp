//
// Created by arno on 14.03.19.
//

#include "Convert.h"
#include "CONST.h"

double Convert::kmhToMs(double kmh) {
    return kmh/CONST::MS_KMH_RATIO;
}

double Convert::msToKmh(double mps) {
    return mps*CONST::MS_KMH_RATIO;
}
