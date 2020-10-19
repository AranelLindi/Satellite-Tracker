#include "sat_calculation.h"

SEZCoordinate transformECIToSEZ(const ECICoordinate &rObsSat, const GeodeticCoordinate &obs, double jd) noexcept
{
    /*const double gmst = Calendar::computeGMST(jd) + obs.longitude; // Sternzeit des Beobachters hängt ab vom Längengrad!

    // Aus Vorlesung RFBA_4, Folie 7:
    // Zur Transformation in das Horizontsystem muss der Vektor um den
    // Winkel Θ (Sternzeit des Beobachtungsortes, Rotation um die Erdachse Z)
    // und um den Winkel φ (Breitengrad des Beobachters) gedreht werden.

    // Süd-Richtung:
    const double r_S = sin(obs.latitude) * cos(gmst) * rObsSat.x +
                       sin(obs.latitude) * sin(gmst) * rObsSat.y -
                       cos(obs.latitude) * rObsSat.z;

    // Ost-Richtung:
    const double r_E = -sin(gmst) * rObsSat.x +
                       cos(gmst) * rObsSat.y;

    // Zenit-Richtung:
    const double r_Z = cos(obs.latitude) * cos(gmst) * rObsSat.x +
                       cos(obs.latitude) * sin(gmst) * rObsSat.y +
                       sin(obs.latitude) * rObsSat.z;

    SEZCoordinate sz;

    sz.rE = r_E;
    sz.rS = r_S;
    sz.rZ = r_Z;

    return sz;*/

    const double sinPhi{sin(obs.latitude)};
    const double cosPhi{cos(obs.latitude)};

    const double gmst{Calendar::computeGMST(jd)};
    const double sinTheta{sin(obs.longitude + gmst)};
    const double cosTheta{cos(obs.longitude + gmst)};

    return SEZCoordinate(sinPhi * cosTheta * rObsSat.x + sinPhi * sinTheta * rObsSat.y - cosPhi * rObsSat.z,
                         -sinTheta * rObsSat.x + cosTheta * rObsSat.y,
                         cosPhi * cosTheta * rObsSat.x + cosPhi * sinTheta * rObsSat.y + sinPhi * rObsSat.z);
}

ECICoordinate convertGeodeticToECI(const GeodeticCoordinate &geodCoord, double jd) noexcept
{
    /*// große Halbachse:
    static const float a = 6378137.0 / 1000; // [km]
    // Abplattung:
    static const float f = 1 / 298.257223563; // Quelle: https://www.unoosa.org/pdf/icg/2012/template/WGS_84.pdf

    // Sternzeit am Beobachtungsort mittels JD:
    const double theta = Calendar::computeGMST(jd) + geodCoord.longitude; // Sternzeit Bodenstation hängt ab vom Längengrad der Bodenstation!

    // Breite:
    const double phi = geodCoord.latitude; // Hilfsvariable

    // Lambda: Potenz 2
    const auto sq = [](double i) -> double { return (i * i); };

    const double C = 1 / (sqrt(1 + f * (f - 2) * sq(sin(phi))));
    const double S = sq(1 - f) * C;

    // neues ECICoordinate Objekt anlegen und Member befüllen:
    // (ECI Koordinaten des Beobachters auf Ellipsoid)
    ECICoordinate eci;
    eci.x = (a * C + geodCoord.height) * cos(phi) * cos(theta);
    eci.y = (a * C + geodCoord.height) * cos(phi) * sin(theta);
    eci.z = (a * S + geodCoord.height) * sin(phi);

    return eci;*/

    const float Re{6378.137f};
    const float f{1.0 / 298.26};

    const double sinPhi{sin(geodCoord.latitude)};
    const double cosPhi{cos(geodCoord.latitude)};

    const double gmst{Calendar::computeGMST(jd)};
    const double sinTheta{sin(geodCoord.longitude + gmst)};
    const double cosTheta{cos(geodCoord.longitude + gmst)};

    const double C{1 / sqrt(1 + f * (f - 2) * sinPhi * sinPhi)};
    const double S{(1 - f) * (1 - f) * C};

    // Include height of geodetic coordinate into equations from RFBA_4.pdf, S.9
    ECICoordinate coord;
    coord.x = (Re * C + geodCoord.height) * cosPhi * cosTheta;
    coord.y = (Re * C + geodCoord.height) * cosPhi * sinTheta;
    coord.z = (Re * S + geodCoord.height) * sinPhi;

    return coord;
}

double computeSlantRange(const SEZCoordinate &sezCoord) noexcept
{
    const auto sq = [](double i) -> double { return (i * i); };

    return sqrt(sq(sezCoord.rS) + sq(sezCoord.rE) + sq(sezCoord.rZ));
}

double computeAzimuth(const SEZCoordinate &sezCoord) noexcept
{
    // von Norden im Uhrzeigersinn positiv, d.h. im Norden beträgt
    // der Azimut 0 Grad. Wertebereich liegt im Intervall [0, 2pi)

    //if (sezCoord.rS == 0)
    //    return 0;                                   // eventuelle Division durch 0 abfangen
    const double azimuth{atan2(sezCoord.rE, -sezCoord.rS)}; // arctan(x)

    return ((azimuth < 0) ? (azimuth + (2 * M_PI)) : (azimuth));
}

double computeElevation(const SEZCoordinate &sezCoord) noexcept
{
    // Elevationswinkel von 0 Grad entspricht Horizont und Winkel von 90 Grad
    // dem Zenit. Wertebereich liegt im Intervall [-pi/2, pi/2]

    // Elevationswinkel in [rad] berechnen:
    return asin(sezCoord.rZ / computeSlantRange(sezCoord)); // r > 0, arcsin(x)
}