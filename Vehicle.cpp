/**
 * @file Vehicle.cpp
 * @brief This file will contain the definitions of the functions in Vehicle.
 *
 * @author Arno Deceuninck
 *
 * @date 01/03/2019
 */

#include <iostream>
#include <limits>
#include "Vehicle.h"
#include "Convert.h"
#include "CONST.h"
#include "Road.h"
#include "RoadNetwork.h"
#include "DesignByContract.h"

Vehicle::Vehicle(const std::string &license_plate, Road *current_road, double current_position, double current_speed)
        : licensePlate(license_plate), currentRoad(current_road),
          currentPosition(current_position), currentSpeed(current_speed), currentSpeedup(0),
          slowingDownForPreviousCar(false), slowingDownForTrafficLight(false), slowingDownForVehicleSpecific(false) {
    _initCheck = this;
    ENSURE(properlyInitialized(), "The vehicle has been properly initialized");
}


const std::string &Vehicle::getLicensePlate() const {
    REQUIRE(properlyInitialized(), "Het voertuig moet deftig geinitialiseerd zijn");
    return licensePlate;
}

Road *Vehicle::getCurrentRoad() const {
    REQUIRE(properlyInitialized(), "Het voertuig moet deftig geinitialiseerd zijn");
    return currentRoad;
}

double Vehicle::getCurrentPosition() const {
    REQUIRE(properlyInitialized(), "Het voertuig moet deftig geinitialiseerd zijn");
    return currentPosition;
}

double Vehicle::getCurrentSpeed() const {
    REQUIRE(properlyInitialized(), "Het voertuig moet deftig geinitialiseerd zijn");
    return currentSpeed;
}

