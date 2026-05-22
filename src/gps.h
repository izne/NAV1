#ifndef NAV1_GPS_H
#define NAV1_GPS_H

#include "nav1.h"

NAV_EXPORT double NAV_SUP_dmmToDecimal(int degrees, double minutes, char dir);
NAV_EXPORT void NAV_SUP_decimalToDMM(double decimal, double *minutes, char *dir);
NAV_EXPORT double NAV_SUP_gpsSpeed(double lat1, double lon1, double lat2, double lon2, double timeSeconds);
NAV_EXPORT int NAV_SUP_nmeaChecksum(const char *sentence);
NAV_EXPORT int NAV_SUP_isValidLatitude(double lat);
NAV_EXPORT int NAV_SUP_isValidLongitude(double lon);

#endif
