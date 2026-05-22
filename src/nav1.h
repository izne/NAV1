#ifndef NAV1_H
#define NAV1_H

#include <windows.h>
#include "export.h"

/* Core waypoint type for route and FPL modules */
#define NAV1_ROUTE_MAX_IDENT 8
#define NAV1_ROUTE_MAX_NAME 32

typedef struct {
    double latitude;
    double longitude;
    double altitudeFt;
    char ident[NAV1_ROUTE_MAX_IDENT];
    char name[NAV1_ROUTE_MAX_NAME];
    int type;  /* 0=user, 1=airport, 2=VOR, 3=NDB, 4=fix, 5=intersection, 6=procedure */
} NAV1_Waypoint;

NAV_EXPORT void Version();
NAV_EXPORT void WINAPI WinVer(HWND, HINSTANCE, LPSTR, int);

#include "geo.h"
#include "conv.h"
#include "nav.h"
#include "aero.h"
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
