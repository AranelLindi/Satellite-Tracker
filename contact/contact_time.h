// Standartbibliotheken
#include <cstdint>  // int-Variablen
#include <fstream>  // IO
#include <iostream> // IO
#include <iomanip>  // verbesserte Konsolenausgabe

// eigener Code
#include "../calendar/GregorianCalendar.h"        // GregorianCalendar-Objekt & Operator Overloading
#include "../Orbitpropagation/orbitpropagation.h" // Calendar-Funktionen
#include "../calc/sat_calculation.h"              // (Aufgabe 3.1)

/**
 * Protokolliert aufgebaute bzw. verlorene Verbindungen zu einem Satelliten.
 */
class ContactTimes
{
    GregorianCalendar begin; // Speichert Zeitpunkt von AOS (für Berechnung von Duration)
    std::ofstream *_writer;  // IO Writer, in den geschrieben werden soll

public:
    ContactTimes(std::ofstream &writer);

    /**
     * Markiert den Zeitpunkt einer aufgebauten Verbindung zu einem Satelliten.
     * @param  {GregorianCalendar} gc : Zeitpunkt Kontakt
     */
    void AOS(const GregorianCalendar &gc);

    /**
     * Markiert den Zeitpunkt einer verloreren Verbindung zu einem Satelliten.
     * @param  {GregorianCalendar} gc : Zeitpunkt Verlust
     */
    void LOS(const GregorianCalendar &gc);

    // Operator überladen um zwei GregorianCalendar-Objekte voneinander subtrahieren zu können.
    // Liefert die Anzahl Sekunden Differenz der beiden Objekte
};

/**
 * Berechnet Kontaktzeiten eines Satelliten im Low Earth Orbit zu einem Beobachter (Bodenstation).
 * @param  {std::string} filename            : Ausgabedatei
 * @param  {Tle} tle                         : TLE Berechnungsgrundlage
 * @param  {GregorianCalendar} start         : Startzeitpunkt Berechnungen
 * @param  {GregorianCalendar} ende          : Endzeitpunkt Berechnungen
 * @param  {float} observer_heigth           : Höhe Beobachter (Bodenstation)
 * @param  {float} observer_latitude_degree  : Breitengrad Beobachter (Bodenstation)
 * @param  {float} observer_longitude_degree : Längengrad Beobachter (Bodenstation)
 * @param  {bool(*)(double)} condition       : Funktionszeiger auf Funktion die Kontaktbedingung prüft (seperat implementieren!)
 */
void DetermineContactTimes(const std::string &filename, Tle &tle, const GregorianCalendar &start, const GregorianCalendar &ende, float observer_heigth, float observer_latitude_degree, float observer_longitude_degree, bool (*condition)(double));