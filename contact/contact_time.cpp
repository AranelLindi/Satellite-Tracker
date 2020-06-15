#include "contact_time.h" // Header

#define __abstand__ std::setfill('0') << std::setw(2)

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
    // \033[1m = Fettschrfit auf Ubuntu Konsole
    // \022[31m = Schriftfarbe Rot
    // \033[0m = Beendet Formatierungen
    writer << "\033[1m\033[31mAOS (Kontaktaufnahme)\tLOS (Kontaktverlust)\tDUR (Dauer)\033[0m" << std::endl;
}

void ContactTimes::AOS(const GregorianCalendar &gc)
{
    this->begin = gc;                                                                                                                                                                                                                                       // Zeitpunkt merken, an dem Kontakt zustanden gekommen ist
    *(this->_writer) << std::fixed << __abstand__ << (int)gc.day << "." << __abstand__ << (int)gc.month << "." << (int)gc.year << " " << __abstand__ << (int)gc.hour << ":" << __abstand__ << (int)gc.minute << ":" << __abstand__ << (int)gc.sec << " ; "; // erst bei vollständigem Datensatz (LOS) flushen!
}

void ContactTimes::LOS(const GregorianCalendar &gc)
{
    // Differenz zwischen AOS und LOS ausrechnen:
    int32_t diff = gc - this->begin; // Sekunden

    uint32_t minutes = diff / 60;      // Enthält Ganzzahl der Anzahl der Minuten
    uint8_t sec = diff - minutes * 60; // Enthält Sekunden

    *(this->_writer) << std::fixed;
    *(this->_writer) << __abstand__ << (int)gc.day << "." << __abstand__ << (int)gc.month << "." << (int)gc.year << " " << __abstand__ << (int)gc.hour << ":" << __abstand__ << (int)gc.minute << ":" << __abstand__ << (int)gc.sec;
    *(this->_writer) << " ; " << minutes << ":" << (int)sec << std::endl;
}

void DetermineContactTimes(const std::string &filename, Tle &tle, const GregorianCalendar &start, const GregorianCalendar &ende, float observer_heigth, float observer_latitude_degree, float observer_longitude_degree, bool (*condition)(double)) // Bedigungen aka Parameter noch nicht ganz geklärt, deswegen hier 'void'. Sobald geklärt, hier so ersetzen, dass Funktion theoretisch auch für andere Satelliten ausgeführt werden könnte.
{
    SGP4Propagator prop;
    prop.setTle(tle);

    // Objekte für Koordinatenberechnungen/-transformationen:
    SEZCoordinate sezCoord;
    ECICoordinate satPos, satVel, obsPosECI, rObsSat;
    GeodeticCoordinate obsPosGeod;                             // Beobachter
    obsPosGeod.height = observer_heigth;                       // 0.275
    obsPosGeod.latitude = deg2rad(observer_latitude_degree);   // 49.78
    obsPosGeod.longitude = deg2rad(observer_longitude_degree); // 9.97

    double jdEpoch = Calendar::computeJD(tle.getYear(), tle.getDayFraction()); // TLE Epoch berechnen

    // IO-Writer:
    std::ofstream mywriter(filename);
    // Ausgabe Kontaktzeiten:
    ContactTimes ct(mywriter); //

    // Speichert ob aktuell ein Kontakt mit Satelliten vorliegt:
    bool contact = false; // false = kein Kontakt, true = Kontakt. Wird in for-Schleife gesetzt.

    // Mindestelevation für Kontakt
    const double MINELEVATION = M_PI / 36;

    // gc wird bei jeder Iteration um 1s inkrementiert
    for (GregorianCalendar gc = start; gc <= ende; gc++)
    {
        double jd_current = computeJDFromGregCal(gc);

        //std::cout << (int)gc.day << "." << (int)gc.month << "." << (int)gc.year << " " << (int)gc.hour << ":" << (int)gc.minute << ":" << (int)gc.sec << std::endl;
        //std::cout << (jd_current - jd_epoch)*86400 << std::endl;

        // Datei pro Iteration neu berechnen:
        prop.calculatePositionAndVelocity((jd_current - jdEpoch) * 86400.0, satPos, satVel);

        // Beobachter:
        ECICoordinate obsECI = convertGeodeticToECI(obsPosGeod, jd_current);

        // Beobachtungsvektor berechnen:
        ECICoordinate rvECI;
        rvECI.x = satPos.x - obsECI.x;
        rvECI.y = satPos.y - obsECI.y;
        rvECI.z = satPos.z - obsECI.z;

        SEZCoordinate rvSEZ = transformECIToSEZ(rvECI, obsPosGeod, jd_current);

        double elevation = computeElevation(rvSEZ);
        //std::cout << elevation << std::endl;

        //std::cout << (int)gc.hour << ":" << (int)gc.minute << ":" << (int)gc.sec << "\t" << (computeElevation(rvSEZ))*(M_PI/180.0) << std::endl;

        // elevation >= MINELEVATION

        if ((condition(elevation)) && !(contact))
        {
            // In der letzten Iteration bestand noch kein Kontakt, jetzt aber schon!
            contact = true;
            //std::cout << "Contact!" << std::endl;
            //std::cout << (int)gc.hour << ":" << (int)gc.minute << ":" << (int)gc.sec << std::endl;
            ct.AOS(gc);
        }
        else if (contact && !(condition(elevation))) // elevation < MINELEVATION
        {
            // In der letzten Iteration bestand noch Kontakt, aber jetzt nicht mehr!
            contact = false;
            //std::cout << "Contact lost!" << std::endl;
            ct.LOS(gc);
        }
    }

    // Bereinigen und File schließen:
    mywriter.flush();
    mywriter.close();
}