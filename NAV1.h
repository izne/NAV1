#ifndef NAV1_H
#define NAV1_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NAV1_EXPORT
#define NAV1_EXPORT
#endif

/* ===================================================================
 *  NAV1 — Angelov NAV System, v1.0
 *  Public API header — single include for libNAV1.a consumers
 *
 *  All functions are pure C, no OS dependencies.
 *  Link with: gcc ... -L. -lNAV1
 * =================================================================== */

/* ------------------------------------------------------------------ */
/*  Core waypoint type                                                */
/* ------------------------------------------------------------------ */

#define NAV1_ROUTE_MAX_IDENT 8
#define NAV1_ROUTE_MAX_NAME 32

typedef struct {
    double latitude;
    double longitude;
    double altitudeFt;
    char ident[NAV1_ROUTE_MAX_IDENT];
    char name[NAV1_ROUTE_MAX_NAME];
    int type;
} NAV1_Waypoint;

/* ------------------------------------------------------------------ */
/*  Geodesy — NAV1_GEO_*                                              */
/* ------------------------------------------------------------------ */

NAV1_EXPORT double NAV1_GEO_distanceToTarget(double lat1, double lon1, double lat2, double lon2);
NAV1_EXPORT double NAV1_GEO_headingToTarget(double lat1, double lon1, double lat2, double lon2);
NAV1_EXPORT void   NAV1_GEO_destinationPoint(double lat1, double lon1, double bearing, double distNm, double *outLat, double *outLon);
NAV1_EXPORT void   NAV1_GEO_midpoint(double lat1, double lon1, double lat2, double lon2, double *outLat, double *outLon);
NAV1_EXPORT double NAV1_GEO_finalBearing(double lat1, double lon1, double lat2, double lon2);
NAV1_EXPORT double NAV1_GEO_crossTrackDistance(double lat1, double lon1, double lat2, double lon2, double lat3, double lon3);
NAV1_EXPORT double NAV1_GEO_dmsToDecimal(int degrees, int minutes, double seconds, char dir);
NAV1_EXPORT void   NAV1_GEO_decimalToDMS(double decimal, int *degrees, int *minutes, double *seconds);
NAV1_EXPORT void   NAV1_GEO_geodeticToEcef(double lat, double lon, double altM, double *x, double *y, double *z);
NAV1_EXPORT int    NAV1_GEO_ecefToGeodetic(double x, double y, double z, double *lat, double *lon, double *altM);

/* ------------------------------------------------------------------ */
/*  Unit conversions — NAV1_CONV_*                                    */
/* ------------------------------------------------------------------ */

NAV1_EXPORT double NAV1_CONV_mToFt(double meters);
NAV1_EXPORT double NAV1_CONV_ftToM(double feet);
NAV1_EXPORT double NAV1_CONV_nmToM(double nm);
NAV1_EXPORT double NAV1_CONV_mToNm(double meters);
NAV1_EXPORT double NAV1_CONV_nmToKm(double nm);
NAV1_EXPORT double NAV1_CONV_kmToNm(double km);
NAV1_EXPORT double NAV1_CONV_nmToMi(double nm);
NAV1_EXPORT double NAV1_CONV_miToNm(double mi);
NAV1_EXPORT double NAV1_CONV_kmToMi(double km);
NAV1_EXPORT double NAV1_CONV_miToKm(double mi);
NAV1_EXPORT double NAV1_CONV_knotsToMs(double knots);
NAV1_EXPORT double NAV1_CONV_msToKnots(double ms);
NAV1_EXPORT double NAV1_CONV_knotsToKmh(double knots);
NAV1_EXPORT double NAV1_CONV_kmhToKnots(double kmh);
NAV1_EXPORT double NAV1_CONV_cToF(double celsius);
NAV1_EXPORT double NAV1_CONV_fToC(double fahrenheit);
NAV1_EXPORT double NAV1_CONV_hPaToInHg(double hPa);
NAV1_EXPORT double NAV1_CONV_inHgTohPa(double inHg);
NAV1_EXPORT double NAV1_CONV_hPaToPsi(double hPa);
NAV1_EXPORT double NAV1_CONV_psiTohPa(double psi);

