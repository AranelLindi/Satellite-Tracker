#include "contact/contact_time.h"

/* 
* BERECHNET KONTAKTZEITPUNKTE MIT SATELLITEN AUF BASIS EINES TLE
*/


// ############### CONTACT CALCULATION: CONDITION ###############
const constexpr double MIN_ELEVATION = M_PI / 36; // entspricht 5 Grad (gehört zur bool Funktion, deswegen nicht in Header!)
bool checkContact(double ele)
{
    if (ele >= MIN_ELEVATION)
    {
        return true;
    }
    else if (ele < MIN_ELEVATION)
    {
        return false;
    }
    return false;
}
// ##############################################################

int main(void) // Enthält beispielhafte Implementierung
{
    // Beispielhafter Aufruf einer Kontaktzeitberechnung:
    // Nötig:
    // - Condition um Verbindungsanforderung zu prüfen (siehe oben)
    // - TLE als Klasse Tle
    // - Start- und Endzeitpunkt

    const auto tles {readTlesFromFile("ISS.txt")};
    auto sonate {tles.at(25544)}; // select Sonate

    const GregorianCalendar ENDE {2020, 6, 16, 0, 0, 0};
    const GregorianCalendar START {2020, 6, 15, 0, 0, 0};

    bool (*condition)(double) = checkContact; // Funktionszeiger auf Condition-Function (prüft in DetermineContactTimes bei jeder Iteration ob eine Verbindung zu dem Satelliten besteht)

    DetermineContactTimes("Ausgabe.txt", sonate, START, ENDE, 0.275, 49.78, 9.97, condition);
}