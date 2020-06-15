cmake_minimum_required(VERSION 3.13)

project(SatelliteTracking) # Project Name

set(CMAKE_CXX_STANDART 17) # Cpp17 Standard

add_executable(SatelliteTracking 
                main_position.cpp 
                calendar/GregorianCalendar.cpp 
                contact/contact_time.cpp 
                calc/sat_calculation.cpp 
                Orbitpropagation/calendars/calendar.cpp
                Orbitpropagation/coordinates/geocoordinates.cpp
                Orbitpropagation/TLEinterpreter/tle/TLE.cpp
                Orbitpropagation/TLEinterpreter/reader/reader.cpp
                Orbitpropagation/SGP4Propagator/SGP4Propagator.cpp
            )
