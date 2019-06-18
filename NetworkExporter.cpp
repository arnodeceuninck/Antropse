//
// Created by arno on 4/13/19.
//

#include <string>
#include <ostream>
#include "NetworkExporter.h"
#include "DesignByContract.h"
#include "Road.h"
#include "Vehicle.h"
#include "AntropseUtils.h"

NetworkExporter::NetworkExporter() {
    _initCheck = this;
    _documentStarted = false;
    ENSURE(properlyInitialized(),
           "constructor must end in properlyInitialized state");
}

bool NetworkExporter::properlyInitialized() {
    return _initCheck == this;
}

bool NetworkExporter::documentStarted() {
    return _documentStarted;
}

void NetworkExporter::documentStart(std::ostream &onStream) {
    REQUIRE(properlyInitialized(), "The exporter must be properly initialized");
    _documentStarted = true;
    ENSURE(documentStarted(), "The document is started");
}

void NetworkExporter::documentEnd(std::ostream &onStream) {
    REQUIRE(documentStarted(), "The document must be started");
    _documentStarted = false;
    ENSURE(!documentStarted(), "The document is closed");
}

void NetworkExporter::exportOn(std::ostream &onStream, RoadNetwork &game) {
    REQUIRE(this->properlyInitialized(), "NetworkExporter wasn't initialized when calling exportOn.");
    REQUIRE(game.properlyInitialized(), "RoadNetwork wasn't initialized when calling exportOn");
    REQUIRE(this->documentStarted(), "RoadNetwork Exporter wasn't in documentStarted when calling exportOn.");

    this->sectionStart(onStream, "Roadnetwork " + toString(game.getIteration()));

    this->roadNetworkStart(onStream);

    this->roadsStart(onStream);
    for (int road = 0; road < game.nrOfRoads(); ++road) {
        this->roadAdd(onStream, game.getRoads()[road], game);
    }
    this->roadsEnd(onStream);

    this->vehiclesStart(onStream);
    for (int vehicle = 0; vehicle < game.nrOfCars(); ++vehicle) {
        this->vehicleAdd(onStream, game.getCars()[vehicle]);
    }
    this->vehiclesEnd(onStream);

    this->roadNetworkEnd(onStream);
    this->sectionEnd(onStream);
}

void NetworkExporter::sectionStart(std::ostream &onStream, const std::string sectionTitle) {
    onStream << sectionTitle << std::endl << std::endl;
}

void NetworkExporter::roadNetworkStart(std::ostream &onStream) {

}

void NetworkExporter::roadsStart(std::ostream &onStream) {

}

void NetworkExporter::roadAdd(std::ostream &onStream, const Road *road, RoadNetwork &roadNetwork) {

    std::string listItem = "\t-> ";

    onStream << "Baan: " << road->getName() << std::endl;

    onStream << listItem << "snelheidslimiet vanaf positie 0: " << road->getSpeedLimit(0) << std::endl;

    const std::map<double, int> zones = road->getZones();
    for (std::map<double, int>::const_iterator zone = zones.begin(); zone != zones.end(); ++zone) {
        onStream << listItem << "snelheidslimiet vanaf positie " << (*zone).first << ": " << (*zone).second << std::endl;
    }

    //TODO: zones
    onStream << listItem << "lengte: " << road->getLength() << std::endl;
    const std::set<double> busStops = road->getBusStops();
    for (std::set<double>::const_iterator busStop = busStops.begin(); busStop != busStops.end(); ++busStop) {
        onStream << listItem << "bushalte op positie " << *busStop << std::endl;
    }

    const std::map<double, TrafficLight *> trafficLights = road->getTrafficLights();
    for (std::map<double, TrafficLight *>::const_iterator trafficLight = trafficLights.begin();
         trafficLight != trafficLights.end(); ++trafficLight) {
        onStream << listItem << "Verkeerslicht op positie " << (*trafficLight).first << ": ";
        TrafficLightColor color = (*trafficLight).second->getColor(roadNetwork.getIteration());
        if(color == green){
            onStream << "Groen";
        } else if (color == orange){
            onStream << "Oranje";
        } else if (color == red){
            onStream << "Rood";
        }
        onStream << std::endl;
    }

    onStream << std::endl;

}

void NetworkExporter::roadsEnd(std::ostream &onStream) {
//    onStream << std::endl;
}

void NetworkExporter::vehiclesStart(std::ostream &onStream) {

}

void NetworkExporter::vehicleAdd(std::ostream &onStream, const Vehicle* vehicle) {

    std::string listItem = "\t-> ";

    onStream << "Voertuig: " << vehicle->getType() << " (" << vehicle->getLicensePlate() << ")" << std::endl;

    onStream << listItem << "baan: " << vehicle->getCurrentRoad()->getName() << std::endl;
    onStream << listItem << "positie: " << vehicle->getCurrentPosition() << std::endl;
    onStream << listItem << "snelheid: " << vehicle->getCurrentSpeed() << std::endl;
//    onStream << listItem << "versnelling: " << vehicle->getCurrentSpeedup() << std::endl;

    onStream << std::endl;

}

void NetworkExporter::vehiclesEnd(std::ostream &onStream) {
//    onStream << std::endl;
}

void NetworkExporter::sectionEnd(std::ostream &onStream) {

}

void NetworkExporter::roadNetworkEnd(std::ostream &onStream) {
    onStream << "-----------------------------------------------------------" << std::endl;
}
