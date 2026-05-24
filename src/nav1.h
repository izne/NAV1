#ifndef NAV1_INTERNAL_H
#define NAV1_INTERNAL_H

/* Internal master header — includes export.h + all module headers.
   Public consumers should use NAV1.h (project root) instead. */

#include "export.h"

#define NAV1_ROUTE_MAX_IDENT 8
#define NAV1_ROUTE_MAX_NAME 32

typedef struct {
    double latitude;
    double longitude;
    double altitudeFt;
    char ident[NAV1_ROUTE_MAX_IDENT];
    char name[NAV1_ROUTE_MAX_NAME];
    int type;
} NAV1_Waypoint;

#include "geo.h"
#include "conv.h"
#include "nav.h"
#include "aero.h"
#include "gps.h"
#include "flight_mgmt.h"
#include "control.h"
#include "nmea.h"
#include "perf.h"
#include "route.h"
#include "wb.h"
#include "xplane_nav.h"
#include "arinc429.h"
#include "arinc424.h"
#include "metar.h"

#endif
