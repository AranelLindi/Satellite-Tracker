#include "position.h" // HEADER

#define INTERVAL 10 // [s] (Overloading increment-Operator!) Gibt an, um wieviele Sekunden ein GregorianCalendar-Objekt inkrementiert werden soll


// Operator != definieren, sodass zwei GregorianCalender bequem miteinander verglichen werden können


// Operator ++ definieren, sodass ein GregorianCalender, bequem um 30 Sekunden inkrementiert werden kann
/*GregorianCalendar &operator++(GregorianCalendar &temp, int)
{
    // Prüfen ob eine Variable ihren Definitionsbereich überschreiten würde, dann entsprechend
    // die höherhierarchischeren Variablen inkrementieren. Dies funktioniert, bis ein neuer
    // Tag beginnen würde, denn dort müsste dann wieder der gesamte Kalender mit eingebunden werden
    // (z.b Schalttagprüfungen etc)
    // ACHTUNG: Es darf maximal um 60 Sekunden inkrementiert werden!
    const int i = (unsigned)INTERVAL;


    int16_t year = 0;
    uint8_t month = 0;
    uint8_t day = 0;
    double hour = 0;
    double minute = 0;
    uint8_t sec = 0;

    // Liefert die Anzahl Tage des Monats abhängig vom Schaltjahr/Nicht-Schaltjahr
    const auto getNumberOfDaysInMonth = [](int16_t year, uint8_t month) -> uint8_t {
        const uint8_t dayspermonth[] = {31,28,31,30,31,30,31,31,30,31,30,31}; // const ermöglicht Optimierungen
        if (month == 2 && (year % 4 == 0 && (!(year % 100 == 0) | year % 400 == 0))) return 29; // Um Wert im Leap Year nicht zu erhöhen, in diesem Fall Konstante 29 zurückgeben
        return dayspermonth[month-1]; // ansonsten Wert aus Array entnehmen
    };
    
    // Liefert in Abhängigkeit der Anzahl Sekunden, die entsprechende Anzahl Monate. Berücksichtigt Schaltjahre
    //const auto getSecondsInMonth = [](int16_t year, uint64_t seconds) -> uint8_t { 
        
    //};

    // prüft für ein übergebenes Jahr, ob es ein Schaltjahr ist (gilt nur für gregorianischen Kalendar!)
    const auto leapyear = [](int16_t year) -> bool { return (year % 4 == 0 && (!(year % 100 == 0) | year % 400 == 0)); };

    // Liefert für temp und i in Tagen den neuen Monat
    const auto getMonth = [&temp](uint16_t days) -> uint8_t {
        uint8_t DAYSPERMONTH[] = {31,28,31,30,31,30,31,31,30,31,30,31};

        // Auf Schaltjahr prüfen:
        if(temp.year % 4 == 0 && (!(temp.year % 100 == 0) | temp.year % 400 == 0)) DAYSPERMONTH[1]++; // Feb. hat in Schaltjahr einen Tag mehr. Inkrementieren!
        
        uint8_t rmonth = 0; // Speichert Inkrementierungen des Monats die zurückgegeben werden
        
        days += temp.day; // geht bei Berechnung vom 0ten Tag des Monats aus und springt monatsweise zum nächsten Arrayfeld
        
        // Iteriert durch Monatsarray so lange, bis die stetig subtrahierten Tage nicht mehr reichen um über den aktuellen Monat zu "springen", dann sind wir im richtigen Monat gelandet
        for (uint8_t i = temp.month; days > DAYSPERMONTH[i-1]; i++)
        {
            days -= DAYSPERMONTH[i-1];
            rmonth++;
            if (i == 12) i = 1; // Im Dezember wieder auf Januar zurücksetzen
        }       
    };

    // Liefert den Tag des Monats abhängig von der Anzahl der Tage die hinzuzufügen sind
    const auto getDayInMonth = [&temp](uint32_t days) -> uint8_t {
        uint8_t DAYSPERMONTH[] = {31,28,31,30,31,30,31,31,30,31,30,31};

        // Auf Schaltjahr prüfen:
        if(temp.year % 4 == 0 && (!(temp.year % 100 == 0) | temp.year % 400 == 0)) DAYSPERMONTH[1]++; // Feb. hat in Schaltjahr einen Tag mehr. Inkrementieren!

        days += temp.day; // geht bei Berechnung vom 0ten Tag des Monats aus (indem die übergebenen Tage mit dem bisherigen Datum (Tag) summiert werden) und springt monatsweise zum nächsten Arrayfeld

        // Iteriert durch Monatsarray so lange, bis die stetig subtrahierte Tage nicht mehr reichen, um über den den aktuellen Monat zu "springen", dann sind wir im richtigen Monat gelandet.
        for(uint32_t i = temp.month; days > DAYSPERMONTH[i-1]; i++) {
            days -= DAYSPERMONTH[i-1]; // Anzahl Tage des Monats in dem wir sind, abziehen

            if (i == 12) i = 1; // Im Dezember wieder auf Januar zurücksetzen
        }
        // zuletzt die noch übrigen Tage im richtigen Monat zurückgeben:
        return days;
    };

    // Liefert Anzahl Sekunden eines Schaltjahres/normalen Jahres
    const auto getSecondsInYear = [](int32_t year) -> uint32_t {
                            const uint32_t SECONDSLEAPYEAR = 60*60*24*366;
                            const uint32_t SECONDSNORMYEAR = 60*60*24*365;
                            if (year % 4 == 0 && (!(year % 100 == 0) | year % 400 == 0)) {
                                    return SECONDSLEAPYEAR;
                            }
                            else {
                                    return SECONDSNORMYEAR;
                            }                            
                        };



    if (temp.sec + i >= 60) {
        // Es muss die Minute um mindestens 1 erhöht werden!
        if (temp.minute + ((i / 60) +1) >= 60) {
            // Es muss weiter die Stunde um mindestens 1 erhöht werden!
            if (temp.hour + ((i / (60*24)) + 1) >= 24) {
                // Es muss der Tag um mindestens 1 erhöht werden!
                if (temp.day + ((i / (60*60*24)) + 1) >= getNumberOfDaysInMonth(temp.year, temp.month)) {
                    // Es muss weiter der Monat um mindestens 1 erhöht werden!
                    if (temp.month + getMonth((i / (60*60*24))+1) >= 12) {
                        // Es muss weiter das Jahr um mindestens 1 erhöht werden!
                        temp.year++;

                        uint32_t _x = i; // Speichert als Kopie die übergebenen Anzahl Sekunden
                        // Prüfen ob das Jahr nochmal erhöht werden müsste: (So lange iterieren und erhöhen, bis _x bzw. i nicht mehr ausreichen um ein Jahr zu überspringen, dann sind wir im richtigen Jahr gelandet)                    
                        uint32_t secs = getSecondsInYear(temp.year); // Enthält für jede neue Iteration die Anzahl Sekunden des Jahres, abhängig von Schaltjahr/Nicht-Schaltjahr
                        for(int32_t _x = i; (_x - secs) >= 0; secs = getSecondsInYear(temp.year)) {
                            temp.year++;
                            _x -= secs;
                        }
                        // Nach der Loop, enthält _x die restlichen Anzahl Sekunden, die jetzt aufgeteilt werden müssen.
                        i = _x;
                    } else {
                        // Jahr muss nicht erhöht werden, nur Monat erhöhen, aber nicht über 12 (also Dezember):
                        uint32_t dd = i / (60*60*24); // i in Tagen - Intervall: [0 bzw. 1; theoretisch offen]
                        if (dd == 0) dd++;
                        temp.month = getMonth(dd);
                    }
                    // Tag erhöht sich über letzten Tag des Monats, neu berechnen:
                    uint32_t dd = i / (60*60*24); // i in Tagen - Intervall: [0 bzw. 1; theoretisch offen]
                    if (dd == 0) dd++;
                    temp.day = getDayInMonth(dd);
                } else {
                    // Monat muss nicht erhöht werden, nur Tage erhöhen, aber nicht über Ende des Monats:
                    uint8_t dd = i /(60*60*24); // Intervall dd: [0 bzw. 1; MAXDAYINMONTH] (max. 31)
                    if (dd == 0) dd++;
                    temp.day += dd; // sollte stimmen!
                } 
                // Stunden erhöhen sich über/auf 24, neu berechnen:
                temp.hour = (temp.hour + (i / (60*60))) % 24; // stimmt! Stunde berechnen
            } else {       
                // Tag muss nicht erhöht werden, nur Stunden erhöhen, aber nicht über 24
                uint8_t dh = i/(60*60); // Intervall dh: [0 bzw. 1; 24)
                if (dh == 0) dh++;
                temp.hour += dh; // sollte stimmen!
            }
            // Minuten erhöhen sich über/auf 60, neu berechnen:
            temp.minute = (temp.minute + (i/60)) % 60; // stimmt! Minute berechnen
        } else {
            // Stunde muss nicht erhöht werden, nur Minuten und Sekunden / Minuten bleiben unter 60!:
            uint8_t dm = i/60; // Intervall dm: [0 bzw. 1; 60)
            if (dm == 0) dm++; // mindestens um eine Minute inkrementieren. Wurde z.b. nur eine Sekunde hinzuaddiert, würde bei 1/60, 0 Minuten addiert werden.
            temp.minute += dm; // stimmt!
        }
        // Sekunden erhöhen sich über/auf 60, neu berechnen:
        temp.sec = (temp.sec + i) % 60; // stimmt! Sekunden berechnen
    } else { 
        // Minute muss nicht erhöht werden, nur Sekunden anpassen:
        temp.sec += i; // stimmt!
    } 

    return temp;
}*/