bool Vehicle::setLicensePlate(const std::string &newLicensePlate) {
    if (!(newLicensePlate.size() > 0)) {
        return false;
    }
    REQUIRE(newLicensePlate.size() > 0, "Je nummerplaat kan niet leeg zijn");
    Vehicle::licensePlate = newLicensePlate;
    ENSURE(getLicensePlate() == licensePlate, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
    return true;
}

bool Vehicle::setCurrentRoad(Road *newCurrentRoad) {
    if (newCurrentRoad == NULL or currentSpeed > newCurrentRoad->getSpeedLimit(currentPosition)) {
        return false;
    }
    REQUIRE(currentSpeed <= newCurrentRoad->getSpeedLimit(currentPosition),
            "De huidige nelheid van je voertuig moet kleiner zijn dan de max toegelaten snelheid op de weg");
    Vehicle::currentRoad = newCurrentRoad;
    ENSURE(getCurrentRoad() == newCurrentRoad, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
    return true;
}

bool Vehicle::setCurrentPosition(int newCurrentPosition) {
    if (!(newCurrentPosition >= 0 and
          ((currentRoad != NULL and newCurrentPosition <= currentRoad->getLength()) or
           currentRoad == NULL))) {
        return false;
    }
    REQUIRE(newCurrentPosition >= 0, "De positie moet positief zijn");
    REQUIRE((currentRoad != NULL and newCurrentPosition <= currentRoad->getLength()) or
            currentRoad == NULL, "De positie valt buiten de weg");

    Vehicle::currentPosition = newCurrentPosition;
    ENSURE(getCurrentPosition() == newCurrentPosition, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
    return true;
}

bool Vehicle::setCurrentSpeed(double newCurrentSpeed) {

    if (!(newCurrentSpeed <= getMaxSpeed() and
          ((currentRoad != NULL and newCurrentSpeed <= currentRoad->getSpeedLimit(currentPosition)) or
           currentRoad == NULL) and newCurrentSpeed >= getMinSpeed() and newCurrentSpeed == (int) newCurrentSpeed)) {
        return false;
    }
    REQUIRE(newCurrentSpeed <= getMaxSpeed(), "Maximumsnelheid voor wagen overschreven");
    REQUIRE((currentRoad != NULL and newCurrentSpeed <= currentRoad->getSpeedLimit(currentPosition)) or
             currentRoad == NULL, "Te snel rijden is verboden");

    Vehicle::currentSpeed = newCurrentSpeed;
    ENSURE(getCurrentSpeed() == currentSpeed, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
    return true;
}

//bool Vehicle::setCurrentSpeedup(double newCurrentSpeedup) {
//    if (currentSpeedup < getMinSpeedup() or currentSpeedup > getMaxSpeedup()) {
//        return false;
//    }
//    REQUIRE(currentSpeedup >= getMinSpeedup(), "Versnelling te traag");
//    REQUIRE(currentSpeedup <= getMaxSpeedup(), "Versnelling te hoog");
//    Vehicle::currentSpeedup = newCurrentSpeedup;
//    ENSURE(getCurrentSpeedup() == newCurrentSpeedup, "Als je de waarde opvraagt, krijg je de nieuwe waarde");
//    return true;
//}


Vehicle::Vehicle() : licensePlate(""), currentRoad(NULL),
                     currentPosition(0), currentSpeed(0),
                     currentSpeedup(0), slowingDownForPreviousCar(false), slowingDownForTrafficLight(false),
                     slowingDownForVehicleSpecific(false) {
    _initCheck = this;
    ENSURE(properlyInitialized(), "The vehicle has been properly initialized");
}

bool Vehicle::move(RoadNetwork *roadNetwork, std::ostream &errStream) {
    REQUIRE(roadNetwork->checkPositionCars(), "position");
    REQUIRE(roadNetwork->checkIfCarsOnExistingRoad(), "exist on road");
    REQUIRE(roadNetwork->findCar(licensePlate) != NULL, "De wagen moet in het netwerk zitten");

    double time = CONST::SECONDS_PER_ITERATION;

    updateCurrentPosition(time);
    updateCurrentSpeed(time);
    updateCurrentSpeedup(time, roadNetwork);

    while (!checkCurrentPositionOnRoad()) {
        setCurrentPositionOnNewRoad(roadNetwork);
    }

    if (currentRoad != NULL) {
        checkForTrafficLight(roadNetwork, errStream);
        checkVehicleSpecificMove(roadNetwork, errStream);
    }


    slowingDownForPreviousCar = false;
    slowingDownForTrafficLight = false;
    slowingDownForVehicleSpecific = false;

//    std::cout << "Car " << licensePlate << " " << currentPosition << std::endl;
    ENSURE(roadNetwork->checkPositionCars(), "position");
    ENSURE(roadNetwork->checkIfCarsOnExistingRoad(), "exist on road");
    return true;
}

//Vehicle::Vehicle(const Vehicle *vehicle) : licensePlate(vehicle->getLicensePlate()),
//                                           currentRoad(vehicle->getCurrentRoad()),
//                                           currentPosition(vehicle->getCurrentPosition()),
//                                           currentSpeed(vehicle->getCurrentSpeed()),
//                                           currentSpeedup(vehicle->getCurrentSpeedup()) {
//    _initCheck = this;
//    ENSURE(properlyInitialized(), "The vehicle has been properly initialized");
//}


double Vehicle::getCurrentSpeedup() const {
    REQUIRE(properlyInitialized(), "Het voertuig moet deftig geinitialiseerd zijn");
    return currentSpeedup;
}

Vehicle::~Vehicle() {

}

bool Vehicle::properlyInitialized() const {
    return Vehicle::_initCheck == this;
}

void Vehicle::updateCurrentPosition(double time) {
    // Bereken nieuwe positie van voertuig
    currentPosition = Convert::kmhToMs(currentSpeed) * time + currentPosition;

}

void Vehicle::updateCurrentSpeed(double time) {
    // Bereken nieuwe snelheid van voertuig
    double currentSpeedMS = currentSpeedup * time + Convert::kmhToMs(currentSpeed);
    double newCurrentSpeed = Convert::msToKmh(currentSpeedMS);

    // TODO: Hiermee rekening houden bij het berekenen van de versnelling:
    //       Ofwel gaan versnellen naar de max road speed
    //       Ofwel naar de max vehicle speed
    //       Ofwel naar de plaats om te stoppen
    if (newCurrentSpeed > getMaxSpeed()) {
        newCurrentSpeed = getMaxSpeed();
    }

    // TODO: Check of je dit mag laten vallen; normaal gezien moet update speedup zorgen voor een vertraging als je erover zit
//    if(currentSpeed > currentRoad->getSpeedLimit(currentPosition)){
//        currentSpeed = currentRoad->getSpeedLimit(currentPosition);
//    }

    // Auto's kunnen niet achteruit rijden in de simulatie
    if (newCurrentSpeed < getMinSpeed()) {
        newCurrentSpeed = getMinSpeed();
    }

    currentSpeed = newCurrentSpeed;
}

bool Vehicle::checkCurrentPositionOnRoad() {
    return !(currentRoad != NULL && currentPosition > currentRoad->getLength());
}

void Vehicle::setCurrentPositionOnNewRoad(RoadNetwork *roadNetwork) {
    REQUIRE(!checkCurrentPositionOnRoad(), "De wagen moet buiten de weg vallen");
    currentPosition = currentPosition - currentRoad->getLength();
    if(currentRoad->getIntersection() != NULL) {
        // IF huidige baan heeft verbinding
        // Zet voertuig op verbindingsbaan
        currentRoad = currentRoad->getIntersection();
    } else {
        // ELSE
        // Verwijder voertuig uit simulatie
        roadNetwork->removeVehicle(licensePlate);
        removeCurrentRoad();
    }
}

void Vehicle::updateCurrentSpeedup(const double &time, RoadNetwork *roadNetwork) {
    REQUIRE(roadNetwork->properlyInitialized(), "roadNetwork moet correct geinitialiseerd zijn");

    // Bereken nieuwe versnelling van voertuig;
    Vehicle *previousCar = roadNetwork->findPreviouscar(this);

    if (previousCar != NULL) {

        double idealFollowingDistance = getIdealDistance(roadNetwork);
        double actualFollowingDistance =
                previousCar->getCurrentPosition() - previousCar->getLength() - currentPosition;


        double newSpeedup = (actualFollowingDistance - idealFollowingDistance) / 2;

        if (slowingDownForTrafficLight or slowingDownForVehicleSpecific) {
            currentSpeedup = currentSpeedup < newSpeedup ? currentSpeedup : newSpeedup;
        } else {
            currentSpeedup = newSpeedup;
            slowingDownForPreviousCar = true;
        }


//        std::cout << "Following " << idealFollowingDistance << " " << actualFollowingDistance << currentSpeedup << std::endl;

    } else {

        slowingDownForPreviousCar = false;

        currentSpeedup = -(Convert::kmhToMs(currentSpeed) -
                           Convert::kmhToMs(currentRoad->getSpeedLimit(currentPosition)));
//        if(currentSpeed < currentRoad->getSpeedLimit(currentPosition)){
//            currentSpeedup = getMaxSpeedup();
//        } else {
//            currentSpeedup = 0;
//        }
    }

    if (currentSpeedup > getMaxSpeedup()) {
        currentSpeedup = getMaxSpeedup();
    }
}

double Vehicle::getIdealDistance(RoadNetwork *roadNetwork) const {
    Vehicle *previousCar = roadNetwork->findPreviouscar(this);
    if (previousCar == NULL) {
        // TODO: controleren wat te returnen als er geen vorig voertuig is
        return std::numeric_limits<double>::max();
    }
    return (3 * currentSpeed) / 4 + previousCar->getLength() + CONST::MIN_FOLLOWING_DISTANCE;
}

// TODO: slowing down for traffic light
void Vehicle::checkForTrafficLight(RoadNetwork *roadNetwork, std::ostream &errStream) {

    if(currentRoad == NULL){
        return;
    }

    double positionNextTrafficLight = currentRoad->getNextTrafficLight(currentPosition);

    if (positionNextTrafficLight == -1) {
        return;
    }

    if (positionNextTrafficLight - currentPosition < 2 * getIdealDistance(roadNetwork)) {
        TrafficLightColor trafficLightColor = currentRoad->getTrafficLight(positionNextTrafficLight)->getColor(
                roadNetwork->getIteration());

        if (trafficLightColor == red or trafficLightColor == orange) {

            if (currentPosition == positionNextTrafficLight and currentSpeed > 0 and trafficLightColor == red) {
                errStream << "woopsiepoopsie door rood licht gereden, let's pretend I didn't see that ;)" << std::endl;
//                move(roadNetwork);
            }
            double slowDown = calculateSlowDownForPosition(positionNextTrafficLight);
            if(slowDown < currentSpeedup){
                currentSpeedup = slowDown;
            }
            currentSpeedup = calculateSlowDownForPosition(positionNextTrafficLight);

            if (currentSpeedup < getMinSpeedup()) {
                errStream << "Impossible to stop before the traffic light" << std::endl;
                updateCurrentSpeedup(1, roadNetwork);
            }

        }
    }
}

double Vehicle::calculateSlowDownForPosition(double stopPosition) {
    double deltaP = stopPosition - currentPosition; // - Convert::kmhToMs(currentSpeed);
    if (deltaP == 0) {
        return 0;
    }

    // TODO: check of deze random gekozen waarden goed zijn
    if (currentSpeed == 0 or (deltaP > -(getMinSpeedup()*2) and currentSpeed < -(getMinSpeedup()))) {
        return deltaP / 4; // Een beetje versnellen
    }

//    std::cout << "Huidige snelheid (m/s): " << Convert::kmhToMs(currentSpeed) << std::endl;
//    std::cout << "Positie van stilstand: " << stopPosition << std::endl;
//    std::cout << "Huidige positie: " << currentPosition << std::endl;
//    std::cout << "Aantal m voor stilstand: " << deltaP << std::endl;
    double speedMS = Convert::kmhToMs(currentSpeed);
    return -(speedMS * speedMS) / deltaP;
}

void Vehicle::checkVehicleSpecificMove(RoadNetwork *roadNetwork, std::ostream &errStream) {}

//void Vehicle::disableSpeedupUpdates() {
//    speedupUpdates = false;
//}

//void Vehicle::enableSpeedupUpdates() {
//    speedupUpdates = true;
//}

//bool Vehicle::speedupUpdateEnabled() const{
//    return speedupUpdates;
//}

void Vehicle::removeCurrentRoad() {
    currentRoad = NULL;
}
