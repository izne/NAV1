#ifndef NAV1_ROUTE_H
#define NAV1_ROUTE_H

#include "nav1.h"

typedef struct {
    NAV_Sup_Waypoint *waypoints;
    int count;
    int capacity;
    int currentIndex;
} NAV_Sup_Route;

NAV_EXPORT void NAV_SUP_routeInit(NAV_Sup_Route *route, NAV_Sup_Waypoint *buffer, int capacity);
NAV_EXPORT int  NAV_SUP_routeAdd(NAV_Sup_Route *route, double lat, double lon, double altFt, const char *ident, const char *name, int type);
NAV_EXPORT int  NAV_SUP_routeInsert(NAV_Sup_Route *route, int index, double lat, double lon, double altFt, const char *ident, const char *name, int type);
NAV_EXPORT int  NAV_SUP_routeRemove(NAV_Sup_Route *route, int index);
NAV_EXPORT void NAV_SUP_routeClear(NAV_Sup_Route *route);
NAV_EXPORT int  NAV_SUP_routeSequence(NAV_Sup_Route *route);
NAV_EXPORT int  NAV_SUP_routeDirectTo(NAV_Sup_Route *route, int index);
NAV_EXPORT int  NAV_SUP_routeLegInfo(const NAV_Sup_Route *route, int index, double *outBearingDeg, double *outDistNm);
NAV_EXPORT int  NAV_SUP_routeTurnAnticipation(const NAV_Sup_Route *route, double groundSpeedKts, double turnRateDegS, double *outLat, double *outLon);
NAV_EXPORT int  NAV_SUP_routeSaveCSV(const char *filename, const NAV_Sup_Route *route);
NAV_EXPORT int  NAV_SUP_routeLoadCSV(const char *filename, NAV_Sup_Waypoint *buffer, int capacity, NAV_Sup_Route *route);

#endif
