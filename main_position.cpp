// Standartbibliotheken
#include <iostream>
#include <fstream>

// eigener Code
#include "Orbitpropagation/orbitpropagation.h" // für SGP4-Berechnungen
#include "calendar/GregorianCalendar.h"        // Calendar-Container

const GregorianCalendar START {2020, 5, 29, 14, 25, 0};
const GregorianCalendar ENDE {2020, 5, 29, 14, 32, 0};

// Schreibt ein ECICoordinate-Objekt in den Filestream
inline void writeECI(std::ofstream &writer, const ECICoordinate &eci, const GregorianCalendar &greg) noexcept // -> Wenn es einen Header gibt, gehört diese inline funktion genau da rein! MACHEN!
{
    //writer << computeJDFromGregCal(greg) << "," << eci.x << "," << eci.y << "," << eci.z << '\n';
    writer << eci.x/100 << "," << eci.y/100 << "," << eci.z/100 << '\n';
    // KEIN FLUSHEN! Ist zu teuer! Nur am Ende einmal!
}

int main(void)
{
    // Alle TLES aus Datei abfragen:
    const std::map<int, Tle> _tles = readTlesFromFile("SONATE.txt");
    const Tle sonate = _tles.at(44400); // quasi das einzige TLE abfragen

    // SGP4 Propagator laden und SONATE Tle übergeben:
    SGP4Propagator prop;
    prop.setTle(sonate);

    GregorianCalendar ctime {START}; // Zuweisung der Startzeit zum "Iterationsobjekt"

    ECICoordinate pos {0, 0, 0}; // Position
    ECICoordinate vel {0, 0, 0}; // Geschwindigkeit

    // CSV Datei vorbereiten:
    std::ofstream myfile;
    myfile.open("Ausgabe31.txt");

    uint64_t counter {0}; // zählt Datensätze
    while ((ctime <= ENDE))
    {
        prop.calculatePositionAndVelocity((computeJDFromGregCal(ctime) - Calendar::computeJD(sonate.getYear(), sonate.getDayFraction())) * 86400, pos, vel);

        myfile.precision(15);
        writeECI(myfile, pos, ctime);

        ctime += 30; // Inkrementiert Pos um 30 Sekunden (Operator Overloading!)

        counter++; // Counter inkrementieren (Datzsätze)
    }

    std::cout << "Datensätze: " << counter << '\n';

    // Am Ende einmal flushen:
    myfile << std::flush;

    // Fertig
    myfile.close();
}