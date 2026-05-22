#include "xplane_nav.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define XPN_INIT_CAPACITY 1024
#define XPN_GROW_FACTOR 2

static double toRad(double deg) { return deg * 3.14159265358979323846 / 180.0; }
static double toDeg(double rad) { return rad * 180.0 / 3.14159265358979323846; }
static double R = 3440.06479;

static int dbGrow(NAV1_NavDatabase *db)
{
    int newCap = db->capacity ? db->capacity * XPN_GROW_FACTOR : XPN_INIT_CAPACITY;
    NAV1_NavRecord *p = (NAV1_NavRecord *)realloc(db->records, (size_t)newCap * sizeof(NAV1_NavRecord));
    if (!p)
        return 0;
    db->records = p;
    db->capacity = newCap;
    return 1;
}

static int dbAdd(NAV1_NavDatabase *db, int type, const char *ident, const char *name, double lat, double lon, double freq, double elev)
{
    NAV1_NavRecord *r;
    if (db->count >= db->capacity)
        if (!dbGrow(db))
            return 0;
    r = &db->records[db->count++];
    r->type = type;
    r->latitude = lat;
    r->longitude = lon;
    r->frequency = freq;
    r->elevationFt = elev;
    strncpy(r->ident, ident, sizeof(r->ident) - 1);
    r->ident[sizeof(r->ident) - 1] = '\0';
    strncpy(r->name, name, sizeof(r->name) - 1);
    r->name[sizeof(r->name) - 1] = '\0';
    return 1;
}

int NAV1_XPL_xpNavLoad(const char *filename, NAV1_NavDatabase *db)
{
    FILE *f;
    char line[256];
    int code;
    double lat, lon, elev, freq;
    char id[16], name[48];
    int n;
    db->records = NULL;
    db->count = 0;
    db->capacity = 0;
    f = fopen(filename, "r");
    if (!f)
        return 0;
    while (fgets(line, sizeof(line), f))
    {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r')
            continue;
        id[0] = '\0';
        name[0] = '\0';
        n = sscanf(line, "%d%lf%lf%lf%lf%7s%47[^\n]", &code, &lat, &lon, &elev, &freq, id, name);
        if (n < 6)
            continue;
        if (code < 2 || code > 8)
            continue;
        if (id[0] == '\0')
            continue;
        dbAdd(db, code, id, name, lat, lon, freq, elev);
    }
    fclose(f);
    return db->count > 0 ? 1 : 0;
}

int NAV1_XPL_xpFixLoad(const char *filename, NAV1_NavDatabase *db)
{
    FILE *f;
    char line[128];
    char id[16];
    double lon, lat;
    int n;
    db->records = NULL;
    db->count = 0;
    db->capacity = 0;
    f = fopen(filename, "r");
    if (!f)
        return 0;
    while (fgets(line, sizeof(line), f))
    {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r')
            continue;
        id[0] = '\0';
        n = sscanf(line, "%7s%lf%lf", id, &lon, &lat);
        if (n < 3)
            continue;
        if (id[0] == '\0')
            continue;
        dbAdd(db, NAV1_NAV_FIX, id, "", lat, lon, 0.0, 0.0);
    }
    fclose(f);
    return db->count > 0 ? 1 : 0;
}

int NAV1_XPL_xpNavFindByID(const NAV1_NavDatabase *db, const char *ident, int startIndex, NAV1_NavRecord *out)
{
    int i;
    if (!db || !db->records || startIndex < 0)
        return 0;
    for (i = startIndex; i < db->count; i++)
    {
        if (strcmp(db->records[i].ident, ident) == 0)
        {
            if (out)
                *out = db->records[i];
            return i + 1;
        }
    }
    return 0;
}

int NAV1_XPL_xpNavFindNearest(const NAV1_NavDatabase *db, double lat, double lon, int typeFilter, NAV1_NavRecord *out)
{
    int i, bestIdx = -1;
    double bestDist = 1e100, d, dlat, dlon, lat1r, lat2r;
    if (!db || db->count == 0)
        return 0;
    lat1r = toRad(lat);
    for (i = 0; i < db->count; i++)
    {
        if (typeFilter > 0 && db->records[i].type != typeFilter)
            continue;
        dlat = toRad(db->records[i].latitude - lat);
        dlon = toRad(db->records[i].longitude - lon);
        lat2r = toRad(db->records[i].latitude);
        d = sin(dlat / 2.0) * sin(dlat / 2.0) +
            cos(lat1r) * cos(lat2r) * sin(dlon / 2.0) * sin(dlon / 2.0);
        d = 2.0 * atan2(sqrt(d), sqrt(1.0 - d));
        if (d < bestDist)
        {
            bestDist = d;
            bestIdx = i;
        }
    }
    if (bestIdx < 0)
        return 0;
    if (out)
        *out = db->records[bestIdx];
    return 1;
}

