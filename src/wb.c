#include "wb.h"

#define AVGAS_KG_PER_L  0.72
#define JETA_KG_PER_L   0.804

NAV1_EXPORT double NAV1_WB_moment(double weightKg, double armCm)
{
    return weightKg * armCm;
}

NAV1_EXPORT double NAV1_WB_cgFromMoments(double totalMomentKgCm, double totalWeightKg)
{
    if (totalWeightKg < 1e-12) return 0.0;
    return totalMomentKgCm / totalWeightKg;
}

NAV1_EXPORT double NAV1_WB_fuelWeight(double volumeL, int type)
{
    double density = (type == NAV1_WB_FUEL_JETA) ? JETA_KG_PER_L : AVGAS_KG_PER_L;
    return volumeL * density;
}

NAV1_EXPORT double NAV1_WB_fuelVolume(double weightKg, int type)
{
    double density = (type == NAV1_WB_FUEL_JETA) ? JETA_KG_PER_L : AVGAS_KG_PER_L;
    if (density < 1e-12) return 0.0;
    return weightKg / density;
}

NAV1_EXPORT double NAV1_WB_cogAdd(double cgCm, double totalWeightKg, double addWeightKg, double addArmCm)
{
    double newWeight = totalWeightKg + addWeightKg;
    if (newWeight < 1e-12) return 0.0;
    return (totalWeightKg * cgCm + addWeightKg * addArmCm) / newWeight;
}

NAV1_EXPORT double NAV1_WB_cogRemove(double cgCm, double totalWeightKg, double removeWeightKg, double removeArmCm)
{
    double newWeight = totalWeightKg - removeWeightKg;
    if (newWeight < 1e-12) return 0.0;
    return (totalWeightKg * cgCm - removeWeightKg * removeArmCm) / newWeight;
}

NAV1_EXPORT double NAV1_WB_weightToTonne(double weightKg)
{
    return weightKg / 1000.0;
}

NAV1_EXPORT double NAV1_WB_tonneToWeight(double tonnes)
{
    return tonnes * 1000.0;
}

NAV1_EXPORT int NAV1_WB_cgEnvelopeCheck(double cgCm, double cgMinCm, double cgMaxCm)
{
    if (cgMaxCm <= cgMinCm) return 0;
    return (cgCm >= cgMinCm && cgCm <= cgMaxCm) ? 1 : 0;
}
