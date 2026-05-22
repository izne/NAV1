#ifndef NAV1_AERO_H
#define NAV1_AERO_H

#include "nav1.h"

NAV1_EXPORT void NAV1_AERO_windCorrection(double trackDeg, double tasKts, double windDirDeg, double windSpeedKts, double *headingDeg, double *groundSpeedKts, double *wcaDeg);
NAV1_EXPORT void NAV1_AERO_windComponents(double windDirDeg, double windSpeedKts, double runwayHeadingDeg, double *crosswindKts, double *headwindKts);
NAV1_EXPORT void NAV1_AERO_isaAtmosphere(double altitudeFt, double *temperatureC, double *pressureHPa, double *densityKgM3);
NAV1_EXPORT double NAV1_AERO_pressureAltitude(double altitudeFt, double altimeterSettingInHg);
NAV1_EXPORT double NAV1_AERO_densityAltitude(double altitudeFt, double oatC, double altimeterSettingInHg);

#endif
