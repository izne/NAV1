#ifndef NAV1_PERF_H
#define NAV1_PERF_H

#include "nav1.h"

#define NAV1_PERF_RUNWAY_DRY   0
#define NAV1_PERF_RUNWAY_WET   1
#define NAV1_PERF_RUNWAY_ICE   2
#define NAV1_PERF_RUNWAY_SOFT  3

NAV1_EXPORT double NAV1_PERF_climbGradient(double flightPathDeg);
NAV1_EXPORT double NAV1_PERF_fpaFromGradient(double gradientPct);
NAV1_EXPORT double NAV1_PERF_requiredRunwayLength(double dryLength, int condition);
NAV1_EXPORT double NAV1_PERF_crosswindExceedance(double windDirDeg, double windSpeedKts, double rwyHeadingDeg, double maxCrosswindKts);
NAV1_EXPORT double NAV1_PERF_headwindFraction(double windDirDeg, double windSpeedKts, double rwyHeadingDeg);

#endif
