#ifndef NAV1_FLIGHT_MGMT_H
#define NAV1_FLIGHT_MGMT_H

#include "nav1.h"

NAV1_EXPORT double NAV1_FLT_timeToAltitude(double currentAltFt, double targetAltFt, double vsFpm);
NAV1_EXPORT double NAV1_FLT_requiredVerticalSpeed(double currentAltFt, double targetAltFt, double distNm, double gsKts);
NAV1_EXPORT double NAV1_FLT_topOfDescent(double currentAltFt, double targetAltFt, double vsFpm, double gsKts);
NAV1_EXPORT double NAV1_FLT_timeToWaypoint(double distNm, double gsKts);
NAV1_EXPORT double NAV1_FLT_fuelEndurance(double fuelGal, double flowGph);
NAV1_EXPORT double NAV1_FLT_fuelRange(double fuelGal, double flowGph, double gsKts);

NAV1_EXPORT double NAV1_FLT_fuelRequired(double distNm, double gsKts, double flowGph);
NAV1_EXPORT double NAV1_FLT_specificRange(double gsKts, double flowGph);

#endif
