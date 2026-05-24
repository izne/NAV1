#ifndef NAV1_NAV_H
#define NAV1_NAV_H

#include "nav1.h"

#define NAV1_NAV_HOLD_DIRECT    0
#define NAV1_NAV_HOLD_PARALLEL  1
#define NAV1_NAV_HOLD_TEARDROP  2

NAV1_EXPORT double NAV1_NAV_alongTrackDistance(double lat1, double lon1, double lat2, double lon2, double lat3, double lon3);
NAV1_EXPORT void NAV1_NAV_greatCircleWaypoints(double lat1, double lon1, double lat2, double lon2, int count, double *outLats, double *outLons);
NAV1_EXPORT double NAV1_NAV_rhumbDistance(double lat1, double lon1, double lat2, double lon2);
NAV1_EXPORT double NAV1_NAV_rhumbBearing(double lat1, double lon1, double lat2, double lon2);
NAV1_EXPORT void NAV1_NAV_antipodalPoint(double lat, double lon, double *outLat, double *outLon);
NAV1_EXPORT void NAV1_NAV_bearingToCompass(double bearing, char *out, int outSize);

NAV1_EXPORT int  NAV1_NAV_closestPointOnCourse(double lat1, double lon1, double lat2, double lon2, double lat3, double lon3, double *outLat, double *outLon);

NAV1_EXPORT void NAV1_NAV_leadPoint(double lat, double lon, double outboundCourse, double radiusNm, double interceptCourse, double *outLat, double *outLon);
NAV1_EXPORT int  NAV1_NAV_courseIntercept(double lat, double lon, double heading, double targetLat, double targetLon, double targetCourse, double interceptAngle, double *outLat, double *outLon);
NAV1_EXPORT int  NAV1_NAV_holdEntryMode(double inboundCourse, double headingToFix, int rightTurn);

NAV1_EXPORT void NAV1_NAV_fixRadialDistance(double vorLat, double vorLon, double radialDeg, double distNm, double *outLat, double *outLon);

#endif
