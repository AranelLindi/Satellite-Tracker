#include "contact_time.h" // Header

GregorianCalendar computeGragCalFromJD(double jd)
{
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

ContactTimes::ContactTimes(std::ofstream &writer)
{
    // Klasse verwendet übergebenen Writer um ihre Daten zu schreiben
    // Es erfolgt keine Prüfung ob writer auf gültiges Objekt verweist!
    this->_writer = &writer;
}

std::string ContactTimes::createDateString(const GregorianCalendar &gc)
{
    return std::to_string(gc.day + '.' + gc.month + '.' + gc.year + ' ' + gc.hour + ':' + gc.minute + ':' + gc.sec);
}

void ContactTimes::AOS(const GregorianCalendar &gc)
{
    this->begin = gc; // Zeitpunkt merken, an dem Kontakt zustanden gekommen ist
    *(this->_writer) << createDateString(gc) << ";"
                     << ";"; // erst bei vollständigem Datensatz (LOS) flushen!
}

void ContactTimes::LOS(const GregorianCalendar &gc)
{
    // Differenz zwischen AOS und LOS ausrechnen:
    int32_t diff = gc - this->begin; // Sekunden

    uint32_t minutes = diff / 60;      // Enthält Ganzzahl der Anzahl der Minuten
    uint8_t sec = diff - minutes * 60; // Enthält Sekunden

    *(this->_writer) << createDateString(gc) << ';' << minutes << ':' << sec << std::endl; // flushen
}

int64_t operator-(const GregorianCalendar &a, const GregorianCalendar &b)
{
    // rechnet den zeitlichen Unterschied zwischen a und b aus und gibt diesen
    // als Sekunden zurück.

    int64_t diff = 0;

    diff = a.sec - b.sec +
           (a.minute - b.minute) * 60 +
           (a.hour - b.hour) * 24 * 60;

    // TODO: Weitere Berechnungen bis einschl. Jahr einfügen! Für Aufgaben reicht es bis hier!
}

int main(void)
{
    const double MIN_ELEVATION_RANGE = M_PI / 36; // entspricht 5 Grad

    const GregorianCalendar ENDE = {2020, 5, 30, 23, 59, 59};
    GregorianCalendar START = {2020, 5, 30, 0, 0, 0};

    double jd_start = computeJDFromGregCal(START);

    bool contact = false; // false = kein Kontakt, true = Kontakt. Wird in for-Schleife gesetzt.

    std::ofstream mywriter;

    ContactTimes ct(mywriter);

    // gc wird bei jeder Iteration um INTERVAL (Standart: 30 Sekunden) inkrementiert
    for (GregorianCalendar gc = START; gc != ENDE; gc++)
    {
        if (!contact && SatelliteAvailable())
        {
            // In der letzten Iteration bestand noch kein Kontakt, jetzt aber schon!
            contact = true;

            ct.AOS(gc);
        }
        else if (contact && !SatelliteAvailable())
        {
            // In der letzten Iteration bestand noch Kontakt, aber jetzt nicht mehr!
            contact = false;

            ct.LOS(gc);
        }
    }

    // Bereinigen und File schließen:
    mywriter.flush();
    mywriter.close();
}

/*
    CODE UM COMPUTEGRAGCALFROMJD ZU TESTEN


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

*/