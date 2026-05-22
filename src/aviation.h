#ifndef NAV1_AVIATION_H
#define NAV1_AVIATION_H

#include "nav1.h"

NAV_EXPORT void NAV_SUP_windCorrection(double trackDeg, double tasKts, double windDirDeg, double windSpeedKts, double *headingDeg, double *groundSpeedKts, double *wcaDeg);
NAV_EXPORT void NAV_SUP_windComponents(double windDirDeg, double windSpeedKts, double runwayHeadingDeg, double *crosswindKts, double *headwindKts);
NAV_EXPORT void NAV_SUP_isaAtmosphere(double altitudeFt, double *temperatureC, double *pressureHPa, double *densityKgM3);
NAV_EXPORT double NAV_SUP_pressureAltitude(double altitudeFt, double altimeterSettingInHg);
NAV_EXPORT double NAV_SUP_densityAltitude(double altitudeFt, double oatC, double altimeterSettingInHg);

#endif