/* ------------------------------------------------------------------ */
/*  Advanced geodesy — NAV1_NAV_*                                     */
/* ------------------------------------------------------------------ */

NAV1_EXPORT double NAV1_NAV_alongTrackDistance(double lat1, double lon1, double lat2, double lon2, double lat3, double lon3);
NAV1_EXPORT void   NAV1_NAV_greatCircleWaypoints(double lat1, double lon1, double lat2, double lon2, int count, double *outLats, double *outLons);
NAV1_EXPORT double NAV1_NAV_rhumbDistance(double lat1, double lon1, double lat2, double lon2);
NAV1_EXPORT double NAV1_NAV_rhumbBearing(double lat1, double lon1, double lat2, double lon2);
NAV1_EXPORT void   NAV1_NAV_antipodalPoint(double lat, double lon, double *outLat, double *outLon);
NAV1_EXPORT void   NAV1_NAV_bearingToCompass(double bearing, char *out, int outSize);
NAV1_EXPORT int    NAV1_NAV_closestPointOnCourse(double lat1, double lon1, double lat2, double lon2, double lat3, double lon3, double *outLat, double *outLon);

/* ------------------------------------------------------------------ */
/*  Aviation math — NAV1_AERO_*                                       */
/* ------------------------------------------------------------------ */

NAV1_EXPORT void   NAV1_AERO_windCorrection(double trackDeg, double tasKts, double windDirDeg, double windSpeedKts, double *headingDeg, double *groundSpeedKts, double *wcaDeg);
NAV1_EXPORT void   NAV1_AERO_windComponents(double windDirDeg, double windSpeedKts, double runwayHeadingDeg, double *crosswindKts, double *headwindKts);
NAV1_EXPORT void   NAV1_AERO_isaAtmosphere(double altitudeFt, double *temperatureC, double *pressureHPa, double *densityKgM3);
NAV1_EXPORT double NAV1_AERO_pressureAltitude(double altitudeFt, double altimeterSettingInHg);
NAV1_EXPORT double NAV1_AERO_densityAltitude(double altitudeFt, double oatC, double altimeterSettingInHg);
NAV1_EXPORT double NAV1_AERO_speedOfSound(double oatC);
NAV1_EXPORT double NAV1_AERO_machNumber(double tasKts, double oatC);
NAV1_EXPORT double NAV1_AERO_casToTas(double casKts, double pressureAltFt, double oatC);

/* ------------------------------------------------------------------ */
/*  GPS helpers — NAV1_GPS_*                                          */
/* ------------------------------------------------------------------ */

NAV1_EXPORT double NAV1_GPS_dmmToDecimal(int degrees, double minutes, char dir);
NAV1_EXPORT void   NAV1_GPS_decimalToDMM(double decimal, double *minutes, char *dir);
NAV1_EXPORT double NAV1_GPS_gpsSpeed(double lat1, double lon1, double lat2, double lon2, double timeSeconds);
NAV1_EXPORT int    NAV1_GPS_nmeaChecksum(const char *sentence);
NAV1_EXPORT int    NAV1_GPS_isValidLatitude(double lat);
NAV1_EXPORT int    NAV1_GPS_isValidLongitude(double lon);

/* ------------------------------------------------------------------ */
/*  Flight management — NAV1_FLT_*                                    */
/* ------------------------------------------------------------------ */

NAV1_EXPORT double NAV1_FLT_timeToAltitude(double currentAltFt, double targetAltFt, double vsFpm);
NAV1_EXPORT double NAV1_FLT_requiredVerticalSpeed(double currentAltFt, double targetAltFt, double distNm, double gsKts);
NAV1_EXPORT double NAV1_FLT_topOfDescent(double currentAltFt, double targetAltFt, double vsFpm, double gsKts);
NAV1_EXPORT double NAV1_FLT_timeToWaypoint(double distNm, double gsKts);
NAV1_EXPORT double NAV1_FLT_fuelEndurance(double fuelGal, double flowGph);
NAV1_EXPORT double NAV1_FLT_fuelRange(double fuelGal, double flowGph, double gsKts);

