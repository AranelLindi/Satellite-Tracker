#ifndef GREGORIANCALENDAR_H
#define GREGORIANCALENDAR_H

// Standartbibliotheken
#include <cstdint>

// eigener Code
#include "Orbitpropagation/calendars/calendar.h"

struct GregorianCalendar {
    int16_t year; // year
    int8_t month; // month [1; 12]
    int8_t day; // day
    int8_t hour; // hour [0; 24]
    int8_t minute; // minute [0, 60)
    int8_t sec; // second [0, 60)
};

double computeJDFromGregCal(const GregorianCalendar& gregCal);


#endif // GregorianCalendar.h