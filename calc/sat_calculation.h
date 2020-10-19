// Standartbibliotheken

// eigener Code
#include "../Orbitpropagation/coordinates/geocoordinates.h"

struct SEZCoordinate
{
    const double rS; // Entfernung in Südrichtung [km]
    const double rE; // Entfernung in Ostrichtung [km]
    const double rZ; // Entfernung in Zenitrichtung [km]

    SEZCoordinate(double _rS, double _rE, double _rZ) noexcept : rS(_rS), rE(_rE), rZ(_rZ) {}
};

// Wandelt geodätische Koordinate zum Zeitpunkt jd in ECI Koordinate um. (Referenzellepsoid WGS84)
ECICoordinate convertGeodeticToECI(const GeodeticCoordinate &geodCoord, double jd) noexcept;

// Überführt Beobachtungsvektor von Beobachter Satellit in topozentrische Horizontsystem South-East-Zenith (SEZ)
SEZCoordinate transformECIToSEZ(const ECICoordinate &rObsSat, const GeodeticCoordinate &obs, double jd) noexcept;

// Berechnet Schrägentfernung zu Satelliten aus SEZ Koordinaten
double computeSlantRange(const SEZCoordinate &sezCoord) noexcept;

// Berechnet Azimutwinkel aus Koordinaten des topozentrischen Horizontsystems
double computeAzimuth(const SEZCoordinate &sezCoord) noexcept;

// Berechnet Elevationswinkel aus Koordinaten des topozentrischen Horizontsystems
double computeElevation(const SEZCoordinate &sezCoord) noexcept;