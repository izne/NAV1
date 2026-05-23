#include "flight_mgmt.h"
#include <math.h>

NAV1_EXPORT double NAV1_FLT_timeToAltitude(double currentAltFt, double targetAltFt, double vsFpm)
{
    return (targetAltFt - currentAltFt) / vsFpm * 60.0;
}

NAV1_EXPORT double NAV1_FLT_requiredVerticalSpeed(double currentAltFt, double targetAltFt, double distNm, double gsKts)
{
    double timeHours = distNm / gsKts;
    double timeMinutes = timeHours * 60.0;
    return (targetAltFt - currentAltFt) / timeMinutes;
}

NAV1_EXPORT double NAV1_FLT_topOfDescent(double currentAltFt, double targetAltFt, double vsFpm, double gsKts)
{
    double altLoss = currentAltFt - targetAltFt;
    double timeMinutes = altLoss / vsFpm;
    double timeHours = timeMinutes / 60.0;
    return gsKts * timeHours;
}

NAV1_EXPORT double NAV1_FLT_timeToWaypoint(double distNm, double gsKts)
{
    return distNm / gsKts * 3600.0;
}

NAV1_EXPORT double NAV1_FLT_fuelEndurance(double fuelGal, double flowGph)
{
    return fuelGal / flowGph;
}

NAV1_EXPORT double NAV1_FLT_fuelRange(double fuelGal, double flowGph, double gsKts)
{
    return fuelGal / flowGph * gsKts;
}

NAV1_EXPORT double NAV1_FLT_fuelRequired(double distNm, double gsKts, double flowGph)
{
    return distNm / gsKts * flowGph;
}

NAV1_EXPORT double NAV1_FLT_specificRange(double gsKts, double flowGph)
{
    return gsKts / flowGph;
}
