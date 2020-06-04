#include "contact_time.h" // Header

#include <cstdio>



GregorianCalendar computeGregFromJD(double jd) {
    // Quelle: http://articles.adsabs.harvard.edu//full/1984QJRAS..25...53H/0000055.000.html
    // (Teilweise modifiziert, weil Tests nicht das richtige Ergebnis lieferten)
    // (Zusätzlich Rechnung des Fractional Parts in Uhrzeit hinzugefügt)

    // temporäre Variablen
    int16_t g = (floor(floor((jd - 4479.5) / 36524.25) * 0.75 + 0.5)) - 37;
    uint32_t N = jd + g + 0.5; // +0.5 von mir hinzugefügt um Tagesverschiebung von Julianischen in gregorianischen Kalender auszugleichen (Workaround!)
    int16_t d_ = floor(fmod((N - 59.25), 365.25));

    // ***************** JAHR/MONAT/TAG *****************
    // Jahr:
    int16_t A = (floor(N / 365.25)) - 4712;

    // Monat:
    int8_t M = (fmod((((d_ + 0.5) / 30.6) + 2), 12)) + 1;

    // Tag:
    int8_t D = (floor(fmod((d_ + 0.5), 30.6))) + 1;
    // **************************************************

    // ***************** STUNDE/MINUTE/SEKUNDE *****************
    //  Uhrzeit wird unabhängig von oberen Variablen berechnet,
    //  es wird lediglich der Fractional Part von jd für die Be-
    //  rechnungen extrahiert und verwendet, da in der Datums-
    //  berechnung (oben) Workarounds vorgenommen wurden.

    uint8_t hour = 0, minute = 0, sec = 0;

    double dummy = 0; // wird nicht verwendet, enthält nur den Integral Part von jd
    double jd_frac = modf(jd, &dummy);

    // JD Tag geht von 12-12 Uhr, Gregorian von 0-24 Uhr; hier Zeit entsprechend umrechnen:
    if (jd_frac >= 0.5)
        jd_frac -= 0.5;
    else if (jd_frac < 0.5)
        jd_frac += 0.5;

    // Stunde:
    double hour_frac = jd_frac * 24;
    hour = hour_frac;

    // Minute:
    double minute_frac = (hour_frac - hour) * 60; // Zieht Ganzzahl von Fraction ab, um nur Fractional Part zu erhalten
    minute = minute_frac;

    // Sekunde:
    double sec_frac = (minute_frac - minute) * 60; // (siehe Minute)
    sec = sec_frac;
    // *********************************************************

    // ***************** RÜCKGABE *****************
    //  Ausgerechnetes Datum in ein GregorianCalendar
    //  -Objekt schreiben:
    GregorianCalendar calcDate;
    calcDate.year = A;
    calcDate.month = M;
    calcDate.day = D;
    calcDate.hour = hour;
    calcDate.minute = minute;
    calcDate.sec = sec;

    // ... und zurückgeben:
    return calcDate;
    // ***********************************************
}



int main(void)
{
    double jd; //= 2459003.5;

    int year;
    int month, day, hour, minute, sec;

    std::cin >> year;
    std::cin >> month;
    std::cin >> day;
    std::cin >> hour;
    std::cin >> minute;
    std::cin >> sec;

    double calcedJD = Calendar::computeJD((uint16_t)year, (uint8_t)month, (uint8_t)day, (uint8_t)hour, (uint8_t)minute, (uint8_t)sec, 0, 0);
    
    std::cout.precision(15);
    std::cout << calcedJD << std::endl;

    auto t = computeGregFromJD(calcedJD);

    std::cout << '\n';
    std::cout << (int)t.day << "." << (int)t.month << "." << t.year << " " << (int)t.hour << ":" << (int)t.minute << ":" << (int)t.sec << std::endl;
}