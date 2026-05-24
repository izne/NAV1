#ifndef NAV1_WB_H
#define NAV1_WB_H

#include "nav1.h"

#define NAV1_WB_FUEL_AVGAS  0
#define NAV1_WB_FUEL_JETA   1

NAV1_EXPORT double NAV1_WB_moment(double weightKg, double armCm);
NAV1_EXPORT double NAV1_WB_cgFromMoments(double totalMomentKgCm, double totalWeightKg);
NAV1_EXPORT double NAV1_WB_fuelWeight(double volumeL, int type);
NAV1_EXPORT double NAV1_WB_fuelVolume(double weightKg, int type);
NAV1_EXPORT double NAV1_WB_cogAdd(double cgCm, double totalWeightKg, double addWeightKg, double addArmCm);
NAV1_EXPORT double NAV1_WB_cogRemove(double cgCm, double totalWeightKg, double removeWeightKg, double removeArmCm);
NAV1_EXPORT double NAV1_WB_weightToTonne(double weightKg);
NAV1_EXPORT double NAV1_WB_tonneToWeight(double tonnes);
NAV1_EXPORT int    NAV1_WB_cgEnvelopeCheck(double cgCm, double cgMinCm, double cgMaxCm);

#endif
