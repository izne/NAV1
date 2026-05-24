#include "hold.h"
#include <math.h>
#include <string.h>

#define NAV1_HOLD_PI 3.14159265358979323846

static double toRad(double deg) { return deg * NAV1_HOLD_PI / 180.0; }
static double toDeg(double rad) { return rad * 180.0 / NAV1_HOLD_PI; }

NAV1_EXPORT void NAV1_HOLD_patternInit(NAV1_HoldPattern *h, double fixLat, double fixLon, double inboundCourse, int rightTurn, double legDistNm)
{
    if (!h) return;
    h->fixLat = fixLat;
    h->fixLon = fixLon;
    h->inboundCourse = inboundCourse;
    h->rightTurn = rightTurn;
    h->legDistNm = legDistNm;
    h->windDirDeg = 0.0;
    h->windSpeedKts = 0.0;
}

NAV1_EXPORT double NAV1_HOLD_outboundHeading(const NAV1_HoldPattern *h)
{
    if (!h) return 0.0;
    double outbound = h->inboundCourse + (h->rightTurn ? 180.0 : -180.0);
    outbound = fmod(outbound + 360.0, 360.0);

    if (h->windSpeedKts > 0.1) {
        double windTowards = fmod(h->windDirDeg + 180.0, 360.0);
        double wca = toDeg(asin(h->windSpeedKts * sin(toRad(windTowards - outbound)) / 100.0));
        if (wca > 20.0) wca = 20.0;
        if (wca < -20.0) wca = -20.0;
        outbound = fmod(outbound + (h->rightTurn ? -wca : wca) + 360.0, 360.0);
    }
    return outbound;
}

NAV1_EXPORT void NAV1_HOLD_legPoints(const NAV1_HoldPattern *h, double *outboundLat, double *outboundLon, double *abeamLat, double *abeamLon)
{
    double outHdg, abeamHdg, dlat, dlon;

    if (!h) {
        if (outboundLat) *outboundLat = 0.0;
        if (outboundLon) *outboundLon = 0.0;
        if (abeamLat) *abeamLat = 0.0;
        if (abeamLon) *abeamLon = 0.0;
        return;
    }

    outHdg = NAV1_HOLD_outboundHeading(h);

    {
        double latR = toRad(h->fixLat);
        double lonR = toRad(h->fixLon);
        double brgR = toRad(outHdg);
        double distR = h->legDistNm * 1852.0 / 6371000.0;
        double sinLat = sin(latR);
        double cosLat = cos(latR);
        double sinDist = sin(distR);
        double cosDist = cos(distR);

        dlat = toDeg(asin(sinLat * cosDist + cosLat * sinDist * cos(brgR)));
        dlon = toDeg(lonR + atan2(sin(brgR) * sinDist * cosLat, cosDist - sinLat * sinLat));
    }

    if (outboundLat) *outboundLat = dlat;
    if (outboundLon) *outboundLon = dlon;

    abeamHdg = outHdg + (h->rightTurn ? -90.0 : 90.0);
    abeamHdg = fmod(abeamHdg + 360.0, 360.0);

    {
        double latR = toRad(h->fixLat);
        double lonR = toRad(h->fixLon);
        double brgR = toRad(abeamHdg);
        double distR = h->legDistNm * 1852.0 / 6371000.0;
        double sinLat = sin(latR);
        double cosLat = cos(latR);
        double sinDist = sin(distR);
        double cosDist = cos(distR);

        dlat = toDeg(asin(sinLat * cosDist + cosLat * sinDist * cos(brgR)));
        dlon = toDeg(lonR + atan2(sin(brgR) * sinDist * cosLat, cosDist - sinLat * sinLat));
    }

    if (abeamLat) *abeamLat = dlat;
    if (abeamLon) *abeamLon = dlon;
}

NAV1_EXPORT double NAV1_HOLD_maxHoldingTime(double fuelRemainingGal, double flowGph)
{
    if (flowGph < 0.1) return 0.0;
    return fuelRemainingGal / flowGph * 60.0;
}
