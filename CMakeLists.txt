cmake_minimum_required(VERSION 3.12)
project(Antropse)


set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 98)
# set(CMAKE_CXX_FLAGS "-Wall -Werror -O2")
# set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS "-Wall -Werror -O2 -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "-pthread -fprofile-arcs -ftest-coverage")

# Set include dir
include_directories(gtest/include)

# Set Library dir
link_directories(gtest/lib)

set(TINYXML_SOURCE_FILES TinyXML/tinystr.cpp TinyXML/tinystr.h
        TinyXML/tinyxml.cpp TinyXML/tinyxml.h
        TinyXML/tinyxmlerror.cpp
        TinyXML/tinyxmlparser.cpp)

set(ANTROPSE_SOURCE_FILES Vehicle.cpp Vehicle.h
        Car.cpp Car.h
        Road.cpp Road.h
        RoadNetwork.cpp RoadNetwork.h
        CONST.h
        Convert.cpp Convert.h
        NetworkImporter.cpp NetworkImporter.h
        AntropseUtils.cpp AntropseUtils.h
        NetworkExporter.cpp NetworkExporter.h
        MotorBike.cpp MotorBike.h
        Bus.cpp Bus.h
        Truck.cpp Truck.h
        TrafficLight.cpp TrafficLight.h
        GraphicImpressionExporter.cpp GraphicImpressionExporter.h
        ${TINYXML_SOURCE_FILES})

# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES main.cpp)

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES AntropseTests.cpp
        NetworkImporterTests.cpp
        NetworkDomainTests.cpp
        NetworkExporterTests.cpp)

# Create RELEASE target
add_executable(Antropse ${RELEASE_SOURCE_FILES} ${ANTROPSE_SOURCE_FILES})

# Create DEBUG target
add_executable(AntropseDebug ${DEBUG_SOURCE_FILES} ${ANTROPSE_SOURCE_FILES})

# Link library
target_link_libraries(AntropseDebug gtest)





