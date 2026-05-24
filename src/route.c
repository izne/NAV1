#include "route.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

static double toRad(double deg) { return deg * 3.14159265358979323846 / 180.0; }
static double toDeg(double rad) { return rad * 180.0 / 3.14159265358979323846; }
static double R = 3440.06479;

void NAV1_RTE_routeInit(NAV1_Route *route, NAV1_Waypoint *buffer, int capacity)
{
    route->waypoints = buffer;
    route->capacity = capacity;
    route->count = 0;
    route->currentIndex = -1;
}

int NAV1_RTE_routeAdd(NAV1_Route *route, double lat, double lon, double altFt, const char *ident, const char *name, int type)
{
    if (route->count >= route->capacity)
        return 0;
    NAV1_Waypoint *wp = &route->waypoints[route->count];
    wp->latitude = lat;
    wp->longitude = lon;
    wp->altitudeFt = altFt;
    wp->type = type;
    strncpy(wp->ident, ident, NAV1_ROUTE_MAX_IDENT - 1);
    wp->ident[NAV1_ROUTE_MAX_IDENT - 1] = '\0';
    strncpy(wp->name, name, NAV1_ROUTE_MAX_NAME - 1);
    wp->name[NAV1_ROUTE_MAX_NAME - 1] = '\0';
    route->count++;
    if (route->currentIndex < 0)
        route->currentIndex = 0;
    return 1;
}

int NAV1_RTE_routeInsert(NAV1_Route *route, int index, double lat, double lon, double altFt, const char *ident, const char *name, int type)
{
    int i;
    if (route->count >= route->capacity)
        return 0;
    if (index < 0 || index > route->count)
        return 0;
    for (i = route->count; i > index; i--)
        route->waypoints[i] = route->waypoints[i - 1];
    route->waypoints[index].latitude = lat;
    route->waypoints[index].longitude = lon;
    route->waypoints[index].altitudeFt = altFt;
    route->waypoints[index].type = type;
    strncpy(route->waypoints[index].ident, ident, NAV1_ROUTE_MAX_IDENT - 1);
    route->waypoints[index].ident[NAV1_ROUTE_MAX_IDENT - 1] = '\0';
    strncpy(route->waypoints[index].name, name, NAV1_ROUTE_MAX_NAME - 1);
    route->waypoints[index].name[NAV1_ROUTE_MAX_NAME - 1] = '\0';
    route->count++;
    if (route->currentIndex < 0)
        route->currentIndex = 0;
    return 1;
}

int NAV1_RTE_routeRemove(NAV1_Route *route, int index)
{
    int i;
    if (index < 0 || index >= route->count)
        return 0;
    for (i = index; i < route->count - 1; i++)
        route->waypoints[i] = route->waypoints[i + 1];
    route->count--;
    if (route->currentIndex >= route->count)
        route->currentIndex = route->count - 1;
    return 1;
}

void NAV1_RTE_routeClear(NAV1_Route *route)
{
    route->count = 0;
    route->currentIndex = -1;
}

int NAV1_RTE_routeSequence(NAV1_Route *route)
{
    if (route->count == 0)
        return 0;
    if (route->currentIndex < 0)
        route->currentIndex = 0;
    else if (route->currentIndex < route->count - 1)
        route->currentIndex++;
    else
        return 0;
    return 1;
}

int NAV1_RTE_routeDirectTo(NAV1_Route *route, int index)
{
    if (index < 0 || index >= route->count)
        return 0;
    route->currentIndex = index;
    return 1;
}

int NAV1_RTE_routeLegInfo(const NAV1_Route *route, int index, double *outBearingDeg, double *outDistNm)
{
    int fromIdx = route->currentIndex;
    double lat1, lon1, lat2, lon2, d, dlon, lat1r, lat2r, dlonr, x, y, brg;
    if (fromIdx < 0 || fromIdx >= route->count - 1)
        return 0;
    if (index < 0 || index >= route->count)
        return 0;
    if (index <= fromIdx)
        return 0;
    lat1 = route->waypoints[fromIdx].latitude;
    lon1 = route->waypoints[fromIdx].longitude;
    lat2 = route->waypoints[index].latitude;
    lon2 = route->waypoints[index].longitude;
    dlon = lon2 - lon1;
    lat1r = toRad(lat1);
    lat2r = toRad(lat2);
    dlonr = toRad(dlon);
    x = dlonr * cos((lat1r + lat2r) / 2.0);
    y = lat2r - lat1r;
    if (outDistNm)
        *outDistNm = sqrt(x * x + y * y) * R;
    x = sin(dlonr) * cos(lat2r);
    y = cos(lat1r) * sin(lat2r) - sin(lat1r) * cos(lat2r) * cos(dlonr);
    if (x == 0.0 && y == 0.0)
        brg = 0.0;
    else
        brg = atan2(x, y);
    if (outBearingDeg)
        *outBearingDeg = fmod(toDeg(brg) + 360.0, 360.0);
    return 1;
}

