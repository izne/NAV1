#ifndef NAV1_VNAV_H
#define NAV1_VNAV_H

#include "nav1.h"

NAV1_EXPORT int NAV1_VNAV_verticalProfile(double alt1Ft, double alt2Ft, double distNm, double gsKts, double *fpaDeg, double *vsFpm, double *timeSec);
NAV1_EXPORT double NAV1_VNAV_topOfClimb(double altAboveFieldFt, double rateFpm, double gsKts);
NAV1_EXPORT double NAV1_VNAV_topOfDescent(double altAboveTargetFt, double rateFpm, double gsKts);
NAV1_EXPORT double NAV1_VNAV_requiredVSAtConstraint(double currentAltFt, double targetAltFt, double distNm, double gsKts);
NAV1_EXPORT double NAV1_VNAV_altitudeAtDistance(double startAltFt, double vsFpm, double gsKts, double distNm);
NAV1_EXPORT double NAV1_VNAV_stepClimb(double currentAltFt, double targetAltFt, double rateFpm, double gsKts);

#endif