/* ------------------------------------------------------------------ */
/*  Signal processing — NAV1_CTL_*                                    */
/* ------------------------------------------------------------------ */

typedef struct {
    double y;
} NAV1_LowPassFilter;

NAV1_EXPORT void   NAV1_CTL_lpfInit(NAV1_LowPassFilter *f, double initial);
NAV1_EXPORT double NAV1_CTL_lpfUpdate(NAV1_LowPassFilter *f, double x, double alpha);

typedef struct {
    double last;
} NAV1_SlewLimiter;

NAV1_EXPORT void   NAV1_CTL_slewInit(NAV1_SlewLimiter *s, double initial);
NAV1_EXPORT double NAV1_CTL_slewUpdate(NAV1_SlewLimiter *s, double x, double maxRate, double dt);

typedef struct {
    double *buffer;
    int size;
    int index;
    int count;
    double sum;
} NAV1_MovingAverage;

NAV1_EXPORT void   NAV1_CTL_maInit(NAV1_MovingAverage *ma, double *buffer, int size);
NAV1_EXPORT double NAV1_CTL_maUpdate(NAV1_MovingAverage *ma, double x);

typedef struct {
    double Kp, Ki, Kd;
    double integral;
    double lastError;
    double outMin, outMax;
} NAV1_PIDController;

NAV1_EXPORT void   NAV1_CTL_pidInit(NAV1_PIDController *pid, double Kp, double Ki, double Kd, double outMin, double outMax);
NAV1_EXPORT double NAV1_CTL_pidUpdate(NAV1_PIDController *pid, double setpoint, double input, double dt);

typedef struct {
    double angle;
} NAV1_ComplementaryFilter;

NAV1_EXPORT void   NAV1_CTL_cfInit(NAV1_ComplementaryFilter *f, double initial);
NAV1_EXPORT double NAV1_CTL_cfUpdate(NAV1_ComplementaryFilter *f, double accelAngle, double gyroRate, double dt, double alpha);

/* ------------------------------------------------------------------ */
/*  NMEA parser — NAV1_NMEA_*                                         */
/* ------------------------------------------------------------------ */

typedef struct {
    int    hasFix;
    double latitude;
    double longitude;
    double altitudeM;
    double geoidSepM;
    int    fixQuality;
    int    numSats;
    double hdop;
    double speedKts;
    double trackDeg;
    double magVarDeg;
    char   timeUTC[10];
    char   dateUTC[7];
} NAV1_NMEAData;

NAV1_EXPORT int NAV1_NMEA_nmeaParse(const char *sentence, NAV1_NMEAData *out);

/* ------------------------------------------------------------------ */
/*  Route management — NAV1_RTE_*                                     */
/* ------------------------------------------------------------------ */

typedef struct {
    NAV1_Waypoint *waypoints;
    int count;
    int capacity;
    int currentIndex;
} NAV1_Route;

NAV1_EXPORT void NAV1_RTE_routeInit(NAV1_Route *route, NAV1_Waypoint *buffer, int capacity);
NAV1_EXPORT int  NAV1_RTE_routeAdd(NAV1_Route *route, double lat, double lon, double altFt, const char *ident, const char *name, int type);
NAV1_EXPORT int  NAV1_RTE_routeInsert(NAV1_Route *route, int index, double lat, double lon, double altFt, const char *ident, const char *name, int type);
NAV1_EXPORT int  NAV1_RTE_routeRemove(NAV1_Route *route, int index);
NAV1_EXPORT void NAV1_RTE_routeClear(NAV1_Route *route);
NAV1_EXPORT int  NAV1_RTE_routeSequence(NAV1_Route *route);
NAV1_EXPORT int  NAV1_RTE_routeDirectTo(NAV1_Route *route, int index);
NAV1_EXPORT int  NAV1_RTE_routeLegInfo(const NAV1_Route *route, int index, double *outBearingDeg, double *outDistNm);
NAV1_EXPORT int  NAV1_RTE_routeTurnAnticipation(const NAV1_Route *route, double groundSpeedKts, double turnRateDegS, double *outLat, double *outLon);
NAV1_EXPORT int  NAV1_RTE_routeSaveCSV(const char *filename, const NAV1_Route *route);
NAV1_EXPORT int  NAV1_RTE_routeLoadCSV(const char *filename, NAV1_Waypoint *buffer, int capacity, NAV1_Route *route);

