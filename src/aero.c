#include "aero.h"
#include <math.h>

static double toRad(double deg) { return deg * M_PI / 180; }
static double toDeg(double rad) { return rad * 180 / M_PI; }

NAV_EXPORT void NAV1_AERO_windCorrection(double trackDeg, double tasKts, double windDirDeg, double windSpeedKts, double *headingDeg, double *groundSpeedKts, double *wcaDeg)
{
    const double trackR = toRad(trackDeg);
    const double windTowards = fmod(windDirDeg + 180.0, 360.0);
    const double windR = toRad(windTowards);
    const double aw = windR - trackR;
    const double xw = windSpeedKts * sin(aw);

    double arg = xw / tasKts;
    if (arg > 1.0) arg = 1.0;
    if (arg < -1.0) arg = -1.0;
    const double wcaR = asin(arg);

    const double hw = windSpeedKts * cos(aw);

    *wcaDeg = -toDeg(wcaR);
    *headingDeg = fmod(trackDeg + *wcaDeg + 360, 360);
    *groundSpeedKts = tasKts * cos(wcaR) + hw * cos(wcaR);
    if (*groundSpeedKts < 0) *groundSpeedKts = 0;
}

NAV_EXPORT void NAV1_AERO_windComponents(double windDirDeg, double windSpeedKts, double runwayHeadingDeg, double *crosswindKts, double *headwindKts)
{
    const double windTowards = fmod(windDirDeg + 180.0, 360.0);
    const double aw = toRad(windTowards - runwayHeadingDeg);
    *crosswindKts = windSpeedKts * sin(aw);
    *headwindKts = -windSpeedKts * cos(aw);
}

NAV_EXPORT void NAV1_AERO_isaAtmosphere(double altitudeFt, double *temperatureC, double *pressureHPa, double *densityKgM3)
{
    const double h = altitudeFt * 0.3048;
    if (h < 11000.0)
    {
        *temperatureC = 15.0 - 0.0065 * h;
        const double theta = 1.0 - 0.0065 * h / 288.15;
        *pressureHPa = 1013.25 * pow(theta, 5.2561);
        *densityKgM3 = 1.225 * pow(theta, 4.2561);
    }
    else
    {
        *temperatureC = -56.5;
        const double h_km = (h - 11000.0);
        *pressureHPa = 226.32 * exp(-0.0001577 * h_km);
        *densityKgM3 = 0.3639 * exp(-0.0001577 * h_km);
    }
}

NAV_EXPORT double NAV1_AERO_pressureAltitude(double altitudeFt, double altimeterSettingInHg)
{
    return 145366.45 * (1.0 - pow(altimeterSettingInHg / 29.9213, 0.190261)) + altitudeFt;
}

NAV_EXPORT double NAV1_AERO_densityAltitude(double altitudeFt, double oatC, double altimeterSettingInHg)
{
    const double PA = NAV1_AERO_pressureAltitude(altitudeFt, altimeterSettingInHg);
    const double isaTemp = 15.0 - 1.98 * PA / 1000.0;
    return PA + 120.0 * (oatC - isaTemp);
}