static int isSpeedAltField(const char *tok)
{
    if (!tok || !tok[0])
        return 0;
    if (tok[0] == 'N' || tok[0] == 'M' || tok[0] == 'S')
    {
        int i = 1;
        while (tok[i] && ((tok[i] >= '0' && tok[i] <= '9') || tok[i] == '.'))
            i++;
        if (tok[i] == 'F' || tok[i] == 'A')
        {
            i++;
            while (tok[i] && ((tok[i] >= '0' && tok[i] <= '9') || tok[i] == '.'))
                i++;
            return tok[i] == '\0';
        }
        return tok[i] == '\0';
    }
    if (tok[0] == 'F' || tok[0] == 'A')
    {
        int i = 1;
        while (tok[i] && tok[i] >= '0' && tok[i] <= '9')
            i++;
        return tok[i] == '\0';
    }
    return 0;
}

static int isAllAlpha(const char *tok)
{
    int i;
    if (!tok || !tok[0])
        return 0;
    for (i = 0; tok[i]; i++)
        if (!((tok[i] >= 'A' && tok[i] <= 'Z') || (tok[i] >= 'a' && tok[i] <= 'z')))
            return 0;
    return 1;
}

static int isAlphaDigits(const char *tok)
{
    int i, hasAlpha = 0, hasDigit = 0;
    if (!tok || !tok[0])
        return 0;
    for (i = 0; tok[i]; i++)
    {
        if ((tok[i] >= 'A' && tok[i] <= 'Z') || (tok[i] >= 'a' && tok[i] <= 'z'))
            hasAlpha = 1;
        else if (tok[i] >= '0' && tok[i] <= '9')
            hasDigit = 1;
        else
            return 0;
    }
    return hasAlpha && hasDigit;
}

static int isCoordPair(const char *tok)
{
    int len;
    if (!tok || !tok[0])
        return 0;
    len = (int)strlen(tok);
    if (len < 8)
        return 0;
    if (tok[len - 1] == 'E' || tok[len - 1] == 'W')
        return 1;
    return 0;
}

static int parseCoordPair(const char *tok, double *outLat, double *outLon)
{
    char latStr[32], lonStr[32];
    double latDeg, lonDeg, latMin, lonMin;
    int i, sep;
    char ns, ew;
    int len = (int)strlen(tok);
    for (i = 0; i < len; i++)
    {
        if (tok[i] == 'N' || tok[i] == 'S')
        {
            sep = i + 1;
            break;
        }
    }
    if (i >= len)
        return 0;
    ns = tok[i];
    strncpy(latStr, tok, i);
    latStr[i] = '\0';
    strncpy(lonStr, tok + i + 1, len - i - 2);
    lonStr[len - i - 2] = '\0';
    ew = tok[len - 1];
    if (ew != 'E' && ew != 'W')
        return 0;
    if (strchr(latStr, '.') && strchr(lonStr, '.'))
    {
        sscanf(latStr, "%lf", &latDeg);
        sscanf(lonStr, "%lf", &lonDeg);
        latMin = 0.0;
        lonMin = 0.0;
    }
    else
    {
        int latInt, lonInt;
        sscanf(latStr, "%d", &latInt);
        sscanf(lonStr, "%d", &lonInt);
        if (strlen(latStr) > 2)
        {
            char latD[4], latM[16];
            strncpy(latD, latStr, 2);
            latD[2] = '\0';
            strcpy(latM, latStr + 2);
            latDeg = (double)atoi(latD);
            latMin = (double)atof(latM);
        }
        else
        {
            latDeg = (double)latInt;
            latMin = 0.0;
        }
        if (strlen(lonStr) > 3)
        {
            char lonD[5], lonM[16];
            strncpy(lonD, lonStr, 3);
            lonD[3] = '\0';
            strcpy(lonM, lonStr + 3);
            lonDeg = (double)atoi(lonD);
            lonMin = (double)atof(lonM);
        }
        else
        {
            lonDeg = (double)lonInt;
            lonMin = 0.0;
        }
    }
    *outLat = latDeg + latMin / 60.0;
    if (ns == 'S') *outLat = -*outLat;
    *outLon = lonDeg + lonMin / 60.0;
    if (ew == 'W') *outLon = -*outLon;
    return 1;
}

static void trimNewline(char *s)
{
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r'))
        s[--len] = '\0';
}

