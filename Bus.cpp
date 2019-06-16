/**
 * @file Bus.cpp
 * @brief This file will contain the definitions of the functions in Bus.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include "Bus.h"
#include "CONST.h"
#include "DesignByContract.h"
#include "Road.h"

Bus::Bus(const std::string &licensePlate, Road *currentRoad, double currentPosition, double currentSpeed) :

        Vehicle(licensePlate, currentRoad, currentPosition, currentSpeed), waitingTime(0) {

    REQUIRE(currentPosition >= 0, "Huidige positie moet op de weg liggen");
    REQUIRE(currentRoad != NULL, "De weg moet bestaan en volledig geinitialisseerd zijn");
    REQUIRE(currentPosition <= currentRoad->getLength(), "Huidige positie moet op de weg liggen");
    REQUIRE(currentSpeed <= currentRoad->getSpeedLimit(currentPosition), "De auto mag niet te snel rijden");
    REQUIRE(currentSpeed >= 0, "De auto moet een positieve snelheid hebben");
}

std::string Bus::getType() const {
    return "BUS";
}

Bus::Bus() : Vehicle() {}

double Bus::getLength() const {
    return CONST::BUS_LENGTH;
}

double Bus::getMinSpeed() const {
    return CONST::MIN_BUS_SPEED;
}

double Bus::getMaxSpeed() const {
    return CONST::MAX_BUS_SPEED;
}

double Bus::getMaxSpeedup() const {
    return CONST::MAX_BUS_SPEEDUP;
}

double Bus::getMinSpeedup() const {
    return CONST::MIN_BUS_SPEEDUP;
}

//double Bus::getWaitingTime() const {
//    return waitingTime;
//}
//
//void Bus::setWaitingTime(double newWaitingTime) {
//    Bus::waitingTime = newWaitingTime;
//}

void Bus::checkVehicleSpecificMove(RoadNetwork *roadNetwork, std::ostream &errStream) {
//    std::cout << "Waiting time (" << licensePlate << ") : " << waitingTime << std::endl;

    if(getType() == "BUS" and (currentRoad->getNextBusStop(getCurrentPosition()) - getCurrentPosition()) < 100 and currentRoad->getNextBusStop(getCurrentPosition()) -getCurrentPosition() >= 0){
        if(waitingTime < 30) {
            double newSpeedup = calculateSlowDownForPosition(currentRoad->getNextBusStop(getCurrentPosition()));
//            std::cout << "Berekende versnelling: " << newSpeedup << std::endl;

            if (slowingDownForTrafficLight or slowingDownForPreviousCar) {
                currentSpeedup = currentSpeedup < newSpeedup ? currentSpeedup : newSpeedup;
            } else {
                currentSpeedup = newSpeedup;
                slowingDownForVehicleSpecific = true;
            }

            if (currentSpeedup < getMinSpeedup()){
                errStream << "Impossible to slow down before the bus stop" << std::endl;
                updateCurrentSpeedup(1, roadNetwork);
            } else if (currentSpeedup > getMaxSpeedup()){
                currentSpeedup = getMaxSpeedup();
            }
        }
//        if(getCurrentPosition() == currentRoad->getNextBusStop(getCurrentPosition())){
        if(getCurrentPosition() + 0.001 > currentRoad ->getNextBusStop(getCurrentPosition())){
            waitingTime++;
            if(getCurrentSpeed() > 0.001){
                if(waitingTime > 30){
                    // De bushalte wordt verlaten, normaal dus dat je sneller rijdt
                    waitingTime = 0;
                    return;
                }
                errStream << "1 2 3 4 HUP NAAR ACHTER HUP NAAR VOOR, 1 2 3 4 DE BUSHALTE WE RIJDEN ERDOOR" << std::endl;
//                move(roadNetwork);
            }
            if(waitingTime > 30){
//                waitingTime = 0;
//                enableSpeedupUpdates();
                updateCurrentSpeedup(1, roadNetwork);
            }
        }
    }
}

