#ifndef NAV1_NAV_H
#define NAV1_NAV_H

#include "nav1.h"

NAV1_EXPORT double NAV1_NAV_alongTrackDistance(double lat1, double lon1, double lat2, double lon2, double lat3, double lon3);
NAV1_EXPORT void NAV1_NAV_greatCircleWaypoints(double lat1, double lon1, double lat2, double lon2, int count, double *outLats, double *outLons);
NAV1_EXPORT double NAV1_NAV_rhumbDistance(double lat1, double lon1, double lat2, double lon2);
NAV1_EXPORT double NAV1_NAV_rhumbBearing(double lat1, double lon1, double lat2, double lon2);
NAV1_EXPORT void NAV1_NAV_antipodalPoint(double lat, double lon, double *outLat, double *outLon);
NAV1_EXPORT void NAV1_NAV_bearingToCompass(double bearing, char *out, int outSize);

NAV1_EXPORT int  NAV1_NAV_closestPointOnCourse(double lat1, double lon1, double lat2, double lon2, double lat3, double lon3, double *outLat, double *outLon);

#endif
