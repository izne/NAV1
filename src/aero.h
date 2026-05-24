#ifndef NAV1_AERO_H
#define NAV1_AERO_H

#include "nav1.h"

NAV1_EXPORT void NAV1_AERO_windCorrection(double trackDeg, double tasKts, double windDirDeg, double windSpeedKts, double *headingDeg, double *groundSpeedKts, double *wcaDeg);
NAV1_EXPORT void NAV1_AERO_windComponents(double windDirDeg, double windSpeedKts, double runwayHeadingDeg, double *crosswindKts, double *headwindKts);
NAV1_EXPORT void NAV1_AERO_isaAtmosphere(double altitudeFt, double *temperatureC, double *pressureHPa, double *densityKgM3);
NAV1_EXPORT double NAV1_AERO_pressureAltitude(double altitudeFt, double altimeterSettingInHg);
NAV1_EXPORT double NAV1_AERO_densityAltitude(double altitudeFt, double oatC, double altimeterSettingInHg);

NAV1_EXPORT double NAV1_AERO_speedOfSound(double oatC);
NAV1_EXPORT double NAV1_AERO_machNumber(double tasKts, double oatC);
NAV1_EXPORT double NAV1_AERO_casToTas(double casKts, double pressureAltFt, double oatC);

NAV1_EXPORT void   NAV1_AERO_windTriangle(double headingDeg, double tasKts, double windDirDeg, double windSpeedKts, double *groundTrackDeg, double *groundSpeedKts);

NAV1_EXPORT double NAV1_AERO_turnRadius(double tasKts, double bankDeg);
NAV1_EXPORT double NAV1_AERO_turnRate(double tasKts, double bankDeg);
NAV1_EXPORT double NAV1_AERO_bankForRate(double tasKts, double rateDegPerSec);
NAV1_EXPORT double NAV1_AERO_flightPathAngle(double vsFpm, double gsKts);
NAV1_EXPORT double NAV1_AERO_vsFromFlightPathAngle(double fpaDeg, double gsKts);

NAV1_EXPORT double NAV1_AERO_reciprocalHeading(double heading);
NAV1_EXPORT double NAV1_AERO_standardRateTurnBank(double tasKts);
NAV1_EXPORT double NAV1_AERO_trueAltitude(double pressureAltitudeFt, double oatC);
NAV1_EXPORT double NAV1_AERO_tasToCas(double tasKts, double pressureAltFt, double oatC);
NAV1_EXPORT double NAV1_AERO_coldTemperatureAltCorrection(double indicatedAltFt, double oatC);

#endif
