#include "arinc424.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define A424_INIT_CAPACITY 128
#define A424_GROW_FACTOR 2
#define A424_LINE_LEN 132

typedef struct {
    int section;
    int sub;
} A424Key;

typedef struct {
    A424Key key;
    int typeIdx;
} A424Dispatch;

static A424Dispatch dispatchTable[] = {
    {{'P', 'A'}, 2},
    {{'P', 'C'}, 1},
    {{'P', 'G'}, 3},
    {{'P', 'I'}, 4},
    {{'E', 'A'}, 1},
};
static const int dispatchTableSize = sizeof(dispatchTable) / sizeof(dispatchTable[0]);

static void trimTrail(char *s)
{
    int i = (int)strlen(s) - 1;
    while (i >= 0 && (s[i] == ' ' || s[i] == '\n' || s[i] == '\r'))
        s[i--] = '\0';
}

static int parseLatLon(const char *latStr, int latLen, const char *lonStr, int lonLen, double *outLat, double *outLon)
{
    char ns, ew;
    int d, m;
    double s;
    if (!latStr || !lonStr || latLen < 9 || lonLen < 10)
        return 0;
    ns = latStr[0];
    ew = lonStr[0];
    if (ns != 'N' && ns != 'S') return 0;
    if (ew != 'E' && ew != 'W') return 0;
    if (sscanf(latStr + 1, "%2d%2d%lf", &d, &m, &s) < 3)
    {
        if (sscanf(latStr + 1, "%2d%lf", &d, &m) < 2)
            return 0;
        s = 0.0;
    }
    *outLat = (double)d + (double)m / 60.0 + s / 3600.0;
    if (ns == 'S') *outLat = -*outLat;
    if (sscanf(lonStr + 1, "%3d%2d%lf", &d, &m, &s) < 3)
    {
        if (sscanf(lonStr + 1, "%3d%lf", &d, &m) < 2)
            return 0;
        s = 0.0;
    }
    *outLon = (double)d + (double)m / 60.0 + s / 3600.0;
    if (ew == 'W') *outLon = -*outLon;
    return 1;
}

static int fieldInt(const char *line, int beg1, int len)
{
    char buf[32];
    int v;
    if (beg1 < 1) return 0;
    strncpy(buf, line + beg1 - 1, (size_t)len);
    buf[len] = '\0';
    if (sscanf(buf, "%d", &v) == 1)
        return v;
    return 0;
}

static double fieldDouble(const char *line, int beg1, int len)
{
    char buf[32];
    double v;
    if (beg1 < 1) return 0.0;
    strncpy(buf, line + beg1 - 1, (size_t)len);
    buf[len] = '\0';
    if (sscanf(buf, "%lf", &v) == 1)
        return v;
    return 0.0;
}

static void fieldStr(const char *line, int beg1, int len, char *out, int outMax)
{
    int cpLen = len < outMax - 1 ? len : outMax - 1;
    if (beg1 < 1) { out[0] = '\0'; return; }
    strncpy(out, line + beg1 - 1, (size_t)cpLen);
    out[cpLen] = '\0';
    trimTrail(out);
}

static int parseAirport(const char *line, NAV1_A424Airport *ap)
{
    char latStr[16], lonStr[16], magStr[8], elevStr[8];
    double lat, lon;
    fieldStr(line, 7, 4, ap->icao, sizeof(ap->icao));
    fieldStr(line, 14, 3, ap->iata, sizeof(ap->iata));
    fieldStr(line, 52, 5, magStr, sizeof(magStr));
    fieldStr(line, 57, 5, elevStr, sizeof(elevStr));
    fieldStr(line, 94, 30, ap->name, sizeof(ap->name));
    fieldStr(line, 87, 3, ap->datumCode, sizeof(ap->datumCode));
    fieldStr(line, 82, 3, ap->tz, sizeof(ap->tz));
    ap->longestRunwayFt = fieldInt(line, 28, 3);
    ap->speedLimitKt = fieldInt(line, 62, 3);
    ap->transitionAltFt = fieldInt(line, 71, 5);
    ap->transitionLvlFt = fieldInt(line, 76, 5);
    ap->ifrCapability = (line[30] >= 'A' && line[30] <= 'Z') ? line[30] : ' ';
    ap->publicMilitary = (line[80] >= 'A' && line[80] <= 'Z') ? line[80] : ' ';
    ap->elevationFt = fieldDouble(line, 57, 5);
    ap->magVarDeg = fieldDouble(line, 52, 5);
    fieldStr(line, 33, 9, latStr, sizeof(latStr));
    fieldStr(line, 42, 10, lonStr, sizeof(lonStr));
    if (parseLatLon(latStr, 9, lonStr, 10, &lat, &lon))
    {
        ap->latitude = lat;
        ap->longitude = lon;
    }
    return 1;
}

