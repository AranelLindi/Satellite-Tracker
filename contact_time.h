// Standartbibliotheken
#include <cstdint>

// eigener Code
#include "GregorianCalendar.h"
#include "Orbitpropagation/orbitpropagation.h"

// Berechnet das Datum nach dem gregorianischen Kalender in Abhängigkeit einer Julian Day Fraction.
GregorianCalendar computeGregFromJD(double jd);