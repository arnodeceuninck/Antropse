//
// Created by Sien Admin on 16/05/2019.
//

#include <string>
#include "Road.h"
#include "Vehicle.h"
#include "GraphicImpressionExporter.h"

#define SCALE_LENGTH 100

void GraphicImpressionExporter::sectionStart(std::ostream &onStream, const std::string sectionTitle) {
    NetworkExporter::sectionStart(onStream, sectionTitle);
}

void GraphicImpressionExporter::roadNetworkStart(std::ostream &onStream) {
    NetworkExporter::roadNetworkStart(onStream);
}

void GraphicImpressionExporter::roadsStart(std::ostream &onStream) {
    NetworkExporter::roadsStart(onStream);
}

void GraphicImpressionExporter::roadAdd(std::ostream &onStream, const Road *road, RoadNetwork &roadNetwork) {
    onStream << road->getName() << " \t | \t";

    int roadLenght = road->getLength() / SCALE_LENGTH;
    std::string roadString;
    for (int i = 0; i < roadLenght; ++i) {
        roadString += "=";
    }

    const std::set<double> busStops = road->getBusStops();
    for (std::set<double>::const_iterator busStop = busStops.begin(); busStop != busStops.end(); ++busStop) {
        int busStopPosition = (*busStop) / SCALE_LENGTH;
        roadString[busStopPosition] = 'P';
    }

    const std::map<double, TrafficLight *> trafficLights = road->getTrafficLights();
    for (std::map<double, TrafficLight *>::const_iterator trafficLight = trafficLights.begin();
         trafficLight != trafficLights.end(); ++trafficLight) {
        int trafficLightPosition = (*trafficLight).first / SCALE_LENGTH;
        char colorTL = (*trafficLight).second->getColorChar(roadNetwork.getIteration());
        roadString[trafficLightPosition] = colorTL;
    }

    for (std::vector<Vehicle *>::const_iterator vehicle = roadNetwork.getCars().begin();
         vehicle != roadNetwork.getCars().end();
         ++vehicle) {

        if ((*vehicle)->getCurrentRoad() != road) {
            continue;
        }
        int vehiclePosition = (*vehicle)->getCurrentPosition() / SCALE_LENGTH;
        roadString[vehiclePosition] = (*vehicle)->getShortName();
    }

    onStream << roadString << std::endl;
}

void GraphicImpressionExporter::roadsEnd(std::ostream &onStream) {
    onStream << std::endl;
}

void GraphicImpressionExporter::vehiclesStart(std::ostream &onStream) {}

void GraphicImpressionExporter::vehicleAdd(std::ostream &onStream, const Vehicle *vehicle) {}

void GraphicImpressionExporter::vehiclesEnd(std::ostream &onStream) {}

void GraphicImpressionExporter::sectionEnd(std::ostream &onStream) {
    NetworkExporter::sectionEnd(onStream);
}

void GraphicImpressionExporter::roadNetworkEnd(std::ostream &onStream) {
    NetworkExporter::roadNetworkEnd(onStream);
}
