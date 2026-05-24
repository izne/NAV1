#include "perf.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

NAV1_EXPORT double NAV1_PERF_climbGradient(double flightPathDeg)
{
    return tan(flightPathDeg * M_PI / 180.0) * 100.0;
}

NAV1_EXPORT double NAV1_PERF_fpaFromGradient(double gradientPct)
{
    return atan(gradientPct / 100.0) * 180.0 / M_PI;
}

NAV1_EXPORT double NAV1_PERF_requiredRunwayLength(double dryLength, int condition)
{
    double multiplier;
    switch (condition)
    {
        case NAV1_PERF_RUNWAY_WET:  multiplier = 1.3; break;
        case NAV1_PERF_RUNWAY_ICE:  multiplier = 2.0; break;
        case NAV1_PERF_RUNWAY_SOFT: multiplier = 1.5; break;
        default:                    multiplier = 1.0; break;
    }
    return dryLength * multiplier;
}

NAV1_EXPORT double NAV1_PERF_crosswindExceedance(double windDirDeg, double windSpeedKts, double rwyHeadingDeg, double maxCrosswindKts)
{
    double xw, hw;
    NAV1_AERO_windComponents(windDirDeg, windSpeedKts, rwyHeadingDeg, &xw, &hw);
    double excess = fabs(xw) - maxCrosswindKts;
    return (excess > 0) ? excess : 0.0;
}

NAV1_EXPORT double NAV1_PERF_headwindFraction(double windDirDeg, double windSpeedKts, double rwyHeadingDeg)
{
    double xw, hw;
    if (windSpeedKts < 0.1) return 0.0;
    NAV1_AERO_windComponents(windDirDeg, windSpeedKts, rwyHeadingDeg, &xw, &hw);
    return hw / windSpeedKts;
}
