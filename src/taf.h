#ifndef NAV1_TAF_H
#define NAV1_TAF_H

#include "nav1.h"

#define NAV1_TAF_MAX_CLOUDS  8
#define NAV1_TAF_MAX_WEATHER 6
#define NAV1_TAF_MAX_PERIODS 6

typedef struct {
    int code;
    int altitudeFt;
    char type[8];
} NAV1_TafCloud;

typedef struct {
    char intensity;
    char descriptor[8];
    char phenomena[8];
} NAV1_TafWeather;

typedef struct {
    char changeType[8];
    int fromHour;
    int fromMin;
    int toHour;
    int toMin;
    int prob;
    int windDir;
    int windSpeedKt;
    int windGustKt;
    int visibilityM;
    int visibilitySM_num;
    int visibilitySM_den;
    NAV1_TafWeather sky[NAV1_TAF_MAX_WEATHER];
    int skyCount;
    NAV1_TafCloud clouds[NAV1_TAF_MAX_CLOUDS];
    int cloudCount;
} NAV1_TafPeriod;

typedef struct {
    char icao[8];
    int day;
    int hour;
    int minute;
    int validityDay;
    int validityHour;
    int validityMin;
    int windDir;
    int windSpeedKt;
    int windGustKt;
    double tempMinC;
    double tempMaxC;
    NAV1_TafPeriod periods[NAV1_TAF_MAX_PERIODS];
    int periodCount;
    int valid;
} NAV1_TafData;

NAV1_EXPORT int NAV1_TAF_tafParse(const char *raw, NAV1_TafData *out);
NAV1_EXPORT double NAV1_TAF_tafWindAvg(const NAV1_TafData *t);
NAV1_EXPORT double NAV1_TAF_tafCrosswind(const NAV1_TafData *t, double rwyHdg);
NAV1_EXPORT double NAV1_TAF_tafHeadwind(const NAV1_TafData *t, double rwyHdg);
NAV1_EXPORT int NAV1_TAF_tafCeiling(const NAV1_TafData *t);

#endif
