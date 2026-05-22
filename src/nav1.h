#ifndef NAV1_H
#define NAV1_H

#include <windows.h>
#include "export.h"

/* Core waypoint type for route and FPL modules */
#define NAV_ROUTE_MAX_IDENT 8
#define NAV_ROUTE_MAX_NAME 32

typedef struct {
    double latitude;
    double longitude;
    double altitudeFt;
    char ident[NAV_ROUTE_MAX_IDENT];
    char name[NAV_ROUTE_MAX_NAME];
    int type;  /* 0=user, 1=airport, 2=VOR, 3=NDB, 4=fix, 5=intersection, 6=procedure */
} NAV_Sup_Waypoint;

NAV_EXPORT void AboutNav();
NAV_EXPORT void Version();
NAV_EXPORT void Testcase(int test);
NAV_EXPORT void WINAPI Rundll32Version(HWND, HINSTANCE, LPSTR, int);
NAV_EXPORT void WINAPI RunTest(HWND, HINSTANCE, LPSTR, int);

NAV_EXPORT double NAV_SUP_distanceToTarget(double lat1, double lon1, double lat2, double lon2);
NAV_EXPORT double NAV_SUP_headingToTarget(double lat1, double lon1, double lat2, double lon2);
NAV_EXPORT void    NAV_SUP_destinationPoint(double lat1, double lon1, double bearing, double distNm, double *outLat, double *outLon);
NAV_EXPORT void    NAV_SUP_midpoint(double lat1, double lon1, double lat2, double lon2, double *outLat, double *outLon);
NAV_EXPORT double  NAV_SUP_finalBearing(double lat1, double lon1, double lat2, double lon2);
NAV_EXPORT double  NAV_SUP_crossTrackDistance(double lat1, double lon1, double lat2, double lon2, double lat3, double lon3);
NAV_EXPORT double  NAV_SUP_dmsToDecimal(int degrees, int minutes, double seconds, char dir);
NAV_EXPORT void    NAV_SUP_decimalToDMS(double decimal, int *degrees, int *minutes, double *seconds);

#include "conv.h"
#include "nav.h"
#include "aviation.h"
#include "gps.h"
#include "flight_mgmt.h"
#include "control.h"
#include "nmea.h"
#include "route.h"
#include "xplane_nav.h"
#include "arinc429.h"
#include "arinc424.h"
#include "metar.h"

#endif
