#ifndef NAV1_NMEA_H
#define NAV1_NMEA_H

#include "nav1.h"

typedef struct {
    int    hasFix;
    double latitude;
    double longitude;
    double altitudeM;
    double geoidSepM;
    int    fixQuality;
    int    numSats;
    double hdop;
    double speedKts;
    double trackDeg;
    double magVarDeg;
    char   timeUTC[10];
    char   dateUTC[7];
} NAV1_NMEAData;

NAV1_EXPORT int NAV1_NMEA_nmeaParse(const char *sentence, NAV1_NMEAData *out);

#endif
