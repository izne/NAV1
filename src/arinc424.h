#ifndef NAV1_ARINC424_H
#define NAV1_ARINC424_H

#include "nav1.h"

#define NAV_A424_ICAO_LEN   5
#define NAV_A424_IDENT_LEN  6
#define NAV_A424_NAME_LEN   31

typedef struct {
    char icao[NAV_A424_ICAO_LEN];
    char ident[NAV_A424_IDENT_LEN];
    char name[NAV_A424_NAME_LEN];
    double latitude;
    double longitude;
    double frequency;
    double elevationFt;
    int navType;        /* 2=VOR, 3=NDB, 4=DME, 5=TACAN */
    char classCode[4];
} NAV_Sup_A424Navaid;

typedef struct {
    char icao[NAV_A424_ICAO_LEN];
    char ident[NAV_A424_IDENT_LEN];
    double latitude;
    double longitude;
    int waypointType;   /* 0=unknown, 1=ICAO, 2=RNAV */
    char usage[4];
} NAV_Sup_A424Waypoint;

typedef struct {
    char icao[NAV_A424_ICAO_LEN];
    char iata[5];
    char name[NAV_A424_NAME_LEN];
    double latitude;
    double longitude;
    double elevationFt;
    double magVarDeg;
    char datumCode[4];
    char tz[4];
    int longestRunwayFt;
    int speedLimitKt;
    int transitionAltFt;
    int transitionLvlFt;
    char ifrCapability;
    char publicMilitary;
} NAV_Sup_A424Airport;

typedef struct {
    char icao[NAV_A424_ICAO_LEN];
    char rwyIdent[NAV_A424_IDENT_LEN];
    double latitude;
    double longitude;
    double lengthFt;
    double widthFt;
    double bearingDeg;
    double thresholdElevFt;
    char surfaceCode[3];
    char description[23];
} NAV_Sup_A424Runway;

typedef struct {
    char icao[NAV_A424_ICAO_LEN];
    char rwyIdent[NAV_A424_IDENT_LEN];
    char ident[NAV_A424_IDENT_LEN];
    double latitude;
    double longitude;
    double frequency;
    int category;       /* 0=none, 1=CAT I, 2=CAT II, 3=CAT III */
    double bearingDeg;
} NAV_Sup_A424ILS;

typedef struct {
    NAV_Sup_A424Navaid *navaids;
    NAV_Sup_A424Waypoint *waypoints;
    NAV_Sup_A424Airport *airports;
    NAV_Sup_A424Runway *runways;
    NAV_Sup_A424ILS *ils;
    int nnavaids;
    int nwaypoints;
    int nairports;
    int nrunways;
    int nils;
    int capacity;
} NAV_Sup_A424Database;

NAV_EXPORT int  NAV_SUP_a424ParseFile(const char *filename, NAV_Sup_A424Database *db);
NAV_EXPORT void NAV_SUP_a424Free(NAV_Sup_A424Database *db);

#endif
