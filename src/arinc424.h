#ifndef NAV1_ARINC424_H
#define NAV1_ARINC424_H

#include "nav1.h"

#define NAV1_A424_ICAO_LEN   5
#define NAV1_A424_IDENT_LEN  6
#define NAV1_A424_NAME_LEN   31

typedef struct {
    char icao[NAV1_A424_ICAO_LEN];
    char ident[NAV1_A424_IDENT_LEN];
    char name[NAV1_A424_NAME_LEN];
    double latitude;
    double longitude;
    double frequency;
    double elevationFt;
    int navType;
    char classCode[4];
} NAV1_A424Navaid;

typedef struct {
    char icao[NAV1_A424_ICAO_LEN];
    char ident[NAV1_A424_IDENT_LEN];
    double latitude;
    double longitude;
    int waypointType;
    char usage[4];
} NAV1_A424Waypoint;

typedef struct {
    char icao[NAV1_A424_ICAO_LEN];
    char iata[5];
    char name[NAV1_A424_NAME_LEN];
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
} NAV1_A424Airport;

typedef struct {
    char icao[NAV1_A424_ICAO_LEN];
    char rwyIdent[NAV1_A424_IDENT_LEN];
    double latitude;
    double longitude;
    double lengthFt;
    double widthFt;
    double bearingDeg;
    double thresholdElevFt;
    char surfaceCode[3];
    char description[23];
} NAV1_A424Runway;

typedef struct {
    char icao[NAV1_A424_ICAO_LEN];
    char rwyIdent[NAV1_A424_IDENT_LEN];
    char ident[NAV1_A424_IDENT_LEN];
    double latitude;
    double longitude;
    double frequency;
    int category;
    double bearingDeg;
} NAV1_A424ILS;

typedef struct {
    NAV1_A424Navaid *navaids;
    NAV1_A424Waypoint *waypoints;
    NAV1_A424Airport *airports;
    NAV1_A424Runway *runways;
    NAV1_A424ILS *ils;
    int nnavaids;
    int nwaypoints;
    int nairports;
    int nrunways;
    int nils;
    int capacity;
} NAV1_A424Database;

NAV1_EXPORT int  NAV1_A424_a424ParseFile(const char *filename, NAV1_A424Database *db);
NAV1_EXPORT void NAV1_A424_a424Free(NAV1_A424Database *db);

#endif
