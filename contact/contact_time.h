// Standartbibliotheken
#include <cstdint>  // int-Variablen
#include <fstream>  // IO
#include <iostream> // IO
#include <ctime> // seconds since epoch
//#include <cstdio>  // Konsolenausgabe

// eigener Code
#include "../cal/GregorianCalendar.h"                 // GregorianCalendar-Objekt
#include "../Orbitpropagation/orbitpropagation.h" // Calendar-Funktionen
#include "../calc/sat_calculation.h"                   // (Aufgabe 3.1)
#include "../pos/position.h"                          // Operator-Overloading für GregorianCalendar

// Stellt ein Objekt dar, dass den Status der Verbindung zu einem Satelliten protokolliert.
class ContactTimes
{
    GregorianCalendar begin; // Speichert Zeitpunkt von AOS (für Berechnung von Duration)
    std::ofstream *_writer;  // IO Writer, in den geschrieben werden soll

public:
    ContactTimes(std::ofstream &writer);

    // Zeitpunkt, bei dem der Kontakt mit Satelliten beginnt.
    void AOS(const GregorianCalendar &gc);
    // Zeitpunkt, bei der Kontakt mit Satelliten endet.
    void LOS(const GregorianCalendar &gc);

    // Operator überladen um zwei GregorianCalendar-Objekte voneinander subtrahieren zu können.
    // Liefert die Anzahl Sekunden Differenz der beiden Objekte

};

int64_t operator-(const GregorianCalendar &a, const GregorianCalendar &b);

// Berechnet das Datum nach dem gregorianischen Kalender in Abhängigkeit einer Julian Day Fraction.
GregorianCalendar computeGragCalFromJD(double jd);

// Prüft anhand mehrerer Bedingungen, ob der Satellit Funkkontakt zur Bodenstation haben müsste
bool SatelliteAvailable(double ele);

void DetermineContactTimes(void);