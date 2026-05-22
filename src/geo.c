#include "geo.h"
#include <math.h>

static const double R = 6371e3;

NAV1_EXPORT double NAV1_GEO_distanceToTarget(double lat1, double lon1, double lat2, double lon2)
{
    const double f1 = lat1 * M_PI / 180;
    const double f2 = lat2 * M_PI / 180;
    const double df = (lat2 - lat1) * M_PI / 180;
    const double dl = (lon2 - lon1) * M_PI / 180;
    const double a = sin(df / 2) * sin(df / 2) + cos(f1) * cos(f2) * sin(dl / 2) * sin(dl / 2);
    const double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c / 1852.0;
}

NAV1_EXPORT double NAV1_GEO_headingToTarget(double lat1, double lon1, double lat2, double lon2)
{
    const double lat1_r = lat1 * M_PI / 180;
    const double lat2_r = lat2 * M_PI / 180;
    const double lon1_r = lon1 * M_PI / 180;
    const double lon2_r = lon2 * M_PI / 180;

    const double y = sin(lon2_r - lon1_r) * cos(lat2_r);
    const double x = cos(lat1_r) * sin(lat2_r) - sin(lat1_r) * cos(lat2_r) * cos(lon2_r - lon1_r);
    const double t = atan2(y, x);
    const double bearing = t * 180.0 / M_PI;

    return fmod(bearing + 360.0, 360.0);
}

NAV1_EXPORT void NAV1_GEO_destinationPoint(double lat1, double lon1, double bearing, double distNm, double *outLat, double *outLon)
{
    const double lat1_r = lat1 * M_PI / 180;
    const double lon1_r = lon1 * M_PI / 180;
    const double brng_r = bearing * M_PI / 180;
    const double d = distNm * 1852.0;
    const double dR = d / R;

    const double lat2_r = asin(sin(lat1_r) * cos(dR) + cos(lat1_r) * sin(dR) * cos(brng_r));
    const double lon2_r = lon1_r + atan2(sin(brng_r) * sin(dR) * cos(lat1_r), cos(dR) - sin(lat1_r) * sin(lat2_r));

    *outLat = lat2_r * 180 / M_PI;
    *outLon = fmod(lon2_r * 180 / M_PI + 540, 360) - 180;
}

NAV1_EXPORT void NAV1_GEO_midpoint(double lat1, double lon1, double lat2, double lon2, double *outLat, double *outLon)
{
    const double lat1_r = lat1 * M_PI / 180;
    const double lat2_r = lat2 * M_PI / 180;
    const double lon1_r = lon1 * M_PI / 180;
    const double lon2_r = lon2 * M_PI / 180;
    const double dl = lon2_r - lon1_r;

    const double Bx = cos(lat2_r) * cos(dl);
    const double By = cos(lat2_r) * sin(dl);

    const double lat3_r = atan2(sin(lat1_r) + sin(lat2_r), sqrt((cos(lat1_r) + Bx) * (cos(lat1_r) + Bx) + By * By));
    const double lon3_r = lon1_r + atan2(By, cos(lat1_r) + Bx);

    *outLat = lat3_r * 180 / M_PI;
    *outLon = fmod(lon3_r * 180 / M_PI + 540, 360) - 180;
}

NAV1_EXPORT double NAV1_GEO_finalBearing(double lat1, double lon1, double lat2, double lon2)
{
    const double lat1_r = lat1 * M_PI / 180;
    const double lat2_r = lat2 * M_PI / 180;
    const double lon1_r = lon1 * M_PI / 180;
    const double lon2_r = lon2 * M_PI / 180;

    const double y = sin(lon1_r - lon2_r) * cos(lat1_r);
    const double x = cos(lat2_r) * sin(lat1_r) - sin(lat2_r) * cos(lat1_r) * cos(lon1_r - lon2_r);
    const double t = atan2(y, x);

    return fmod(t * 180 / M_PI + 540, 360);
}

NAV1_EXPORT double NAV1_GEO_crossTrackDistance(double lat1, double lon1, double lat2, double lon2, double lat3, double lon3)
{
    const double lat1_r = lat1 * M_PI / 180;
    const double lon1_r = lon1 * M_PI / 180;
    const double lat2_r = lat2 * M_PI / 180;
    const double lon2_r = lon2 * M_PI / 180;
    const double lat3_r = lat3 * M_PI / 180;
    const double lon3_r = lon3 * M_PI / 180;

    const double df_13 = (lat3 - lat1) * M_PI / 180;
    const double dl_13 = (lon3 - lon1) * M_PI / 180;
    const double a13 = sin(df_13 / 2) * sin(df_13 / 2) + cos(lat1_r) * cos(lat3_r) * sin(dl_13 / 2) * sin(dl_13 / 2);
    const double c13 = 2 * atan2(sqrt(a13), sqrt(1 - a13));

    const double y13 = sin(lon3_r - lon1_r) * cos(lat3_r);
    const double x13 = cos(lat1_r) * sin(lat3_r) - sin(lat1_r) * cos(lat3_r) * cos(lon3_r - lon1_r);
    const double brng13 = atan2(y13, x13);

    const double y12 = sin(lon2_r - lon1_r) * cos(lat2_r);
    const double x12 = cos(lat1_r) * sin(lat2_r) - sin(lat1_r) * cos(lat2_r) * cos(lon2_r - lon1_r);
    const double brng12 = atan2(y12, x12);

    double arg = sin(c13) * sin(brng13 - brng12);
    if (arg > 1.0) arg = 1.0;
    if (arg < -1.0) arg = -1.0;
    const double xtd = asin(arg) * R;

    return xtd / 1852.0;
}

NAV1_EXPORT double NAV1_GEO_dmsToDecimal(int degrees, int minutes, double seconds, char dir)
{
    double decimal = abs(degrees) + minutes / 60.0 + seconds / 3600.0;
    if (dir == 'S' || dir == 'W' || dir == 's' || dir == 'w')
        decimal = -decimal;
    return decimal;
}

NAV1_EXPORT void NAV1_GEO_decimalToDMS(double decimal, int *degrees, int *minutes, double *seconds)
{
    double absDec = fabs(decimal);
    *degrees = (int)absDec;
    double rem = (absDec - *degrees) * 60;
    *minutes = (int)rem;
    *seconds = (rem - *minutes) * 60;
}
