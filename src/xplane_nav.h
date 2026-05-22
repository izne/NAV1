#ifndef NAV1_XPLANE_NAV_H
#define NAV1_XPLANE_NAV_H

#include "nav1.h"

#define NAV_NAV_VOR      2
#define NAV_NAV_NDB      3
#define NAV_NAV_DME      4
#define NAV_NAV_ILS      5
#define NAV_NAV_LOCALIZER 6
#define NAV_NAV_GS       7
#define NAV_NAV_OMNI     8
#define NAV_NAV_FIX     255

typedef struct {
    int type;
    char ident[8];
    char name[40];
    double latitude;
    double longitude;
    double frequency;
    double elevationFt;
} NAV_Sup_NavRecord;

typedef struct {
    NAV_Sup_NavRecord *records;
    int count;
    int capacity;
} NAV_Sup_NavDatabase;

NAV_EXPORT int  NAV_SUP_xpNavLoad(const char *filename, NAV_Sup_NavDatabase *db);
NAV_EXPORT int  NAV_SUP_xpFixLoad(const char *filename, NAV_Sup_NavDatabase *db);
NAV_EXPORT int  NAV_SUP_xpNavFindByID(const NAV_Sup_NavDatabase *db, const char *ident, int startIndex, NAV_Sup_NavRecord *out);
NAV_EXPORT int  NAV_SUP_xpNavFindNearest(const NAV_Sup_NavDatabase *db, double lat, double lon, int typeFilter, NAV_Sup_NavRecord *out);
NAV_EXPORT int  NAV_SUP_fplParseRoute(const char *str, const NAV_Sup_NavDatabase *navdb, const NAV_Sup_NavDatabase *fixdb, NAV_Sup_Waypoint *waypoints, int maxCount);
NAV_EXPORT void NAV_SUP_xpNavFree(NAV_Sup_NavDatabase *db);

#endif
