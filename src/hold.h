#ifndef NAV1_HOLD_H
#define NAV1_HOLD_H

#include "nav1.h"

typedef struct {
    double fixLat;
    double fixLon;
    double inboundCourse;
    int rightTurn;
    double legDistNm;
    double windDirDeg;
    double windSpeedKts;
} NAV1_HoldPattern;

NAV1_EXPORT void NAV1_HOLD_patternInit(NAV1_HoldPattern *h, double fixLat, double fixLon, double inboundCourse, int rightTurn, double legDistNm);
NAV1_EXPORT double NAV1_HOLD_outboundHeading(const NAV1_HoldPattern *h);
NAV1_EXPORT void NAV1_HOLD_legPoints(const NAV1_HoldPattern *h, double *outboundLat, double *outboundLon, double *abeamLat, double *abeamLon);
NAV1_EXPORT double NAV1_HOLD_maxHoldingTime(double fuelRemainingGal, double flowGph);

#endif
