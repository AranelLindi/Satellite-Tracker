cmake_minimum_required(VERSION 3.13)

project(SatelliteTracking) # Project Name

set(CMAKE_CXX_STANDART 17) # Cpp17 Standard

add_executable(SatelliteTracking main_3_1.cpp GregorianCalendar.cpp Orbitpropagation/calendars/calendar.cpp Orbitpropagation/coordinates/geocoordinates.cpp Orbitpropagation/TLEinterpreter/TLE.cpp Orbitpropagation/TLEinterpreter/process.cpp Orbitpropagation/SGP4Propagator/SGP4Propagator.cpp)
