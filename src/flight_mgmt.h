#ifndef NAV1_FLIGHT_MGMT_H
#define NAV1_FLIGHT_MGMT_H

#include "nav1.h"

NAV_EXPORT double NAV_SUP_timeToAltitude(double currentAltFt, double targetAltFt, double vsFpm);
NAV_EXPORT double NAV_SUP_requiredVerticalSpeed(double currentAltFt, double targetAltFt, double distNm, double gsKts);
NAV_EXPORT double NAV_SUP_topOfDescent(double currentAltFt, double targetAltFt, double vsFpm, double gsKts);
NAV_EXPORT double NAV_SUP_timeToWaypoint(double distNm, double gsKts);
NAV_EXPORT double NAV_SUP_fuelEndurance(double fuelGal, double flowGph);
NAV_EXPORT double NAV_SUP_fuelRange(double fuelGal, double flowGph, double gsKts);

#endif
