#ifndef NAV1_XPLANE_NAV_H
#define NAV1_XPLANE_NAV_H

#include "nav1.h"

#define NAV1_NAV_VOR      2
#define NAV1_NAV_NDB      3
#define NAV1_NAV_DME      4
#define NAV1_NAV_ILS      5
#define NAV1_NAV_LOCALIZER 6
#define NAV1_NAV_GS       7
#define NAV1_NAV_OMNI     8
#define NAV1_NAV_FIX     255

typedef struct {
    int type;
    char ident[8];
    char name[40];
    double latitude;
    double longitude;
    double frequency;
    double elevationFt;
} NAV1_NavRecord;

typedef struct {
    NAV1_NavRecord *records;
    int count;
    int capacity;
} NAV1_NavDatabase;

NAV1_EXPORT int  NAV1_XPL_xpNavLoad(const char *filename, NAV1_NavDatabase *db);
NAV1_EXPORT int  NAV1_XPL_xpFixLoad(const char *filename, NAV1_NavDatabase *db);
NAV1_EXPORT int  NAV1_XPL_xpNavFindByID(const NAV1_NavDatabase *db, const char *ident, int startIndex, NAV1_NavRecord *out);
NAV1_EXPORT int  NAV1_XPL_xpNavFindNearest(const NAV1_NavDatabase *db, double lat, double lon, int typeFilter, NAV1_NavRecord *out);
NAV1_EXPORT int  NAV1_XPL_fplParseRoute(const char *str, const NAV1_NavDatabase *navdb, const NAV1_NavDatabase *fixdb, NAV1_Waypoint *waypoints, int maxCount);
NAV1_EXPORT void NAV1_XPL_xpNavFree(NAV1_NavDatabase *db);

#endif
