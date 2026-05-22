#ifndef NAV1_GEO_H
#define NAV1_GEO_H

#include "nav1.h"

NAV_EXPORT double NAV1_GEO_distanceToTarget(double lat1, double lon1, double lat2, double lon2);
NAV_EXPORT double NAV1_GEO_headingToTarget(double lat1, double lon1, double lat2, double lon2);
NAV_EXPORT void    NAV1_GEO_destinationPoint(double lat1, double lon1, double bearing, double distNm, double *outLat, double *outLon);
NAV_EXPORT void    NAV1_GEO_midpoint(double lat1, double lon1, double lat2, double lon2, double *outLat, double *outLon);
NAV_EXPORT double  NAV1_GEO_finalBearing(double lat1, double lon1, double lat2, double lon2);
NAV_EXPORT double  NAV1_GEO_crossTrackDistance(double lat1, double lon1, double lat2, double lon2, double lat3, double lon3);
NAV_EXPORT double  NAV1_GEO_dmsToDecimal(int degrees, int minutes, double seconds, char dir);
NAV_EXPORT void    NAV1_GEO_decimalToDMS(double decimal, int *degrees, int *minutes, double *seconds);

#endif