/* ------------------------------------------------------------------ */
/*  X-Plane nav database + ICAO FPL — NAV1_XPL_*                      */
/* ------------------------------------------------------------------ */

#define NAV1_NAV_VOR       2
#define NAV1_NAV_NDB       3
#define NAV1_NAV_DME       4
#define NAV1_NAV_ILS       5
#define NAV1_NAV_LOCALIZER 6
#define NAV1_NAV_GS        7
#define NAV1_NAV_OMNI      8
#define NAV1_NAV_FIX      255

typedef struct {
    int type;
    char ident[8];
    char name[40];
    double latitude;
    double longitude;
    double frequency;
    double elevationFt;
} NAV1_NavRecord;

typedef struct {
    NAV1_NavRecord *records;
    int count;
    int capacity;
} NAV1_NavDatabase;

NAV1_EXPORT int  NAV1_XPL_xpNavLoad(const char *filename, NAV1_NavDatabase *db);
NAV1_EXPORT int  NAV1_XPL_xpFixLoad(const char *filename, NAV1_NavDatabase *db);
NAV1_EXPORT int  NAV1_XPL_xpNavFindByID(const NAV1_NavDatabase *db, const char *ident, int startIndex, NAV1_NavRecord *out);
NAV1_EXPORT int  NAV1_XPL_xpNavFindNearest(const NAV1_NavDatabase *db, double lat, double lon, int typeFilter, NAV1_NavRecord *out);
NAV1_EXPORT int  NAV1_XPL_fplParseRoute(const char *str, const NAV1_NavDatabase *navdb, const NAV1_NavDatabase *fixdb, NAV1_Waypoint *waypoints, int maxCount);
NAV1_EXPORT void NAV1_XPL_xpNavFree(NAV1_NavDatabase *db);

/* ------------------------------------------------------------------ */
/*  ARINC 429 decoder — NAV1_A429_*                                   */
/* ------------------------------------------------------------------ */

#define NAV1_A429_ENCODING_BNR   0
#define NAV1_A429_ENCODING_BCD   1
#define NAV1_A429_ENCODING_DISC  2
#define NAV1_A429_ENCODING_UNKN  3

#define NAV1_A429_SSM_NORMAL     3
#define NAV1_A429_SSM_TEST       2
#define NAV1_A429_SSM_NODATA     1
#define NAV1_A429_SSM_FAIL       0

typedef struct {
    unsigned int label;
    unsigned int sdi;
    unsigned int ssm;
    unsigned int dataField;
    int  parityOK;
    int  encoding;
    double value;
    char name[40];
    char unit[12];
} NAV1_ARINC429Word;

NAV1_EXPORT void         NAV1_A429_a429Decode(unsigned int word, NAV1_ARINC429Word *out);
NAV1_EXPORT unsigned int NAV1_A429_a429Label(unsigned int word);
NAV1_EXPORT unsigned int NAV1_A429_a429SDI(unsigned int word);
NAV1_EXPORT unsigned int NAV1_A429_a429Data(unsigned int word);
NAV1_EXPORT unsigned int NAV1_A429_a429SSM(unsigned int word);
NAV1_EXPORT int          NAV1_A429_a429ParityCheck(unsigned int word);
NAV1_EXPORT double       NAV1_A429_a429DataBNR(unsigned int dataField, double lsbWeight);
NAV1_EXPORT double       NAV1_A429_a429DataBCD(unsigned int dataField, int digitCount);
NAV1_EXPORT int          NAV1_A429_a429LabelInfo(unsigned int label, char *outName, char *outUnit, int *outEncoding, double *outLsbWeight, int *outIsSigned);
NAV1_EXPORT unsigned int NAV1_A429_a429Encode(unsigned int label, unsigned int sdi, unsigned int dataField, unsigned int ssm);
NAV1_EXPORT unsigned int NAV1_A429_a429SetParity(unsigned int word);
NAV1_EXPORT unsigned int NAV1_A429_a429EncodeBNR(unsigned int label, unsigned int sdi, double value, double lsbWeight, int isSigned, unsigned int ssm);
NAV1_EXPORT unsigned int NAV1_A429_a429EncodeBCD(unsigned int label, unsigned int sdi, double value, int digitCount, unsigned int ssm);

