#ifndef NAV1_GPS_H
#define NAV1_GPS_H

#include "nav1.h"

NAV_EXPORT double NAV1_GPS_dmmToDecimal(int degrees, double minutes, char dir);
NAV_EXPORT void NAV1_GPS_decimalToDMM(double decimal, double *minutes, char *dir);
NAV_EXPORT double NAV1_GPS_gpsSpeed(double lat1, double lon1, double lat2, double lon2, double timeSeconds);
NAV_EXPORT int NAV1_GPS_nmeaChecksum(const char *sentence);
NAV_EXPORT int NAV1_GPS_isValidLatitude(double lat);
NAV_EXPORT int NAV1_GPS_isValidLongitude(double lon);

#endif
