#include "GregorianCalendar.h"


double computeJDFromGregCal(const GregorianCalendar& gregCal) {
    // Benutzt die entsprechende Funktion aus Orbitpropagation 'static double computeJD(int16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t milisecond, uint8_t microsecond);'
    
    return Calendar::computeJD(gregCal.year, gregCal.month, gregCal.day, gregCal.hour, gregCal.minute, gregCal.sec, 0, 0);
}