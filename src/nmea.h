#ifndef NAV1_NMEA_H
#define NAV1_NMEA_H

#include "dll.h"

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
} NAV_Sup_NMEAData;

NAV_EXPORT int NAV_SUP_nmeaParse(const char *sentence, NAV_Sup_NMEAData *out);

#endif
