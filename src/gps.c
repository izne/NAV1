#include "gps.h"
#include "nav1.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

NAV_EXPORT double NAV1_GPS_dmmToDecimal(int degrees, double minutes, char dir)
{
    double decimal = abs(degrees) + minutes / 60.0;
    if (dir == 'S' || dir == 'W' || dir == 's' || dir == 'w')
        decimal = -decimal;
    return decimal;
}

NAV_EXPORT void NAV1_GPS_decimalToDMM(double decimal, double *minutes, char *dir)
{
    double absDec = fabs(decimal);
    *minutes = (absDec - (int)absDec) * 60.0;
    *dir = (decimal >= 0) ? 'N' : 'S';
}

NAV_EXPORT double NAV1_GPS_gpsSpeed(double lat1, double lon1, double lat2, double lon2, double timeSeconds)
{
    if (timeSeconds <= 0) return 0.0;
    double distNm = NAV1_GEO_distanceToTarget(lat1, lon1, lat2, lon2);
    double timeHours = timeSeconds / 3600.0;
    return distNm / timeHours;
}

NAV_EXPORT int NAV1_GPS_nmeaChecksum(const char *sentence)
{
    const char *star, *p;
    unsigned char cs;
    if (!sentence || sentence[0] != '$') return 0;
    star = strchr(sentence, '*');
    if (!star || strlen(star) < 3) return 0;

    cs = 0;
    for (p = sentence + 1; p < star; p++)
        cs ^= (unsigned char)*p;

    unsigned int expected;
    if (sscanf(star + 1, "%2x", &expected) != 1)
        return 0;

    return cs == expected;
}

NAV_EXPORT int NAV1_GPS_isValidLatitude(double lat)
{
    return lat >= -90.0 && lat <= 90.0;
}

NAV_EXPORT int NAV1_GPS_isValidLongitude(double lon)
{
    return lon >= -180.0 && lon <= 180.0;
}
