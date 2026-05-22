#ifndef NAV1_ROUTE_H
#define NAV1_ROUTE_H

#include "nav1.h"

typedef struct {
    NAV1_Waypoint *waypoints;
    int count;
    int capacity;
    int currentIndex;
} NAV1_Route;

NAV1_EXPORT void NAV1_RTE_routeInit(NAV1_Route *route, NAV1_Waypoint *buffer, int capacity);
NAV1_EXPORT int  NAV1_RTE_routeAdd(NAV1_Route *route, double lat, double lon, double altFt, const char *ident, const char *name, int type);
NAV1_EXPORT int  NAV1_RTE_routeInsert(NAV1_Route *route, int index, double lat, double lon, double altFt, const char *ident, const char *name, int type);
NAV1_EXPORT int  NAV1_RTE_routeRemove(NAV1_Route *route, int index);
NAV1_EXPORT void NAV1_RTE_routeClear(NAV1_Route *route);
NAV1_EXPORT int  NAV1_RTE_routeSequence(NAV1_Route *route);
NAV1_EXPORT int  NAV1_RTE_routeDirectTo(NAV1_Route *route, int index);
NAV1_EXPORT int  NAV1_RTE_routeLegInfo(const NAV1_Route *route, int index, double *outBearingDeg, double *outDistNm);
NAV1_EXPORT int  NAV1_RTE_routeTurnAnticipation(const NAV1_Route *route, double groundSpeedKts, double turnRateDegS, double *outLat, double *outLon);
NAV1_EXPORT int  NAV1_RTE_routeSaveCSV(const char *filename, const NAV1_Route *route);
NAV1_EXPORT int  NAV1_RTE_routeLoadCSV(const char *filename, NAV1_Waypoint *buffer, int capacity, NAV1_Route *route);

#endif
