#ifndef POSITION_H
#define POSITION_H

// Standartbibliotheken
#include <iostream>
#include <fstream>

// eigener Code
#include "../Orbitpropagation/orbitpropagation.h" // für SGP4-Berechnungen
#include "../calendar/GregorianCalendar.h"                 // Calendar-Container

struct Position
{
    // Konstanten: (Aufgabe 3.1)
    const GregorianCalendar START = {2020, 5, 29, 14, 25, 0}; // Stellt Dummy-mäßig die Start-Zeit dar
    const GregorianCalendar ENDE = {2020, 5, 29, 14, 32, 0};  // Stellt zum vergleichen, die End-Zeit dar

    // Es können die Operatoren benutzt werden, die in GregorianCalendar.h definiert sind!

    void dummy(void); // ehemalige Main funktion: noch umbennen!
};
#endif // position.h