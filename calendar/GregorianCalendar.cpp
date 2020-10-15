#include "GregorianCalendar.h"

double computeJDFromGregCal(const GregorianCalendar &gregCal)
{
    // Benutzt die entsprechende Funktion aus Orbitpropagation 'static double computeJD(int16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t milisecond, uint8_t microsecond);'

    return Calendar::computeJD(gregCal.year, gregCal.month, gregCal.day, gregCal.hour, gregCal.minute, gregCal.sec, 0, 0);
}

GregorianCalendar& GregorianCalendar::operator=(const GregorianCalendar& gregCal2) noexcept {
    this->year = gregCal2.year;
    this->month = gregCal2.month;
    this->day = gregCal2.day;
    this->hour = gregCal2.hour;
    this->minute = gregCal2.minute;
    this->sec = gregCal2.sec;
}

GregorianCalendar &GregorianCalendar::operator+=(const uint64_t &_i) noexcept
{
    // Prüfen ob eine Variable ihren Definitionsbereich überschreiten würde, dann entsprechend die höherhierarchischeren Variablen inkrementieren.

    uint64_t i{_i}; // Hilfsvariable, speichert Anzah zu erhöhender Sekunden

    // ***************************
    //          lambdas
    // ***************************

    // Liefert die Anzahl Tage des Monats abhängig vom Schaltjahr/Nicht-Schaltjahr
    const auto getNumberOfDaysInMonth = [](int16_t year, uint8_t month) -> uint8_t {
        const uint8_t dayspermonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // const ermöglicht Optimierungen
        if ((month == 2) && (checkLeapYear(year)))
            return 29;                  // Um Wert im Leap Year nicht zu erhöhen, in diesem Fall Konstante 29 zurückgeben
        return dayspermonth[month - 1]; // ansonsten Wert aus Array entnehmen
    };

    // Liefert für temp und i (in Tagen) den neuen Monat
    const auto getMonth = [this](uint16_t days) -> uint8_t {
        uint8_t DAYSPERMONTH[]{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        // Auf Schaltjahr prüfen:
        if (checkLeapYear(this->year))
            DAYSPERMONTH[1]++; // Feb. hat in Schaltjahr einen Tag mehr. Inkrementieren!

        uint8_t rmonth{this->month}; // Speichert Inkrementierungen des Monats die zurückgegeben werden

        days += this->day; // geht bei Berechnung vom 0ten Tag des Monats aus und springt monatsweise zum nächsten Arrayfeld

        // Iteratiert durch DAYSPERMONTH so lange, bis die stetig substrahierten Tage nicht mehr ausreichen würden, um über den aktuellen Monat zu "springen", dann sind wir im richtigen Monat gelandet:
        while (days > DAYSPERMONTH[rmonth - 1])
        {
            days -= DAYSPERMONTH[rmonth - 1];
            rmonth++;
            if (rmonth == 12)
                rmonth = 1;
        }

        return rmonth;
    };

    // Liefert den Tag des Monats abhängig von der Anzahl der Tage die hinzuzufügen sind
    const auto getDayInMonth = [this](uint32_t days) -> uint8_t {
        uint8_t DAYSPERMONTH[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        // Auf Schaltjahr prüfen:
        if (checkLeapYear(this->year))
            DAYSPERMONTH[1]++; // Feb. hat in Schaltjahr einen Tag mehr. Inkrementieren!

        days += this->day; // geht bei Berechnung vom 0ten Tag des Monats aus (indem die übergebenen Tage mit dem bisherigen Datum (Tag) summiert werden) und springt monatsweise zum nächsten Arrayfeld

        // Iteriert durch Monatsarray so lange, bis die stetig subtrahierte Tage nicht mehr reichen, um über den den aktuellen Monat zu "springen", dann sind wir im richtigen Monat gelandet.
        for (uint32_t i{this->month}; days > DAYSPERMONTH[i - 1]; i++)
        {
            days -= DAYSPERMONTH[i - 1]; // Anzahl Tage des Monats in dem wir sind, abziehen

            if (i == 12)
                i = 1; // Im Dezember wieder auf Januar zurücksetzen
        }
        // zuletzt die noch übrigen Tage im richtigen Monat zurückgeben:
        return days;
    };

    // Liefert Anzahl Sekunden eines Schaltjahres/normalen Jahres
    const auto getSecondsInYear = [](int32_t year) -> uint32_t {
        const constexpr uint32_t SECONDSLEAPYEAR = 60 * 60 * 24 * 366;
        const constexpr uint32_t SECONDSNORMYEAR = 60 * 60 * 24 * 365;
        if (checkLeapYear(year))
            return SECONDSLEAPYEAR;
        else
            return SECONDSNORMYEAR;
    };

    // **********************************
    //          Vorberechnungen
    // **********************************

    // Vorberechnungen, die im Algorithmus gebraucht werden:
    uint8_t delta_day{i / (60 * 60 * 24)};
    if (delta_day == 0)
        delta_day++;

    uint8_t delta_hour{i / (60 * 60)};
    if (delta_hour == 0)
        delta_hour++;

    uint8_t delta_minute{i / 60};
    if (delta_minute == 0)
        delta_minute++;

    uint8_t delta_sec{i};

    // *******************************
    //          Algorithmus
    // *******************************

    if (this->sec + i >= 60)
    {
        // Es muss die Minute um mindestens 1 erhöht werden!
        if (this->minute + delta_minute >= 60)
        {
            // Es muss weiter die Stunde um mindestens 1 erhöht werden!
            if (this->hour + delta_hour >= 24)
            {
                // Es muss der Tag um mindestens 1 erhöht werden!
                if (this->day + delta_day > getNumberOfDaysInMonth(this->year, this->month))
                { // HIER GEÄNDERT VON >= IN >
                    // Es muss weiter der Monat um mindestens 1 erhöht werden!
                    if (this->month + getMonth(delta_day) > 12)
                    { // HIER GEÄNDERT VON >= IN >
                        // Es muss weiter das Jahr um mindestens 1 erhöht werden!
                        this->year++;

                        uint32_t _x{i}; // Speichert als Kopie die übergebenen Anzahl Sekunden
                        // Prüfen ob das Jahr nochmal erhöht werden müsste: (So lange iterieren und erhöhen, bis _x bzw. i nicht mehr ausreichen um ein Jahr zu überspringen, dann sind wir im richtigen Jahr gelandet)
                        uint32_t secs{getSecondsInYear(this->year)}; // Enthält für jede neue Iteration die Anzahl Sekunden des Jahres, abhängig von Schaltjahr/Nicht-Schaltjahr, Beginn für Iteration ist aktuelles Jahr
                        for (int32_t _x{i}; (_x - secs) >= 0; secs = getSecondsInYear(this->year))
                        {
                            this->year++;
                            _x -= secs;
                        }
                        // Nach der Loop, enthält _x die restlichen Anzahl Sekunden, die anschließend durch durchlaufen der restlichen if-Statements entsprechen verarbeitet werden.
                        i = _x;
                    }
                    else
                    {
                        // Jahr muss nicht erhöht werden, nur Monat erhöhen, aber nicht über 12 (also Dezember):
                        this->month = getMonth(delta_day);
                    }
                    // Tag erhöht sich über letzten Tag des Monats, neu berechnen:
                    this->day = getDayInMonth(delta_day);
                }
                else
                {
                    // Monat muss nicht erhöht werden, nur Tage erhöhen, aber nicht über Ende des Monats:
                    this->day += delta_day;
                }
                // Stunden erhöhen sich über/auf 24, neu berechnen:
                this->hour = (this->hour + (i / (60 * 60))) % 24; // Stunde berechnen
            }
            else
            {
                // Tag muss nicht erhöht werden, nur Stunden erhöhen, aber nicht über 24
                this->hour += delta_hour;
            }
            // Minuten erhöhen sich über/auf 60, neu berechnen:
            this->minute = (this->minute + (i / 60)) % 60; // Minute berechnen
        }
        else
        {
            // Stunde muss nicht erhöht werden, nur Minuten und Sekunden -- Minuten bleiben unter 60!
            this->minute += delta_minute;
        }
        // Sekunden erhöhen sich über/auf 60, neu berechnen:
        this->sec = (this->sec + i) % 60; // Sekunden berechnen
    }
    else
    {
        // Minute muss nicht erhöht werden, nur Sekunden anpassen:
        this->sec += i;
    }

    //return this; // WC: (8 + n)-Vergleiche (wobei; n = Anzahl zu addierender Jahre): O(n); BC: O(1)
}

GregorianCalendar &GregorianCalendar::operator++(int) noexcept
{
    // Anders als bei der Überladung des += Operators, wird hier immer nur um eine Sekunde inkrementiert!
    // es entfallen also Prüfungen bei denen denen ein größerer Zeitraum übersprungen wird.

    // ***************************
    //          lambdas
    // ***************************

    // Liefert die Anzahl Tage des Monats abhängig vom Schaltjahr/Nicht-Schaltjahr
    const auto getNumberOfDaysInMonth = [](uint16_t year, uint8_t month) -> uint8_t {
        const uint8_t dayspermonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // const ermöglicht Optimierungen
        if ((month == 2) && (checkLeapYear(year)))
            return 29;                  // Um Wert im Leap Year nicht zu erhöhen, in diesem Fall Konstante 29 zurückgeben
        return dayspermonth[month - 1]; // ansonsten Wert aus Array entnehmen
    };

    // *******************************
    //          Algorithmus
    // *******************************

    if (this->sec + 1 == 60)
    {
        // Minute wird inkrementiert
        if (this->minute + 1 == 60)
        {
            // Stunde wird inkrementiert
            if (this->hour + 1 == 24)
            {
                // Tag wird inkrementiert
                if (this->day + 1 > getNumberOfDaysInMonth(this->year, this->month))
                { // HIER: == IN > GEÄNDERT!
                    // Monat wird inkrementiert
                    if (this->month + 1 > 12)
                    { // HIER: == IN > GEÄNDERT!
                        // Jahr wird inkrementiert
                        this->year++;
                        this->month = 1;
                    }
                    else
                    {
                        // Nur Monat erhöhen
                        this->month++;
                    }
                    // Tag wieder auf 1 setzen
                    this->day = 1;
                }
                else
                {
                    // Nur Tag erhöhen
                    this->day++;
                }
                // Stunde wieder auf 0 setzen:
                this->hour = 0;
            }
            else
            {
                // Nur Stunde erhöhen
                this->hour++;
            }
            // Minute wieder auf 0 setzen:
            this->minute = 0;
        }
        else
        {
            // Nur Minute erhöhen
            this->minute++;
        }
        // Sekunde wieder auf 0 setzen:
        this->sec = 0;
    }
    else
    {
        // Nur Sekunde erhöhen
        this->sec++;
    }

    //return temp; // WC: 8 Vergleiche: O(1)
}

int64_t GregorianCalendar::operator-(const GregorianCalendar &gregCal2) const
{
    // Nutzt die vorhanden Funktionen in Orbitpropagation/calendar/calendar.h um gregCal1 und gregCal2 in julianischen Kalendar zu überführen und dann ihre
    // Differenz zu berechnen.

    const double jd1{computeJDFromGregCal(*this)};
    const double jd2{computeJDFromGregCal(gregCal2)};

    return (jd1 - jd2) * (60 * 60 * 24); // Differenz mal Sekunden pro Tag, da JDN in Tage somit bleiben Sekunden als Einheit stehen
}

bool GregorianCalendar::operator!=(const GregorianCalendar &gregCal2) const
{
    return !((this->year == gregCal2.year) &&
             (this->month == gregCal2.month) &&
             (this->day == gregCal2.day) &&
             (this->hour == gregCal2.hour) &&
             (this->minute == gregCal2.minute) &&
             (this->sec == gregCal2.sec));

    // WC: 7 Vergleiche: (1) // inkl. Negation
}

bool GregorianCalendar::operator==(const GregorianCalendar &gregCal2) const
{
    return ((this->year == gregCal2.year) &&
            (this->month == gregCal2.month) &&
            (this->day == gregCal2.day) &&
            (this->hour == gregCal2.hour) &&
            (this->minute == gregCal2.minute) &&
            (this->sec == gregCal2.sec));

    // WC: 6 Vergleiche: O(1)

    //return !(gregCal1 != gregCal2); // geht das so auch? effizient?
}

bool GregorianCalendar::operator<(const GregorianCalendar &gregCal2) const
{
    // Definition: Ein Datum ist kleiner relativ zu einem Anderen, wenn es einen früheren Zeitraum repräsentiert.

    // HILFSVARIABLEN

    // nur 'kleiner als'
    const bool Y{this->year < gregCal2.year};
    const bool M{this->month < gregCal2.month};
    const bool D{this->day < gregCal2.day};

    const bool h{this->hour < gregCal2.hour};
    const bool m{this->minute < gregCal2.minute};
    const bool s{this->sec < gregCal2.sec}; // nur kleiner relevant(!), da letzte Prüfungsinstanz wenn alle anderen 'gleich' sind

    // nur 'gleich'
    const bool Y_{this->year == gregCal2.year};
    const bool M_{this->month == gregCal2.month};
    const bool D_{this->day == gregCal2.day};

    const bool h_{this->hour == gregCal2.hour};
    const bool m_{this->minute == gregCal2.minute};

    // Damit gregCal1 < gregCal2 gilt, muss eine der folgenden Bedingungen erfüllt sein:
    // - Das Jahr von gregCal1 ist kleiner als das von gregCal2
    // - Das Jahr von gregCal1 ist gleich mit dem von gregCal2 UND der Monat von gregCal1 ist kleiner als der von gregCal2
    // - Das Jahr von gregCal1 ist gleich mit dem von gregCal2 UND der Monat von gregCal1 ist gleich mit dem von gregCal2 UND der Tag von gregCal1 ist kleiner als der von gregCal2
    // - und so weiter nach gleichem Prinzip
    // Daraus folgt folgender Ausdruck: (v = ODER; * = UND)
    // (Y) v (Y_M) v (Y_M_D) v (Y_M_D_h) v (Y_M_D_h_m) v (Y_M_D_h_m_s)
    // und nach Umformung:
    return (Y || (Y_ & (M || (M_ & (D || (D_ & (h || (h_ & (m || (m_ & s)))))))))); // WC: 21 Vergleiche: O(1)
}

bool GregorianCalendar::operator>(const GregorianCalendar &gregCal2) const
{
    // Definition: Ein Datum ist größer relativ zu einem anderen, wenn es einen späteren Zeitpunkt repräsentiert.

    // HILFSVARIABLEN

    // nur 'größer als'
    const bool Y{this->year > gregCal2.year};
    const bool M{this->month > gregCal2.month};
    const bool D{this->day > gregCal2.day};

    const bool h{this->hour > gregCal2.hour};
    const bool m{this->minute > gregCal2.minute};
    const bool s{this->sec > gregCal2.sec}; // nur größer relevant(!), da letzte Prüfungsinstanz wenn alle anderen 'gleich' sind

    // nur 'gleich'
    const bool Y_{this->year == gregCal2.year};
    const bool M_{this->month == gregCal2.month};
    const bool D_{this->day == gregCal2.day};

    const bool h_{this->hour == gregCal2.hour};
    const bool m_{this->minute == gregCal2.minute};

    // Damit gregCal1 > gregCal2 gilt, muss eine der folgenden Bedingungen erfüllt sein:
    // - Das Jahr von gregCal1 ist größer als das von gregCal2
    // - Das Jahr von gregCal1 ist gleich mit dem von gregCal2 UND der Monat von gregCal1 ist größer als der von gregCal2
    // - Das Jahr von gregCal1 ist gleich mit dem von gregCal2 UND der Monat von gregCal1 ist gleich mit dem von gregCal2 UND der Tag von gregCal1 ist größer als der von gregCal2
    // - und so weiter nach gleichem Prinzip
    // Daraus folgt folgender Ausdruck: (v = ODER; * = UND)
    // (Y) v (Y_M) v (Y_M_D) v (Y_M_D_h) v (Y_M_D_h_m) v (Y_M_D_h_m_s)
    // und nach Umformung:
    return (Y || (Y_ & (M || (M_ & (D || (D_ & (h || (h_ & (m || (m_ & s)))))))))); // WC: 21 Vergleiche: O(1)
}

bool GregorianCalendar::operator<=(const GregorianCalendar &gregCal2) const
{
    // Definition: Ein Datum ist kleiner gleich relativ zu einem anderen, wenn es einen früheren oder gleichen Zeitpunkt repräsentiert.

    // HILFSVARIABLEN

    // nur 'kleiner als'
    const bool Y{this->year < gregCal2.year};
    const bool M{this->month < gregCal2.month};
    const bool D{this->day < gregCal2.day};

    const bool h{this->hour < gregCal2.hour};
    const bool m{this->minute < gregCal2.minute};
    const bool s{this->sec < gregCal2.sec};

    // nur 'gleich'
    const bool Y_{this->year == gregCal2.year};
    const bool M_{this->month == gregCal2.month};
    const bool D_{this->day == gregCal2.day};

    const bool h_{this->hour == gregCal2.hour};
    const bool m_{this->minute == gregCal2.minute};
    const bool s_dot{this->sec <= gregCal2.sec}; // siehe Ausdrucksformel: Nur 'kleiner gleich' interessant, nicht seperat betrachten

    // Damit gregCal1 <= gregCal2 gilt, muss eine der folgenden Bedingungen erfüllt sein:
    // - Das Jahr von gregCal1 ist kleiner als das von gregCal2
    // - Das Jahr von gregCal1 ist gleich mit dem von gregCal2 UND der Monat von gregCal1 ist kleiner als der von gregCal2
    // - Das Jahr von gregCal1 ist gleich mit dem von gregCal2 UND der Monat von gregCal1 ist gleich mit dem von gregCal2 UND der Tag von gregCal1 ist kleiner als der von gregCal2
    // ... (und so weiter nach gleichem Prinzip)
    // - Das Jahr, Monat, Tag, Stunde, Minute, Sekunde von gregCal1 sind gleich mit denen von gregCal2
    // Daraus folgt folgender Ausdruck: (v = ODER; * = UND)
    // (Y) v (Y_M) v (Y_M_D) v (Y_M_D_h) v (Y_M_D_h_m) v (Y_M_D_h_m_s) v (Y_M_T_H_m_s_)
    // und nach Umformung:
    return (Y || (Y_ & (M || (M_ & (D || (D_ & (h || (h_ & (m || (m_ & s_dot)))))))))); // WC: 22 Vergleiche: O(1)
}

bool GregorianCalendar::operator>=(const GregorianCalendar &gregCal2) const
{
    // Definition: Ein Datum ist größer gleich relativ zu einem Anderen, wenn es einen früheren oder gleichen Zeitraum repräsentiert.

    // HILFSVARIABLEN

    // nur 'kleiner als'
    const bool Y{this->year > gregCal2.year};
    const bool M{this->month > gregCal2.month};
    const bool D{this->day > gregCal2.day};

    const bool h{this->hour > gregCal2.hour};
    const bool m{this->minute > gregCal2.minute};
    const bool s{this->sec > gregCal2.sec};

    // nur 'gleich'
    const bool Y_{this->year == gregCal2.year};
    const bool M_{this->month == gregCal2.month};
    const bool D_{this->day == gregCal2.day};

    const bool h_{this->hour == gregCal2.hour};
    const bool m_{this->minute == gregCal2.minute};
    const bool s_dot{this->sec >= gregCal2.sec}; // siehe Ausdrucksformel: Nur größer gleich interessant, nicht seperat betrachten

    // Damit gregCal1 >= gregCal2 gilt, muss eine der folgenden Bedingungen erfüllt sein:
    // - Das Jahr von gregCal1 ist größer als das von gregCal2
    // - Das Jahr von gregCal1 ist gleich mit dem von gregCal2 UND der Monat von gregCal1 ist größer als der von gregCal2
    // - Das Jahr von gregCal1 ist gleich mit dem von gregCal2 UND der Monat von gregCal1 ist gleich mit dem von gregCal2 UND der Tag von gregCal1 ist größer als der von gregCal2
    // ... (und so weiter nach gleichem Prinzip)
    // - Das Jahr, Monat, Tag, Stunde, Minute, Sekunde von gregCal1 sind gleich mit denen von gregCal2
    // Daraus folgt folgender Ausdruck: (v = ODER; * = UND)
    // (Y) v (Y_M) v (Y_M_D) v (Y_M_D_h) v (Y_M_D_h_m) v (Y_M_D_h_m_s) v (Y_M_T_H_m_s_)
    // und nach Umformung:
    return (Y || (Y_ & (M || (M_ & (D || (D_ & (h || (h_ & (m || (m_ & s_dot)))))))))); // WC: 22 Vergleiche: O(1)
}