/* ------------------------------------------------------------------ */
/*  ARINC 424 nav database parser — NAV1_A424_*                       */
/* ------------------------------------------------------------------ */

#define NAV1_A424_ICAO_LEN   5
#define NAV1_A424_IDENT_LEN  6
#define NAV1_A424_NAME_LEN   31

typedef struct {
    char icao[NAV1_A424_ICAO_LEN];
    char ident[NAV1_A424_IDENT_LEN];
    char name[NAV1_A424_NAME_LEN];
    double latitude;
    double longitude;
    double frequency;
    double elevationFt;
    int navType;
    char classCode[4];
} NAV1_A424Navaid;

typedef struct {
    char icao[NAV1_A424_ICAO_LEN];
    char ident[NAV1_A424_IDENT_LEN];
    double latitude;
    double longitude;
    int waypointType;
    char usage[4];
} NAV1_A424Waypoint;

typedef struct {
    char icao[NAV1_A424_ICAO_LEN];
    char iata[5];
    char name[NAV1_A424_NAME_LEN];
    double latitude;
    double longitude;
    double elevationFt;
    double magVarDeg;
    char datumCode[4];
    char tz[4];
    int longestRunwayFt;
    int speedLimitKt;
    int transitionAltFt;
    int transitionLvlFt;
    char ifrCapability;
    char publicMilitary;
} NAV1_A424Airport;

typedef struct {
    char icao[NAV1_A424_ICAO_LEN];
    char rwyIdent[NAV1_A424_IDENT_LEN];
    double latitude;
    double longitude;
    double lengthFt;
    double widthFt;
    double bearingDeg;
    double thresholdElevFt;
    char surfaceCode[3];
    char description[23];
} NAV1_A424Runway;

typedef struct {
    char icao[NAV1_A424_ICAO_LEN];
    char rwyIdent[NAV1_A424_IDENT_LEN];
    char ident[NAV1_A424_IDENT_LEN];
    double latitude;
    double longitude;
    double frequency;
    int category;
    double bearingDeg;
} NAV1_A424ILS;

typedef struct {
    NAV1_A424Navaid *navaids;
    NAV1_A424Waypoint *waypoints;
    NAV1_A424Airport *airports;
    NAV1_A424Runway *runways;
    NAV1_A424ILS *ils;
    int nnavaids;
    int nwaypoints;
    int nairports;
    int nrunways;
    int nils;
    int capacity;
} NAV1_A424Database;

NAV1_EXPORT int  NAV1_A424_a424ParseFile(const char *filename, NAV1_A424Database *db);
NAV1_EXPORT void NAV1_A424_a424Free(NAV1_A424Database *db);

/* ------------------------------------------------------------------ */
/*  METAR weather parser — NAV1_METAR_*                               */
/* ------------------------------------------------------------------ */

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

NAV1_EXPORT int    NAV1_METAR_metarParse(const char *raw, NAV1_MetarData *out);
NAV1_EXPORT double NAV1_METAR_metarWindAvg(const NAV1_MetarData *m);
NAV1_EXPORT double NAV1_METAR_metarCrosswind(const NAV1_MetarData *m, double rwyHdg);
NAV1_EXPORT double NAV1_METAR_metarHeadwind(const NAV1_MetarData *m, double rwyHdg);

#ifdef __cplusplus
}
#endif

#endif /* NAV1_H */