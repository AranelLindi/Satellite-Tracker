#ifndef GREGORIANCALENDAR_H
#define GREGORIANCALENDAR_H

// Standartbibliotheken
#include <cstdint>

// eigener Code
#include "../Orbitpropagation/calendars/calendar.h"

struct GregorianCalendar {
    int16_t year; // year
    int8_t month; // month [1; 12]
    int8_t day; // day
    int8_t hour; // hour [0; 24]
    int8_t minute; // minute [0, 60)
    int8_t sec; // second [0, 60)
};

// Verwendet entsprechende Methode aus Orbitpropagation/calendar um gregorianisches Datum in JDF zu überführen.
double computeJDFromGregCal(const GregorianCalendar& gregCal);

// Operator Overloading:
// Erhöht GregorianCalendar-Objekt um _i Sekunden.
GregorianCalendar& operator+=(GregorianCalendar& temp, const uint64_t _i);
// Inkrementiert GregorianCalendar-Objekt um 1 Sekunde.
GregorianCalendar& operator++(GregorianCalendar& temp, int);
// Prüft ob zwei GregorianCalendar das gleiche Datum darstellen.
bool operator!=(const GregorianCalendar &gregCal1, const GregorianCalendar &gregCal2);

#endif // GregorianCalendar.h