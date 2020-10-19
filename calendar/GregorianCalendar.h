#ifndef GREGORIANCALENDAR_H
#define GREGORIANCALENDAR_H

/*
Definiert ein Objekt, dass den gregorianischen Kalendar repräsentiert sowie
zugehörige Operatoren um verschiedene Datumsoperationen durchzuführen.
*/

// Standartbibliotheken
#include <cstdint>

// eigener Code
#include "../Orbitpropagation/calendars/calendar.h" // für Funktionen die julianischen Kalendar betreffen

struct GregorianCalendar
{
    int16_t year;  // year
    int8_t month;  // month [1; 12]
    int8_t day;    // day
    int8_t hour;   // hour [0; 24]
    int8_t minute; // minute [0, 60)
    int8_t sec;    // second [0, 60)

    // Konstruktor:
    GregorianCalendar() noexcept {}
    GregorianCalendar(int16_t _year, int8_t _month, int8_t _day, int8_t _hour, int8_t _minute, int8_t _sec) noexcept : year(_year), month(_month), day(_day), hour(_hour), minute(_minute), sec(_sec) {}

    // Inkrementieren:
    GregorianCalendar &operator++(int) noexcept; // s. +=-Beschreibung

    // Arithmetik:
    int64_t operator-(const GregorianCalendar &gregCal2) const noexcept; // verwendet JD, deshalb noexcept möglich!

    // Zuweisung:
    GregorianCalendar &operator+=(const uint64_t &_i) noexcept; // noexcept vielleicht etwas gewagt... ausgeschlossen können Fehler jedensfalls nicht zu 100%
    GregorianCalendar &operator=(const GregorianCalendar &gregCal2) noexcept;

    // Vergleichsoperatoren:
    bool operator!=(const GregorianCalendar &gregCal2) const noexcept;
    bool operator==(const GregorianCalendar &gregCal2) const noexcept;
    bool operator<(const GregorianCalendar &gregCal2) const noexcept;
    bool operator>(const GregorianCalendar &gregCal2) const noexcept;
    bool operator<=(const GregorianCalendar &gregCal2) const noexcept;
    bool operator>=(const GregorianCalendar &gregCal2) const noexcept;
    /* auto operator<=>(const GregorianCalendar& gregCal2) const; */
};

// Verwendet entsprechende Methode aus Orbitpropagation/calendar um gregorianisches Datum in JDF zu überführen.
double computeJDFromGregCal(const GregorianCalendar &gregCal) noexcept;

// Prüft ob das übergebene Jahr ein Schaltjahr ist (true) oder nicht (false)
inline bool checkLeapYear(const uint16_t year) noexcept
{
    return ((year % 4 == 0) && (!(year % 100 == 0) | (year % 400 == 0)));
}

// Operator Overloading:
// Erhöht GregorianCalendar-Objekt um _i Sekunden.
//GregorianCalendar &operator+=(GregorianCalendar &temp, const uint64_t _i);
// Inkrementiert GregorianCalendar-Objekt um 1 Sekunde.
//GregorianCalendar &operator++(GregorianCalendar &temp, int);

// Differenz zweier GregorianCalendar in Sekunden mit Vorzeichen (<0 : gregCal1 < gregCal2 (gregCal1 ist früher als GregCal2), >0 : gregCal1 > gregCal2 (gregCal1 ist später als GregCal2))
//int64_t operator-(const GregorianCalendar& gregCal1, const GregorianCalendar& gregCal2);

// Prüft ob zwei GregorianCalendar ungleich sind.
//bool operator!=(const GregorianCalendar &gregCal1, const GregorianCalendar &gregCal2);

// Prüft ob zwei GregorianCalendar das gleiche Datum darstellen.
//bool operator==(const GregorianCalendar &gregCal1, const GregorianCalendar& gregCal2);

// Prüft ob gregCal1 einen frühere Zeitpunkt als gregCal2 repräsentiert.
//bool operator<(const GregorianCalendar& gregCal1, const GregorianCalendar& gregCal2);

// Prüft ob gregCal1 einen späteren Zeitpunkt als gregCal2 repräsentiert.
//bool operator>(const GregorianCalendar& gregCal1, const GregorianCalendar& gregCal2);

// Prüft ob gregCal1 einen früheren oder gleichen Zeitpunkt wie gregCal2 repräsentiert.
//bool operator<=(const GregorianCalendar& gregCal1, const GregorianCalendar& gregCal2);

// Prüft ob gregCal1 einen gleichen oder späteren Zeitpunkt wie gregCal2 repräsentiert.
//bool operator>=(const GregorianCalendar& gregCal1, const GregorianCalendar& gregCal2);

/* C++20 Feature: (<=> ermöglicht <, >, == in einem einzigen Operator)
auto operator<=>(const GregorianCalendar& gregCal1, const GregorianCalendar& gregCal2);
*/

#endif // GregorianCalendar.h