int NAV1_RTE_routeTurnAnticipation(const NAV1_Route *route, double groundSpeedKts, double turnRateDegS, double *outLat, double *outLon)
{
    int idx;
    double brg1, brg2, d1, d2, turnAngle, turnRadius, dist;
    double lat1, lon1, brg1r, turnAngleRad, lat2, lon2;
    if (route->currentIndex < 0 || route->currentIndex >= route->count - 2)
        return 0;
    if (turnRateDegS <= 0.0)
        return 0;
    idx = route->currentIndex;
    if (!NAV1_RTE_routeLegInfo(route, idx + 1, &brg1, &d1))
        return 0;
    if (!NAV1_RTE_routeLegInfo(route, idx + 2, &brg2, &d2))
        return 0;
    turnAngle = fabs(brg2 - brg1);
    if (turnAngle > 180.0)
        turnAngle = 360.0 - turnAngle;
    if (turnAngle < 0.1 || turnAngle > 120.0)
        return 0;
    turnAngleRad = toRad(turnAngle);
    turnRadius = groundSpeedKts / (turnRateDegS * 60.0 / 3.14159265358979323846);
    dist = turnRadius * tan(turnAngleRad / 2.0);
    if (dist > d1)
        dist = d1 * 0.9;
    lat1 = route->waypoints[idx].latitude;
    lon1 = route->waypoints[idx].longitude;
    brg1r = toRad(brg1);
    lat2 = asin(sin(toRad(lat1)) * cos(dist / R) + cos(toRad(lat1)) * sin(dist / R) * cos(brg1r));
    lon2 = toRad(lon1) + atan2(sin(brg1r) * sin(dist / R) * cos(toRad(lat1)), cos(dist / R) - sin(toRad(lat1)) * sin(lat2));
    if (outLat) *outLat = toDeg(lat2);
    if (outLon) *outLon = fmod(toDeg(lon2) + 540.0, 360.0) - 180.0;
    return 1;
}

int NAV1_RTE_routeSaveCSV(const char *filename, const NAV1_Route *route)
{
    FILE *f;
    int i;
    f = fopen(filename, "w");
    if (!f)
        return 0;
    fprintf(f, "# NAV1 route,lat,lon,altFt,ident,name,type\n");
    for (i = 0; i < route->count; i++)
    {
        fprintf(f, "%.7f,%.7f,%.1f,%s,%s,%d\n",
                route->waypoints[i].latitude,
                route->waypoints[i].longitude,
                route->waypoints[i].altitudeFt,
                route->waypoints[i].ident,
                route->waypoints[i].name,
                route->waypoints[i].type);
    }
    fclose(f);
    return 1;
}

int NAV1_RTE_routeReverse(NAV1_Route *route)
{
    int i, j;
    if (!route || route->count < 2) return 0;
    for (i = 0, j = route->count - 1; i < j; i++, j--) {
        NAV1_Waypoint tmp = route->waypoints[i];
        route->waypoints[i] = route->waypoints[j];
        route->waypoints[j] = tmp;
    }
    return 1;
}

double NAV1_RTE_routeTotalDistance(const NAV1_Route *route)
{
    int i;
    double total = 0.0, brg, dist;
    if (!route || route->count < 2) return 0.0;
    for (i = 0; i < route->count - 1; i++) {
        double lat1 = route->waypoints[i].latitude;
        double lon1 = route->waypoints[i].longitude;
        double lat2 = route->waypoints[i + 1].latitude;
        double lon2 = route->waypoints[i + 1].longitude;
        total += NAV1_GEO_distanceToTarget(lat1, lon1, lat2, lon2);
    }
    return total;
}

int NAV1_RTE_routeETA(const NAV1_Route *route, double gsKts, double *totalTimeSec)
{
    double totalDist;
    if (!route || gsKts < 0.1) return 0;
    totalDist = NAV1_RTE_routeTotalDistance(route);
    if (totalTimeSec) *totalTimeSec = totalDist / gsKts * 3600.0;
    return 1;
}

int NAV1_RTE_routeFindByIdent(const NAV1_Route *route, const char *ident)
{
    int i;
    if (!route || !ident) return -1;
    for (i = 0; i < route->count; i++)
        if (strcmp(route->waypoints[i].ident, ident) == 0)
            return i;
    return -1;
}

int NAV1_RTE_routeLoadCSV(const char *filename, NAV1_Waypoint *buffer, int capacity, NAV1_Route *route)
{
    FILE *f;
    char line[256];
    double lat, lon, alt;
    char ident[NAV1_ROUTE_MAX_IDENT], name[NAV1_ROUTE_MAX_NAME];
    int type, parsed;
    f = fopen(filename, "r");
    if (!f)
        return 0;
    NAV1_RTE_routeInit(route, buffer, capacity);
    while (fgets(line, sizeof(line), f))
    {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r')
            continue;
        parsed = sscanf(line, "%lf,%lf,%lf,%7[^,],%31[^,],%d",
                        &lat, &lon, &alt, ident, name, &type);
        if (parsed >= 6)
        {
            if (!NAV1_RTE_routeAdd(route, lat, lon, alt, ident, name, type))
                break;
        }
    }
    fclose(f);
    return 1;
}