// Schreibt ein ECICoordinate-Objekt in den Filestream
inline void writeECI(std::ofstream &writer, const ECICoordinate &eci, const GregorianCalendar &greg) // -> Wenn es einen Header gibt, gehört diese inline funktion genau da rein! MACHEN!
{
    writer << computeJDFromGregCal(greg) << "," << eci.x << "," << eci.y << "," << eci.z << '\n';
    // KEIN FLUSHEN! Ist zu teuer! Nur am Ende einmal!
}

void Position::dummy(void)
{
    // Alle TLES aus Datei abfragen:
    std::map<int, Tle> _tles = readTlesFromFile("SONATE.txt");

    Tle sonate = _tles.at(44400); // quasi das einzige TLE abfragen

    // SGP4 Propagator laden und SONATE Tle übergeben:
    SGP4Propagator prop;
    prop.setTle(sonate);

    GregorianCalendar ctime = START; // Zuweisung der Startzeit zum "Iterationsobjekt"

    ECICoordinate pos = {0, 0, 0}; // Position
    ECICoordinate vel = {0, 0, 0}; // Geschwindigkeit

    // CSV Datei vorbereiten:
    std::ofstream myfile;
    myfile.open("Ausgabe31.txt");

    // Lambda, das aus dem GregorianCalender-Objekt die Sekunden seit Epoch zurückgibt:
    auto calcSeconds = [&ctime]() -> double {
        return ctime.hour * 3600 + ctime.minute * 60 + ctime.sec;
    };

    int counter = 0; // zählt Datensätze
    while ((ctime != ENDE))
    {
        prop.calculatePositionAndVelocity(calcSeconds(), pos, vel);

        myfile.precision(15);
        writeECI(myfile, pos, ctime);

        ctime += 30; // Inkrementiert Pos um 30 Sekunden (Operator Overloading!)

        counter++; // Counter inkrementieren (Datzsätze)
    }

    std::cout << "Datensätze: " << counter << '\n';

    // Am Ende einmal flushen:
    myfile << std::flush;

    // Fertig
    myfile.close();
}