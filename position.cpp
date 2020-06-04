#include "position.h" // HEADER

#define INTERVAL 30 // [s] (Overloading increment-Operator!) Gibt an, um wieviele Sekunden ein GregorianCalendar-Objekt inkrementiert werden soll


// Operator != definieren, sodass zwei GregorianCalender bequem miteinander verglichen werden können
bool operator!=(const GregorianCalendar &gregCal1, const GregorianCalendar &gregCal2)
{
    return !((gregCal1.year == gregCal2.year) &&
             (gregCal1.month == gregCal2.month) &&
             (gregCal1.day == gregCal2.day) &&
             (gregCal1.hour == gregCal2.hour) &&
             (gregCal1.minute == gregCal2.minute) &&
             (gregCal1.sec == gregCal2.sec));
}

// Operator ++ definieren, sodass ein GregorianCalender, bequem um 30 Sekunden inkrementiert werden kann
GregorianCalendar &operator++(GregorianCalendar &temp, int)
{
    // Prüfen ob eine Variable ihren Definitionsbereich überschreiten würde, dann entsprechend
    // die höherhierarchischeren Variablen inkrementieren. Dies funktioniert, bis ein neuer
    // Tag beginnen würde, denn dort müsste dann wieder der gesamte Kalender mit eingebunden werden
    // (z.b Schalttagprüfungen etc)
    // ACHTUNG: Es darf maximal um 60 Sekunden inkrementiert werden!

    if ((temp.sec + INTERVAL) >= 60)
    {
        if ((temp.minute + 1) >= 60)
        {
            if ((temp.hour + 1) >= 24)
            {
                throw "Neuer Tag, korrektes Datum nicht sichergestellt!";
            }
        }
        else
        {
            temp.minute++;
            temp.sec = (temp.sec + INTERVAL - 60);
        }
    }
    else
    {
        temp.sec += INTERVAL;
    }

    return temp;
}

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
    myfile.open("Ausgabe.txt");

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

        ctime++; // Inkrementiert Pos um 30 Sekunden (Operator Overloading!)

        counter++; // Counter inkrementieren (Datzsätze)
    }

    std::cout << "Datensätze: " << counter << '\n';

    // Am Ende einmal flushen:
    myfile << std::flush;

    // Fertig
    myfile.close();
}