int NAV1_XPL_fplParseRoute(const char *str, const NAV1_NavDatabase *navdb, const NAV1_NavDatabase *fixdb, NAV1_Waypoint *waypoints, int maxCount)
{
    char buf[1024];
    char *tokens[128];
    char *p;
    int ntok = 0, i, wpCount = 0;
    NAV1_NavRecord rec;
    if (!str || !waypoints || maxCount < 1)
        return 0;
    strncpy(buf, str, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    trimNewline(buf);
    p = buf;
    while (*p && ntok < 128)
    {
        while (*p == ' ')
            p++;
        if (!*p)
            break;
        tokens[ntok++] = p;
        while (*p && *p != ' ')
            p++;
        if (*p)
            *p++ = '\0';
    }
    for (i = 0; i < ntok && wpCount < maxCount; i++)
    {
        const char *tok = tokens[i];
        if (strcmp(tok, "DCT") == 0)
            continue;
        if (isSpeedAltField(tok))
            continue;
        if (isCoordPair(tok))
        {
            double lat, lon;
            if (parseCoordPair(tok, &lat, &lon))
            {
                waypoints[wpCount].latitude = lat;
                waypoints[wpCount].longitude = lon;
                waypoints[wpCount].altitudeFt = 0.0;
                waypoints[wpCount].type = 0;
                strncpy(waypoints[wpCount].ident, tok, NAV1_ROUTE_MAX_IDENT - 1);
                waypoints[wpCount].ident[NAV1_ROUTE_MAX_IDENT - 1] = '\0';
                waypoints[wpCount].name[0] = '\0';
                wpCount++;
            }
            continue;
        }
        if (isAllAlpha(tok))
        {
            if (navdb && NAV1_XPL_xpNavFindByID(navdb, tok, 0, &rec))
            {
                waypoints[wpCount].latitude = rec.latitude;
                waypoints[wpCount].longitude = rec.longitude;
                waypoints[wpCount].altitudeFt = rec.elevationFt;
                waypoints[wpCount].type = rec.type;
                strncpy(waypoints[wpCount].ident, rec.ident, NAV1_ROUTE_MAX_IDENT - 1);
                waypoints[wpCount].ident[NAV1_ROUTE_MAX_IDENT - 1] = '\0';
                strncpy(waypoints[wpCount].name, rec.name, NAV1_ROUTE_MAX_NAME - 1);
                waypoints[wpCount].name[NAV1_ROUTE_MAX_NAME - 1] = '\0';
                wpCount++;
            }
            else if (fixdb && NAV1_XPL_xpNavFindByID(fixdb, tok, 0, &rec))
            {
                waypoints[wpCount].latitude = rec.latitude;
                waypoints[wpCount].longitude = rec.longitude;
                waypoints[wpCount].altitudeFt = 0.0;
                waypoints[wpCount].type = rec.type;
                strncpy(waypoints[wpCount].ident, rec.ident, NAV1_ROUTE_MAX_IDENT - 1);
                waypoints[wpCount].ident[NAV1_ROUTE_MAX_IDENT - 1] = '\0';
                waypoints[wpCount].name[0] = '\0';
                wpCount++;
            }
            else
            {
                waypoints[wpCount].latitude = 0.0;
                waypoints[wpCount].longitude = 0.0;
                waypoints[wpCount].altitudeFt = 0.0;
                waypoints[wpCount].type = 4;
                strncpy(waypoints[wpCount].ident, tok, NAV1_ROUTE_MAX_IDENT - 1);
                waypoints[wpCount].ident[NAV1_ROUTE_MAX_IDENT - 1] = '\0';
                waypoints[wpCount].name[0] = '\0';
                wpCount++;
            }
            continue;
        }
        if (isAlphaDigits(tok))
        {
            if (navdb && NAV1_XPL_xpNavFindByID(navdb, tok, 0, &rec))
            {
                waypoints[wpCount].latitude = rec.latitude;
                waypoints[wpCount].longitude = rec.longitude;
                waypoints[wpCount].altitudeFt = rec.elevationFt;
                waypoints[wpCount].type = rec.type;
                strncpy(waypoints[wpCount].ident, rec.ident, NAV1_ROUTE_MAX_IDENT - 1);
                waypoints[wpCount].ident[NAV1_ROUTE_MAX_IDENT - 1] = '\0';
                strncpy(waypoints[wpCount].name, rec.name, NAV1_ROUTE_MAX_NAME - 1);
                waypoints[wpCount].name[NAV1_ROUTE_MAX_NAME - 1] = '\0';
                wpCount++;
            }
            else if (fixdb && NAV1_XPL_xpNavFindByID(fixdb, tok, 0, &rec))
            {
                waypoints[wpCount].latitude = rec.latitude;
                waypoints[wpCount].longitude = rec.longitude;
                waypoints[wpCount].altitudeFt = 0.0;
                waypoints[wpCount].type = rec.type;
                strncpy(waypoints[wpCount].ident, rec.ident, NAV1_ROUTE_MAX_IDENT - 1);
                waypoints[wpCount].ident[NAV1_ROUTE_MAX_IDENT - 1] = '\0';
                waypoints[wpCount].name[0] = '\0';
                wpCount++;
            }
            else if (i < ntok - 1 && isAllAlpha(tokens[i + 1]))
            {
                continue;
            }
            else
            {
                waypoints[wpCount].latitude = 0.0;
                waypoints[wpCount].longitude = 0.0;
                waypoints[wpCount].altitudeFt = 0.0;
                waypoints[wpCount].type = 6;
                strncpy(waypoints[wpCount].ident, tok, NAV1_ROUTE_MAX_IDENT - 1);
                waypoints[wpCount].ident[NAV1_ROUTE_MAX_IDENT - 1] = '\0';
                waypoints[wpCount].name[0] = '\0';
                wpCount++;
            }
            continue;
        }
    }
    return wpCount;
}

void NAV1_XPL_xpNavFree(NAV1_NavDatabase *db)
{
    if (db)
    {
        free(db->records);
        db->records = NULL;
        db->count = 0;
        db->capacity = 0;
    }
}
