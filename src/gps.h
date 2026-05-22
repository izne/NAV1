#ifndef NAV1_GPS_H
#define NAV1_GPS_H

#include "nav1.h"

NAV1_EXPORT double NAV1_GPS_dmmToDecimal(int degrees, double minutes, char dir);
NAV1_EXPORT void NAV1_GPS_decimalToDMM(double decimal, double *minutes, char *dir);
NAV1_EXPORT double NAV1_GPS_gpsSpeed(double lat1, double lon1, double lat2, double lon2, double timeSeconds);
NAV1_EXPORT int NAV1_GPS_nmeaChecksum(const char *sentence);
NAV1_EXPORT int NAV1_GPS_isValidLatitude(double lat);
NAV1_EXPORT int NAV1_GPS_isValidLongitude(double lon);

#endif
