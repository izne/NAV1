#include "nav.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

static const double R = 6371e3;

static double toRad(double deg) { return deg * M_PI / 180; }
static double toDeg(double rad) { return rad * 180 / M_PI; }

static double initialBearing(double lat1, double lon1, double lat2, double lon2)
{
    const double lat1_r = toRad(lat1);
    const double lat2_r = toRad(lat2);
    const double lon1_r = toRad(lon1);
    const double lon2_r = toRad(lon2);
    const double y = sin(lon2_r - lon1_r) * cos(lat2_r);
    const double x = cos(lat1_r) * sin(lat2_r) - sin(lat1_r) * cos(lat2_r) * cos(lon2_r - lon1_r);
    return atan2(y, x);
}

static double haversineAngular(double lat1, double lon1, double lat2, double lon2)
{
    const double lat1_r = toRad(lat1);
    const double lat2_r = toRad(lat2);
    const double df = toRad(lat2 - lat1);
    const double dl = toRad(lon2 - lon1);
    const double a = sin(df / 2) * sin(df / 2) + cos(lat1_r) * cos(lat2_r) * sin(dl / 2) * sin(dl / 2);
    return 2 * atan2(sqrt(a), sqrt(1 - a));
}

NAV_EXPORT double NAV_SUP_alongTrackDistance(double lat1, double lon1, double lat2, double lon2, double lat3, double lon3)
{
    const double c13 = haversineAngular(lat1, lon1, lat3, lon3);
    const double brng13 = initialBearing(lat1, lon1, lat3, lon3);
    const double brng12 = initialBearing(lat1, lon1, lat2, lon2);
    const double dat = atan2(sin(c13) * cos(brng13 - brng12), cos(c13));
    return dat * R / 1852.0;
}

NAV_EXPORT void NAV_SUP_greatCircleWaypoints(double lat1, double lon1, double lat2, double lon2, int count, double *outLats, double *outLons)
{
    if (count < 2) return;
    outLats[0] = lat1;
    outLons[0] = lon1;
    if (count == 2)
    {
        outLats[1] = lat2;
        outLons[1] = lon2;
        return;
    }
    outLats[count - 1] = lat2;
    outLons[count - 1] = lon2;

    const double lat1_r = toRad(lat1);
    const double lon1_r = toRad(lon1);
    const double lat2_r = toRad(lat2);
    const double lon2_r = toRad(lon2);
    const double c = haversineAngular(lat1, lon1, lat2, lon2);
    const double sinC = sin(c);

    for (int i = 1; i < count - 1; i++)
    {
        const double f = (double)i / (count - 1);
        const double A = sin((1 - f) * c) / sinC;
        const double B = sin(f * c) / sinC;
        const double x = A * cos(lat1_r) * cos(lon1_r) + B * cos(lat2_r) * cos(lon2_r);
        const double y = A * cos(lat1_r) * sin(lon1_r) + B * cos(lat2_r) * sin(lon2_r);
        const double z = A * sin(lat1_r) + B * sin(lat2_r);
        outLats[i] = toDeg(atan2(z, sqrt(x * x + y * y)));
        outLons[i] = toDeg(atan2(y, x));
    }
}

NAV_EXPORT double NAV_SUP_rhumbDistance(double lat1, double lon1, double lat2, double lon2)
{
    const double lat1_r = toRad(lat1);
    const double lat2_r = toRad(lat2);
    const double dlat = lat2_r - lat1_r;
    const double dlon = toRad(lon2 - lon1);
    const double q = log(tan(lat2_r / 2 + M_PI / 4) / tan(lat1_r / 2 + M_PI / 4));
    const double qq = fabs(dlat) < 1e-12 ? cos(lat1_r) : dlat / q;
    return R * sqrt(dlat * dlat + qq * qq * dlon * dlon) / 1852.0;
}

NAV_EXPORT double NAV_SUP_rhumbBearing(double lat1, double lon1, double lat2, double lon2)
{
    const double lat1_r = toRad(lat1);
    const double lat2_r = toRad(lat2);
    const double dlon = toRad(lon2 - lon1);
    if (fabs(lat1 - lat2) < 1e-12)
        return dlon > 0 ? 90.0 : 270.0;
    const double y = dlon;
    const double x = log(tan(lat2_r / 2 + M_PI / 4) / tan(lat1_r / 2 + M_PI / 4));
    double t = atan2(y, x);
    return fmod(toDeg(t) + 360, 360);
}

NAV_EXPORT void NAV_SUP_antipodalPoint(double lat, double lon, double *outLat, double *outLon)
{
    *outLat = -lat;
    *outLon = lon + 180.0;
    if (*outLon > 180.0) *outLon -= 360.0;
}

NAV_EXPORT void NAV_SUP_bearingToCompass(double bearing, char *out, int outSize)
{
    const char *points[] = {
        "N", "NbE", "NNE", "NEbN", "NE", "NEbE", "ENE", "EbN",
        "E", "EbS", "ESE", "SEbE", "SE", "SEbS", "SSE", "SbE",
        "S", "SbW", "SSW", "SWbS", "SW", "SWbW", "WSW", "WbS",
        "W", "WbN", "WNW", "NWbW", "NW", "NWbN", "NNW", "NbW"
    };
    double idx = fmod(bearing + 360.0, 360.0);
    idx = (idx + 11.25 / 2.0) / 360.0 * 32;
    int i = (int)idx % 32;
    snprintf(out, outSize, "%s", points[i]);
}
