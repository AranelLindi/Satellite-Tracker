// Standartbibliotheken

// eigener Code
#include "Orbitpropagation/coordinates/geocoordinates.h"

struct SEZCoordinate {
    double rS; // Entfernung in Südrichtung [km]
    double rE; // Entfernung in Ostrichtung [km]
    double rZ; // Entfernung in Zenitrichtung [km]
};


// Wandelt geodätische Koordinate zum Zeitpunkt jd in ECI Koordinate um. (Referenzellepsoid WGS84)
ECICoordinate convertGeodeticToECI(const GeodeticCoordinate &geodCoord, double jd);

// Überführt Beobachtungsvektor von Beobachter Satellit in topozentrische Horizontsystem South-East-Zenith (SEZ)
SEZCoordinate transformECIToSEZ(const ECICoordinate& rObsSat, const GeodeticCoordinate& obs, double jd);

// Berechnet Schrägentfernung zu Satelliten aus SEZ Koordinaten 
double computeSlantRange(const SEZCoordinate& sezCoord);

// Berechnet Azimutwinkel aus Koordinaten des topozentrischen Horizontsystems
double computeAzimuth(const SEZCoordinate &sezCoord);

// Berechnet Elevationswinkel aus Koordinaten des topozentrischen Horizontsystems
double computeElevation(const SEZCoordinate &sezCoord);