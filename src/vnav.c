#include "vnav.h"
#include <math.h>

NAV1_EXPORT int NAV1_VNAV_verticalProfile(double alt1Ft, double alt2Ft, double distNm, double gsKts, double *fpaDeg, double *vsFpm, double *timeSec)
{
    double altDiff = alt2Ft - alt1Ft;
    double distFt = distNm * 6076.12;

    if (distFt < 1.0 || gsKts < 0.1) return 0;

    if (fpaDeg) *fpaDeg = atan(altDiff / distFt) * 180.0 / 3.14159265358979323846;

    if (vsFpm) {
        double timeHours = distNm / gsKts;
        *vsFpm = altDiff / (timeHours * 60.0);
    }

    if (timeSec) {
        *timeSec = distNm / gsKts * 3600.0;
    }

    return 1;
}

NAV1_EXPORT double NAV1_VNAV_topOfClimb(double altAboveFieldFt, double rateFpm, double gsKts)
{
    if (rateFpm < 1.0 || gsKts < 0.1) return 0.0;
    double timeMinutes = altAboveFieldFt / rateFpm;
    double timeHours = timeMinutes / 60.0;
    return gsKts * timeHours;
}

NAV1_EXPORT double NAV1_VNAV_topOfDescent(double altAboveTargetFt, double rateFpm, double gsKts)
{
    if (rateFpm < 1.0 || gsKts < 0.1) return 0.0;
    double timeMinutes = altAboveTargetFt / rateFpm;
    double timeHours = timeMinutes / 60.0;
    return gsKts * timeHours;
}

NAV1_EXPORT double NAV1_VNAV_requiredVSAtConstraint(double currentAltFt, double targetAltFt, double distNm, double gsKts)
{
    if (distNm < 0.01 || gsKts < 0.1) return 0.0;
    double timeHours = distNm / gsKts;
    double timeMinutes = timeHours * 60.0;
    return (targetAltFt - currentAltFt) / timeMinutes;
}

NAV1_EXPORT double NAV1_VNAV_altitudeAtDistance(double startAltFt, double vsFpm, double gsKts, double distNm)
{
    if (gsKts < 0.1) return startAltFt;
    double timeMinutes = distNm / gsKts * 60.0;
    return startAltFt + vsFpm * timeMinutes;
}

NAV1_EXPORT double NAV1_VNAV_stepClimb(double currentAltFt, double targetAltFt, double rateFpm, double gsKts)
{
    if (rateFpm < 1.0 || gsKts < 0.1) return 0.0;
    double altDiff = targetAltFt - currentAltFt;
    if (altDiff <= 0.0) return 0.0;
    double timeMinutes = altDiff / rateFpm;
    double timeHours = timeMinutes / 60.0;
    return gsKts * timeHours;
}
