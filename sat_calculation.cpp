#include "sat_calculation.h"

SEZCoordinate transformECIToSEZ(const ECICoordinate &rObsSatz, const GeodeticCoordinate &obs, double jd)
{
    double r_S = 0, r_E = 0, r_Z = 0;
    double gmst = Calendar::computeGMST(jd);

    // Aus Vorlesung RFBA_4, Folie 7:
    // Zur Transformation in das Horizontsystem muss der Vektor um den
    // Winkel Θ (Sternzeit des Beobachtungsortes, Rotation um die Erdachse Z)
    // und um den Winkel φ (Breitengrad des Beobachters) gedreht werden.

    // Süd-Richtung:
    r_S = sin(obs.latitude) * cos(gmst) * rObsSatz.x +
          sin(obs.latitude) * sin(gmst) * rObsSatz.y -
          cos(obs.latitude) * rObsSatz.z;

    // Ost-Richtung:
    r_E = -sin(gmst) * rObsSatz.x +
          cos(gmst) * rObsSatz.y;

    // Zenit-Richtung:
    r_Z = cos(obs.latitude) * cos(gmst) * rObsSatz.x +
          cos(obs.latitude) * sin(gmst) * rObsSatz.y +
          sin(obs.latitude) * rObsSatz.z;

    SEZCoordinate sz;

    sz.rE = r_E;
    sz.rS = r_S;
    sz.rZ = r_Z;

    return sz;
}

ECICoordinate convertGeodeticToECI(const GeodeticCoordinate &geodCoord, double jd)
{
    // große Halbachse:
    const double a = 6378137.0 / 1000;  // [km]
    // Abplattung:
    const double f = 1 / 298.257223563; // Quelle: https://www.unoosa.org/pdf/icg/2012/template/WGS_84.pdf

    // Sternzeit am Beobachtungsort mittels JD:
    const double gmst = Calendar::computeGMST(jd);

    // Breite:
    double phi = geodCoord.latitude; // Hilfsvariable

    // Lambda: Potenz 2
    auto sq = [](double i) { return i * i; };

    double C = 1 / (sqrt(1 + f * (f - 2) * sq(sin(phi))));
    double S = sq(1 - f) * C;

    // neues ECICoordinate Objekt anlegen und Member befüllen:
    // (ECI Koordinaten des Beobachters auf Ellipsoid)
    ECICoordinate eci;
    eci.x = a * C * cos(phi) * cos(gmst);
    eci.y = a * C * cos(phi) * sin(gmst);
    eci.z = a * S * sin(phi);

    return eci;
}

double computeSlantRange(const SEZCoordinate &sezCoord)
{
    const auto sq = [](double i) -> double { return i * i; };

    return sqrt(sq(sezCoord.rS) + sq(sezCoord.rE) + sq(sezCoord.rZ));
}

double computeAzimuth(const SEZCoordinate &sezCoord)
{
    // von Norden im Uhrzeigersinn positiv, d.h. im Norden beträgt
    // der Azimut 0 Grad. Wertebereich liegt im Intervall [0, 2pi)

    double azimuth_deg = 0, azimuth_rad = 0;

    if (sezCoord.rS == 0)
        return 0;                                   // eventuelle Division durch 0 abfangen
    azimuth_deg = atan(-sezCoord.rE / sezCoord.rS); // arctan(x)

    // Winkel auf das entsprechende Intervall abbilden:
    azimuth_rad = M_PI / 180 * azimuth_deg;

    // reicht da eine Iteration?
    if (azimuth_rad >= M_PI)
        azimuth_rad -= 2 * M_PI;
    if (azimuth_rad < 0)
        azimuth_rad += 2 * M_PI;

    return azimuth_rad;
}

double computeElevation(const SEZCoordinate &sezCoord)
{
    // Elevationswinkel von 0 Grad entspricht Horizont und Winkel von 90 Grad
    // dem Zenit. Wertebereich liegt im Intervall [-pi/2, pi/2]

    double elevation_deg = 0, elevation_rad = 0;
    double r = 0; // Schrägentfernung Satellit

    // Schrägentfernung Satellit
    r = computeSlantRange(sezCoord);

    // Elevationswinkel in Grad berechnen:
    elevation_deg = asin(sezCoord.rZ / r); // r > 0, arcsin(x)

    // Winkel auf das entsprechende Intervall abbilden:
    elevation_rad = M_PI / 180 * elevation_deg;

    // reicht da eine Iteration?
    if (elevation_rad >= M_PI / 2)
        elevation_rad -= M_PI;
    if (elevation_rad <= -M_PI / 2)
        elevation_rad += M_PI;

    return elevation_rad;
}