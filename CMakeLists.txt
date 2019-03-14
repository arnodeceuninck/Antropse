cmake_minimum_required(VERSION 3.12)
project(Antropse)

set(CMAKE_CXX_STANDARD 98)

add_executable(Antropse main.cpp Vehicle.cpp Vehicle.h Car.cpp Car.h Road.cpp Road.h Intersection.cpp Intersection.h
        RoadNetwork.cpp RoadNetwork.h CONST.h TinyXML/tinystr.cpp TinyXML/tinystr.h TinyXML/tinyxml.cpp TinyXML/tinyxml.h
        TinyXML/tinyxmlerror.cpp TinyXML/tinyxmlparser.cpp Convert.cpp Convert.h)

