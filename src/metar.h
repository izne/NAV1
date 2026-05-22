#ifndef NAV1_METAR_H
#define NAV1_METAR_H

#include "nav1.h"

#define NAV_METAR_MAX_CLOUDS  8
#define NAV_METAR_MAX_WEATHER 6

typedef struct {
    int code;
    int altitudeFt;
    char type[8];
} NAV_Sup_MetarCloud;

typedef struct {
    char intensity;
    char descriptor[8];
    char phenomena[8];
} NAV_Sup_MetarWeather;

typedef struct {
    char icao[8];
    int day;
    int hour;
    int minute;
    int windDir;
    int windSpeedKt;
    int windGustKt;
    int windVarLow;
    int windVarHigh;
    int visibilityM;
    int visibilitySM_num;
    int visibilitySM_den;
    char rvr[60];
    NAV_Sup_MetarWeather sky[NAV_METAR_MAX_WEATHER];
    int skyCount;
    NAV_Sup_MetarCloud clouds[NAV_METAR_MAX_CLOUDS];
    int cloudCount;
    double tempC;
    double dewpointC;
    int qnhHPa;
    char trend[16];
    int valid;
} NAV_Sup_MetarData;

NAV_EXPORT int  NAV_SUP_metarParse(const char *raw, NAV_Sup_MetarData *out);
NAV_EXPORT double NAV_SUP_metarWindAvg(const NAV_Sup_MetarData *m);
NAV_EXPORT double NAV_SUP_metarCrosswind(const NAV_Sup_MetarData *m, double rwyHdg);
NAV_EXPORT double NAV_SUP_metarHeadwind(const NAV_Sup_MetarData *m, double rwyHdg);

#endif
