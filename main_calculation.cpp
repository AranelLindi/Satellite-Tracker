// Standartbibliotheken
#include <cmath>    // PI
#include <cstdint>  // int-defs.
#include <iostream> // IO
#include <fstream>  // IO

// eigener Code
#include "calendar/GregorianCalendar.h"                     // GregorianCalendar-Objekt
#include "Orbitpropagation/coordinates/geocoordinates.h"    // Koordinaten-Objekte
#include "Orbitpropagation/TLEinterpreter/tleinterpreter.h" // TLE einlesen
#include "calc/sat_calculation.h"

int main(void) // Enthält Beispiel zur Implementierung
{
    const GregorianCalendar START = {2020, 5, 14, 9, 32, 0};
    const GregorianCalendar ENDE = {2020, 6, 29, 14, 32, 0};

    // TLE laden:
    const auto _tle = readTlesFromFile("SONATE.txt");
    const auto sonate = _tle.at(44400);

    // und SGP4 Calc. übergeben:
    SGP4Propagator prop;
    prop.setTle(sonate);

    // Eigenen Writer anlegen:
    std::ofstream mywriter("Ausgabe32.txt");

    // WICHTIG! INKREMENT AUF 10s GEÄNDERT!
    for (GregorianCalendar i{START}; i != ENDE; i += 3600)
    {
        mywriter << (int)i.day << "." << (int)i.month << "." << (int)i.year << " " << (int)i.hour << ":" << (int)i.minute << ":" << (int)i.sec << '\n';

        // für jede Iteration die Werte berechnen:

        // julian day number
        const double jd{computeJDFromGregCal(i)};

        ECICoordinate posSat, posStation;

        prop.calculatePositionAndVelocity(i.hour * 60 * 24 + i.minute * 60 + i.sec, posSat, posStation);

        CoordinateConvertion cc;
        // ECI Position Satellit
        const GeodeticCoordinate gcsat{cc.convertECItoGeodetic(posSat, jd)};

        // ECI Position Bodensation ( oder gecentric ?? )
        const GeodeticCoordinate gcstation{cc.convertECItoGeodetic(posStation, jd)};

        // Koordinaten topozentrisches Horizontsystem
        const SEZCoordinate sz{transformECIToSEZ(posSat, gcsat, jd)};

        // Schrägentfernung
        const double SlantRange{computeSlantRange(sz)};

        // Azimuth
        const double azimuth{rad2deg(computeAzimuth(sz))};

        // Elevation
        const double elevation{rad2deg(computeElevation(sz))};

        // Anschließend alles in den Filestream schreiben:
        mywriter.precision(15);
        mywriter
            << jd << ';'
            << posSat.x << '|' << posSat.y << '|' << posSat.z << ';'
            << posStation.x << '|' << posStation.y << '|' << posStation.z << ';'
            << sz.rE << '|' << sz.rS << '|' << sz.rZ << ';'
            << SlantRange << ';'
            << azimuth << ';'
            << elevation << '\n';
    }

    mywriter << std::flush;

    mywriter.close();
}