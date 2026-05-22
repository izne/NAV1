#ifndef NAV1_NAV_H
#define NAV1_NAV_H

#include "nav1.h"

NAV_EXPORT double NAV1_NAV_alongTrackDistance(double lat1, double lon1, double lat2, double lon2, double lat3, double lon3);
NAV_EXPORT void NAV1_NAV_greatCircleWaypoints(double lat1, double lon1, double lat2, double lon2, int count, double *outLats, double *outLons);
NAV_EXPORT double NAV1_NAV_rhumbDistance(double lat1, double lon1, double lat2, double lon2);
NAV_EXPORT double NAV1_NAV_rhumbBearing(double lat1, double lon1, double lat2, double lon2);
NAV_EXPORT void NAV1_NAV_antipodalPoint(double lat, double lon, double *outLat, double *outLon);
NAV_EXPORT void NAV1_NAV_bearingToCompass(double bearing, char *out, int outSize);

#endif