static int parseWaypoint(const char *line, NAV1_A424Waypoint *wp)
{
    char latStr[16], lonStr[16], typStr[4], useStr[4];
    double lat, lon;
    fieldStr(line, 7, 4, wp->icao, sizeof(wp->icao));
    fieldStr(line, 14, 5, wp->ident, sizeof(wp->ident));
    fieldStr(line, 27, 3, typStr, sizeof(typStr));
    fieldStr(line, 30, 2, useStr, sizeof(useStr));
    fieldStr(line, 33, 9, latStr, sizeof(latStr));
    fieldStr(line, 42, 10, lonStr, sizeof(lonStr));
    strncpy(wp->usage, useStr, sizeof(wp->usage) - 1);
    wp->usage[sizeof(wp->usage) - 1] = '\0';
    wp->waypointType = fieldInt(line, 27, 3);
    if (parseLatLon(latStr, 9, lonStr, 10, &lat, &lon))
    {
        wp->latitude = lat;
        wp->longitude = lon;
    }
    return 1;
}

static int parseRunway(const char *line, NAV1_A424Runway *rw)
{
    char latStr[16], lonStr[16], lenStr[8], widStr[8], brgStr[8], elevStr[8];
    double lat, lon;
    fieldStr(line, 7, 4, rw->icao, sizeof(rw->icao));
    fieldStr(line, 14, 5, rw->rwyIdent, sizeof(rw->rwyIdent));
    fieldStr(line, 33, 9, latStr, sizeof(latStr));
    fieldStr(line, 42, 10, lonStr, sizeof(lonStr));
    fieldStr(line, 102, 22, rw->description, sizeof(rw->description));
    rw->lengthFt = fieldDouble(line, 23, 5);
    rw->bearingDeg = fieldDouble(line, 28, 4);
    rw->thresholdElevFt = fieldDouble(line, 67, 5);
    rw->widthFt = fieldDouble(line, 78, 3);
    fieldStr(line, 39, 2, rw->surfaceCode, sizeof(rw->surfaceCode));
    if (parseLatLon(latStr, 9, lonStr, 10, &lat, &lon))
    {
        rw->latitude = lat;
        rw->longitude = lon;
    }
    return 1;
}

static int parseILS(const char *line, NAV1_A424ILS *ils)
{
    char latStr[16], lonStr[16], freqStr[8], catStr[4], brgStr[8];
    double lat, lon;
    fieldStr(line, 7, 4, ils->icao, sizeof(ils->icao));
    fieldStr(line, 82, 4, ils->rwyIdent, sizeof(ils->rwyIdent));
    fieldStr(line, 14, 5, ils->ident, sizeof(ils->ident));
    fieldStr(line, 33, 9, latStr, sizeof(latStr));
    fieldStr(line, 42, 10, lonStr, sizeof(lonStr));
    fieldStr(line, 53, 5, freqStr, sizeof(freqStr));
    fieldStr(line, 59, 1, catStr, sizeof(catStr));
    fieldStr(line, 82, 4, brgStr, sizeof(brgStr));
    ils->frequency = fieldDouble(line, 53, 5);
    ils->bearingDeg = fieldDouble(line, 82, 4);
    if (catStr[0] >= '1' && catStr[0] <= '3')
        ils->category = catStr[0] - '0';
    else
        ils->category = 0;
    if (parseLatLon(latStr, 9, lonStr, 10, &lat, &lon))
    {
        ils->latitude = lat;
        ils->longitude = lon;
    }
    return 1;
}

static int dbGrow(void **arr, int *capacity, size_t elemSize)
{
    int newCap = *capacity ? *capacity * A424_GROW_FACTOR : A424_INIT_CAPACITY;
    void *p = realloc(*arr, (size_t)newCap * elemSize);
    if (!p)
        return 0;
    *arr = p;
    *capacity = newCap;
    return 1;
}

