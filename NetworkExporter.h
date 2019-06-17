/**
 * @author Arno Deceuninck
 * @refitem Serge Demeyer
 */
//
// Created by arno on 4/13/19.
//

#ifndef ANTROPSE_NETWORKEXPORTER_H
#define ANTROPSE_NETWORKEXPORTER_H


#include <ostream>
#include "RoadNetwork.h"

class NetworkExporter {
public:

    /**
     * Empty constructure
     *
     * @post
     * ENSURE(properlyInitialized(), "constructor must end in properlyInitialized state");
     */
    NetworkExporter();

    /**
     * Check whether a road network is properly initialized
     *
     * @return True when correctly initialized with NetworkExporter()
     */
    bool properlyInitialized();

    /**
     * Check whether the exporter has been prepared for writing
     *
     * @return true when everything is ready for exporting
     */
    bool documentStarted();

    /**
     * Initialize the exporter for writing
     *
     * @param onStream The stream to write the export result to
     *
     * @pre
     * REQUIRE(properlyInitialized(), "The exporter must be properly initialized");
     *
     * @post
     * ENSURE(documentStarted(), "The document is started");
     */
    virtual void  documentStart(std::ostream& onStream);

    /**
     * Close the exporter
     *
     * @param onStream The stream to write the export result to
     *
     * @pre
     * REQUIRE(documentStarted(), "The document must be started");
     *
     * @post
     * ENSURE(!documentStarted(), "The document is closed");
     */
    virtual void documentEnd(std::ostream& onStream);

    /**
     * Export a roadnetwork
     *
     * @param onStream The stream on which to export the roadnetwork
     * @param game The roadnetwork you want to export
     *
     * @pre
     * REQUIRE(this->properlyInitialized(), "NetworkExporter wasn't initialized when calling exportOn.");
     * REQUIRE(game.properlyInitialized(), "RoadNetwork wasn't initialized when calling exportOn");
     * REQUIRE(this->documentStarted(), "RoadNetwork Exporter wasn't in documentStarted when calling exportOn.");
     */
    void exportOn(std::ostream& onStream, RoadNetwork& game);

protected:
    virtual void sectionStart(std::ostream& onStream, const std::string sectionTitle);
    virtual void roadNetworkStart(std::ostream& onStream);
    virtual void roadsStart(std::ostream& onStream);
    virtual void roadAdd(std::ostream &onStream, const Road *road, RoadNetwork &roadNetwork);
    virtual void roadsEnd(std::ostream& onStream);
    virtual void vehiclesStart(std::ostream& onStream);
    virtual void vehicleAdd(std::ostream& onStream, const Vehicle* vehicle);
    virtual void vehiclesEnd(std::ostream& onStream);
    virtual void sectionEnd(std::ostream& onStream);
    virtual void roadNetworkEnd(std::ostream& onStream);

private:
    NetworkExporter * _initCheck; //!use pointer to myself to verify whether I am properly initialized
    bool _documentStarted;
};


#endif //ANTROPSE_NETWORKEXPORTER_H
