#ifndef NAV1_FLIGHT_MGMT_H
#define NAV1_FLIGHT_MGMT_H

#include "nav1.h"

NAV_EXPORT double NAV1_FLT_timeToAltitude(double currentAltFt, double targetAltFt, double vsFpm);
NAV_EXPORT double NAV1_FLT_requiredVerticalSpeed(double currentAltFt, double targetAltFt, double distNm, double gsKts);
NAV_EXPORT double NAV1_FLT_topOfDescent(double currentAltFt, double targetAltFt, double vsFpm, double gsKts);
NAV_EXPORT double NAV1_FLT_timeToWaypoint(double distNm, double gsKts);
NAV_EXPORT double NAV1_FLT_fuelEndurance(double fuelGal, double flowGph);
NAV_EXPORT double NAV1_FLT_fuelRange(double fuelGal, double flowGph, double gsKts);

#endif