#define DB_ADD(db, field, type) do { \
    if ((db)->n##field >= (db)->capacity) \
        if (!dbGrow((void**)&(db)->field, &(db)->capacity, sizeof(type))) \
            return (db)->n##field > 0; \
    } while(0)

int NAV1_A424_a424ParseFile(const char *filename, NAV1_A424Database *db)
{
    FILE *f;
    char line[A424_LINE_LEN + 4];
    A424Key key;
    int i, dispatchIdx = -1;
    memset(db, 0, sizeof(*db));
    f = fopen(filename, "r");
    if (!f)
        return 0;
    while (fgets(line, sizeof(line), f))
    {
        if (line[0] == 'S' && line[4] >= 'A' && line[4] <= 'Z')
        {
            key.section = line[4];
            key.sub = line[5];
            dispatchIdx = -1;
            for (i = 0; i < dispatchTableSize; i++)
            {
                if (dispatchTable[i].key.section == key.section &&
                    dispatchTable[i].key.sub == key.sub)
                {
                    dispatchIdx = dispatchTable[i].typeIdx;
                    break;
                }
            }
            if (dispatchIdx == 0)
            {
                DB_ADD(db, navaids, NAV1_A424Navaid);
                memset(&db->navaids[db->nnavaids], 0, sizeof(NAV1_A424Navaid));
                db->nnavaids++;
            }
            else if (dispatchIdx == 1)
            {
                DB_ADD(db, waypoints, NAV1_A424Waypoint);
                memset(&db->waypoints[db->nwaypoints], 0, sizeof(NAV1_A424Waypoint));
                db->nwaypoints++;
            }
            else if (dispatchIdx == 2)
            {
                DB_ADD(db, airports, NAV1_A424Airport);
                memset(&db->airports[db->nairports], 0, sizeof(NAV1_A424Airport));
                db->nairports++;
            }
            else if (dispatchIdx == 3)
            {
                DB_ADD(db, runways, NAV1_A424Runway);
                memset(&db->runways[db->nrunways], 0, sizeof(NAV1_A424Runway));
                db->nrunways++;
            }
            else if (dispatchIdx == 4)
            {
                DB_ADD(db, ils, NAV1_A424ILS);
                memset(&db->ils[db->nils], 0, sizeof(NAV1_A424ILS));
                db->nils++;
            }
        }
    }
    rewind(f);
    db->nnavaids = 0;
    db->nwaypoints = 0;
    db->nairports = 0;
    db->nrunways = 0;
    db->nils = 0;
    while (fgets(line, sizeof(line), f))
    {
        if (line[0] != 'S')
            continue;
        key.section = line[4];
        key.sub = line[5];
        dispatchIdx = -1;
        for (i = 0; i < dispatchTableSize; i++)
        {
            if (dispatchTable[i].key.section == key.section &&
                dispatchTable[i].key.sub == key.sub)
            {
                dispatchIdx = dispatchTable[i].typeIdx;
                break;
            }
        }
        if (dispatchIdx == 2)
            parseAirport(line, &db->airports[db->nairports++]);
        else if (dispatchIdx == 1)
            parseWaypoint(line, &db->waypoints[db->nwaypoints++]);
        else if (dispatchIdx == 3)
            parseRunway(line, &db->runways[db->nrunways++]);
        else if (dispatchIdx == 4)
            parseILS(line, &db->ils[db->nils++]);
    }
    fclose(f);
    return (db->nairports > 0 || db->nwaypoints > 0 || db->nrunways > 0 || db->nils > 0) ? 1 : 0;
}

void NAV1_A424_a424Free(NAV1_A424Database *db)
{
    if (db)
    {
        free(db->navaids);   db->navaids = NULL;   db->nnavaids = 0;
        free(db->waypoints); db->waypoints = NULL;  db->nwaypoints = 0;
        free(db->airports);  db->airports = NULL;   db->nairports = 0;
        free(db->runways);   db->runways = NULL;    db->nrunways = 0;
        free(db->ils);       db->ils = NULL;         db->nils = 0;
        db->capacity = 0;
    }
}
