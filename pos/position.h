#ifndef POSITION_H
#define POSITION_H

// Standartbibliotheken
#include <iostream>
#include <fstream>

// eigener Code
#include "Orbitpropagation/orbitpropagation.h" // für SGP4-Berechnungen
#include "GregorianCalendar.h"                 // Calendar-Container

struct Position
{
    // Konstanten: (Aufgabe 3.1)
    const GregorianCalendar START = {2020, 5, 29, 14, 25, 0}; // Stellt Dummy-mäßig die Start-Zeit dar
    const GregorianCalendar ENDE = {2020, 5, 29, 14, 32, 0};  // Stellt zum vergleichen, die End-Zeit dar

    // Müssen als 'friend' definiert werden, da
    friend bool operator!=(const GregorianCalendar &gregCal1, const GregorianCalendar &gregCal2);
    friend GregorianCalendar &operator++(GregorianCalendar &temp, int);

    void dummy(void); // ehemalige Main funktion: noch umbennen!
};

// Prototypen: Operatoren sind damit auch in anderen Files nutzbar
bool operator!=(const GregorianCalendar &gregCal1, const GregorianCalendar &gregCal2);
GregorianCalendar &operator++(GregorianCalendar &temp, int);

#endif // position.h