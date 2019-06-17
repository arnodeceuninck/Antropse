cmake_minimum_required(VERSION 3.12)
project(Antropse)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_FLAGS "-Wall -Werror -O2")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(gtest/include)

# Set Library dir
link_directories(gtest/lib)

# TODO: een set maken voor bestanden die zowel in DEBUG als RELEASE zitten
# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES main.cpp Vehicle.cpp Vehicle.h Car.cpp Car.h Road.cpp Road.h
        RoadNetwork.cpp RoadNetwork.h CONST.h TinyXML/tinystr.cpp TinyXML/tinystr.h TinyXML/tinyxml.cpp TinyXML/tinyxml.h
        TinyXML/tinyxmlerror.cpp TinyXML/tinyxmlparser.cpp Convert.cpp Convert.h NetworkExporter.cpp NetworkExporter.h
        AntropseUtils.cpp AntropseUtils.h NetworkImporter.cpp NetworkImporter.h MotorBike.cpp MotorBike.h Bus.cpp Bus.h
        Truck.cpp Truck.h GraphicImpressionExporter.cpp GraphicImpressionExporter.h TrafficLight.h TrafficLight.cpp)

# Set source files for DEBUG target
# TODO: Alle testen samen laten runnen mbv AntropseTests
set(DEBUG_SOURCE_FILES AntropseTests.cpp Vehicle.cpp Vehicle.h Car.cpp Car.h Road.cpp Road.h
        RoadNetwork.cpp RoadNetwork.h CONST.h TinyXML/tinystr.cpp TinyXML/tinystr.h TinyXML/tinyxml.cpp TinyXML/tinyxml.h
        TinyXML/tinyxmlerror.cpp TinyXML/tinyxmlparser.cpp Convert.cpp Convert.h NetworkImporter.cpp NetworkImporter.h
        NetworkImporterTests.cpp AntropseUtils.cpp AntropseUtils.h NetworkDomainTests.cpp NetworkExporterTests.cpp
        NetworkExporter.cpp NetworkExporter.h MotorBike.cpp MotorBike.h Bus.cpp Bus.h Truck.cpp Truck.h TrafficLight.cpp
        TrafficLight.h GraphicImpressionExporter.cpp GraphicImpressionExporter.h)

# Create RELEASE target
add_executable(Antropse ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(AntropseDebug ${DEBUG_SOURCE_FILES})

# Link library
target_link_libraries(AntropseDebug gtest)





