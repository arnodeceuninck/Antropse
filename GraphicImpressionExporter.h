//
// Created by Sien Admin on 16/05/2019.
//

#ifndef ANTROPSE_GRAPHICIMPRESSIONEXPORTER_H
#define ANTROPSE_GRAPHICIMPRESSIONEXPORTER_H


#include "NetworkExporter.h"

class GraphicImpressionExporter: public NetworkExporter {
protected:
    virtual void sectionStart(std::ostream &onStream, const std::string sectionTitle);

    virtual void roadNetworkStart(std::ostream &onStream);

    virtual void roadsStart(std::ostream &onStream);

    virtual void roadAdd(std::ostream &onStream, const Road *road, RoadNetwork &roadNetwork);

    virtual void roadsEnd(std::ostream &onStream);

    virtual void vehiclesStart(std::ostream &onStream);

    virtual void vehicleAdd(std::ostream &onStream, const Vehicle *vehicle);

    virtual void vehiclesEnd(std::ostream &onStream);

    virtual void sectionEnd(std::ostream &onStream);

    virtual void roadNetworkEnd(std::ostream &onStream);
};


#endif //ANTROPSE_GRAPHICIMPRESSIONEXPORTER_H
