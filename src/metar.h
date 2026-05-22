#ifndef NAV1_METAR_H
#define NAV1_METAR_H

#include "nav1.h"

#define NAV1_METAR_MAX_CLOUDS  8
#define NAV1_METAR_MAX_WEATHER 6

typedef struct {
    int code;
    int altitudeFt;
    char type[8];
} NAV1_MetarCloud;

typedef struct {
    char intensity;
    char descriptor[8];
    char phenomena[8];
} NAV1_MetarWeather;

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
    NAV1_MetarWeather sky[NAV1_METAR_MAX_WEATHER];
    int skyCount;
    NAV1_MetarCloud clouds[NAV1_METAR_MAX_CLOUDS];
    int cloudCount;
    double tempC;
    double dewpointC;
    int qnhHPa;
    char trend[16];
    int valid;
} NAV1_MetarData;

NAV1_EXPORT int  NAV1_METAR_metarParse(const char *raw, NAV1_MetarData *out);
NAV1_EXPORT double NAV1_METAR_metarWindAvg(const NAV1_MetarData *m);
NAV1_EXPORT double NAV1_METAR_metarCrosswind(const NAV1_MetarData *m, double rwyHdg);
NAV1_EXPORT double NAV1_METAR_metarHeadwind(const NAV1_MetarData *m, double rwyHdg);

#endif
