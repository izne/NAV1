#ifndef NAV1_GEO_H
#define NAV1_GEO_H

#include "nav1.h"

NAV1_EXPORT double NAV1_GEO_distanceToTarget(double lat1, double lon1, double lat2, double lon2);
NAV1_EXPORT double NAV1_GEO_headingToTarget(double lat1, double lon1, double lat2, double lon2);
NAV1_EXPORT void    NAV1_GEO_destinationPoint(double lat1, double lon1, double bearing, double distNm, double *outLat, double *outLon);
NAV1_EXPORT void    NAV1_GEO_midpoint(double lat1, double lon1, double lat2, double lon2, double *outLat, double *outLon);
NAV1_EXPORT double  NAV1_GEO_finalBearing(double lat1, double lon1, double lat2, double lon2);
NAV1_EXPORT double  NAV1_GEO_crossTrackDistance(double lat1, double lon1, double lat2, double lon2, double lat3, double lon3);
NAV1_EXPORT double  NAV1_GEO_dmsToDecimal(int degrees, int minutes, double seconds, char dir);
NAV1_EXPORT void    NAV1_GEO_decimalToDMS(double decimal, int *degrees, int *minutes, double *seconds);

NAV1_EXPORT void    NAV1_GEO_geodeticToEcef(double lat, double lon, double altM, double *x, double *y, double *z);
NAV1_EXPORT int    NAV1_GEO_ecefToGeodetic(double x, double y, double z, double *lat, double *lon, double *altM);

NAV1_EXPORT double NAV1_GEO_normalizeAngle(double a);
NAV1_EXPORT double NAV1_GEO_shortestAngularDistance(double from, double to);

NAV1_EXPORT int    NAV1_GEO_intersectingRadials(double lat1, double lon1, double brg1, double lat2, double lon2, double brg2, double *outLat, double *outLon);

#endif
