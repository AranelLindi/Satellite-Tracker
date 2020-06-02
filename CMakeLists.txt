cmake_minimum_required(VERSION 3.13)

project(SatelliteTracking)

set(CMAKE_CXX_STANDART 17) # Cpp17 Standard

add_executable(SatelliteTracking main.cpp GregorianCalendar.cpp Orbitpropagation/Calendar/calendar.cpp Orbitpropagation/Coordinates/geocoordinates.cpp Orbitpropagation/TLEinterpreter/TLE.cpp Orbitpropagation/TLEinterpreter/process.cpp Orbitpropagation/SGP4Propagator/SGP4Propagator.cpp)
