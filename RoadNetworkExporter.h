/**
 * @author Arno Deceuninck
 * @refitem Serge Demeyer
 */
//
// Created by arno on 4/13/19.
//

#ifndef ANTROPSE_ROADNETWORKEXPORTER_H
#define ANTROPSE_ROADNETWORKEXPORTER_H


#include <iosfwd>
#include "RoadNetwork.h"

class RoadNetworkExporter {
public:

    RoadNetworkExporter();

    bool properlyInitialized();
    bool documentStarted();

    virtual void  documentStart(std::ostream& onStream);

    virtual void documentEnd(std::ostream& onStream);

    void exportOn(std::ostream& onStream, RoadNetwork& game);

protected:
    virtual void sectionStart(std::ostream& onStream, const std::string sectionTitle);
    virtual void roadNetworkStart(std::ostream& onStream);
    virtual void roadsStart(std::ostream& onStream);
    virtual void roadAdd(std::ostream& onStream, const Road* road);
    virtual void roadsEnd(std::ostream& onStream);
    virtual void vehiclesStart(std::ostream& onStream);
    virtual void vehicleAdd(std::ostream& onStream, const Vehicle* vehicle);
    virtual void vehiclesEnd(std::ostream& onStream);
    virtual void sectionEnd(std::ostream& onStream);
    virtual void roadNetworkEnd(std::ostream& onStream);

private:
    RoadNetworkExporter * _initCheck; //!use pointer to myself to verify whether I am properly initialized
    bool _documentStarted;
};


#endif //ANTROPSE_ROADNETWORKEXPORTER_H
