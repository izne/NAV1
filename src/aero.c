#include "aero.h"
#include <math.h>

static double toRad(double deg) { return deg * M_PI / 180; }
static double toDeg(double rad) { return rad * 180 / M_PI; }

NAV1_EXPORT void NAV1_AERO_windCorrection(double trackDeg, double tasKts, double windDirDeg, double windSpeedKts, double *headingDeg, double *groundSpeedKts, double *wcaDeg)
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

NAV1_EXPORT void NAV1_AERO_windComponents(double windDirDeg, double windSpeedKts, double runwayHeadingDeg, double *crosswindKts, double *headwindKts)
{
    const double windTowards = fmod(windDirDeg + 180.0, 360.0);
    const double aw = toRad(windTowards - runwayHeadingDeg);
    *crosswindKts = windSpeedKts * sin(aw);
    *headwindKts = -windSpeedKts * cos(aw);
}

NAV1_EXPORT void NAV1_AERO_isaAtmosphere(double altitudeFt, double *temperatureC, double *pressureHPa, double *densityKgM3)
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

NAV1_EXPORT double NAV1_AERO_pressureAltitude(double altitudeFt, double altimeterSettingInHg)
{
    return 145366.45 * (1.0 - pow(altimeterSettingInHg / 29.9213, 0.190261)) + altitudeFt;
}

NAV1_EXPORT double NAV1_AERO_densityAltitude(double altitudeFt, double oatC, double altimeterSettingInHg)
{
    const double PA = NAV1_AERO_pressureAltitude(altitudeFt, altimeterSettingInHg);
    const double isaTemp = 15.0 - 1.98 * PA / 1000.0;
    return PA + 120.0 * (oatC - isaTemp);
}

#define AERO_R     287.058
#define AERO_GAMMA 1.4

NAV1_EXPORT double NAV1_AERO_speedOfSound(double oatC)
{
    return sqrt(AERO_GAMMA * AERO_R * (oatC + 273.15)) * 1.94384;
}

NAV1_EXPORT double NAV1_AERO_machNumber(double tasKts, double oatC)
{
    return tasKts / NAV1_AERO_speedOfSound(oatC);
}

NAV1_EXPORT double NAV1_AERO_casToTas(double casKts, double pressureAltFt, double oatC)
{
    double t_isa, p_hPa, rho;
    NAV1_AERO_isaAtmosphere(pressureAltFt, &t_isa, &p_hPa, &rho);
    {
        const double T = oatC + 273.15;
        const double sigma = (p_hPa * 100.0) / (AERO_R * T) / 1.225;
        if (sigma < 0.01) return casKts * 10.0;
        return casKts / sqrt(sigma);
    }
}

NAV1_EXPORT void NAV1_AERO_windTriangle(double headingDeg, double tasKts, double windDirDeg, double windSpeedKts, double *groundTrackDeg, double *groundSpeedKts)
{
    const double windTowards = fmod(windDirDeg + 180.0, 360.0);
    const double aw = toRad(windTowards - headingDeg);
    const double crosswind = windSpeedKts * sin(aw);
    const double headwind  = -windSpeedKts * cos(aw);
    *groundSpeedKts = tasKts + headwind;
    const double drift = asin(crosswind / tasKts);
    *groundTrackDeg = fmod(headingDeg + toDeg(drift) + 360.0, 360.0);
}
