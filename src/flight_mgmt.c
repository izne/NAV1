#include "flight_mgmt.h"
#include <math.h>

NAV_EXPORT double NAV1_FLT_timeToAltitude(double currentAltFt, double targetAltFt, double vsFpm)
{
    return (targetAltFt - currentAltFt) / vsFpm * 60.0;
}

NAV_EXPORT double NAV1_FLT_requiredVerticalSpeed(double currentAltFt, double targetAltFt, double distNm, double gsKts)
{
    double timeHours = distNm / gsKts;
    double timeMinutes = timeHours * 60.0;
    return (targetAltFt - currentAltFt) / timeMinutes;
}

NAV_EXPORT double NAV1_FLT_topOfDescent(double currentAltFt, double targetAltFt, double vsFpm, double gsKts)
{
    double altLoss = currentAltFt - targetAltFt;
    double timeMinutes = altLoss / vsFpm;
    double timeHours = timeMinutes / 60.0;
    return gsKts * timeHours;
}

NAV_EXPORT double NAV1_FLT_timeToWaypoint(double distNm, double gsKts)
{
    return distNm / gsKts * 3600.0;
}

NAV_EXPORT double NAV1_FLT_fuelEndurance(double fuelGal, double flowGph)
{
    return fuelGal / flowGph;
}

NAV_EXPORT double NAV1_FLT_fuelRange(double fuelGal, double flowGph, double gsKts)
{
    return fuelGal / flowGph * gsKts;
}
