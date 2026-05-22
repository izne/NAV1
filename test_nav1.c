#include "nav1.h"
#include "conv.h"
#include "nav.h"
#include "aviation.h"
#include "gps.h"
#include "flight_mgmt.h"
#include "control.h"
#include "nmea.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

static int tests_run = 0;
static int tests_passed = 0;

#define TEST_NEAR(desc, actual, expected, tol) do { \
    tests_run++; \
    double a = (actual); \
    double e = (expected); \
    double d = fabs(a - e); \
    if (d <= (tol)) { \
        printf("  PASS  %s = %.6f\n", (desc), a); \
        tests_passed++; \
    } else { \
        printf("  FAIL  %s = %.6f (expected ~%.6f, diff %.6f)\n", (desc), a, e, d); \
    } \
} while(0)

#define TEST_TRUE(desc, cond) do { \
    tests_run++; \
    if (cond) { \
        printf("  PASS  %s\n", (desc)); \
        tests_passed++; \
    } else { \
        printf("  FAIL  %s\n", (desc)); \
    } \
} while(0)

#define TEST_STR(desc, actual, expected) do { \
    tests_run++; \
    if (strcmp((actual), (expected)) == 0) { \
        printf("  PASS  %s = %s\n", (desc), (actual)); \
        tests_passed++; \
    } else { \
        printf("  FAIL  %s = %s (expected %s)\n", (desc), (actual), (expected)); \
    } \
} while(0)

int main()
{
    printf("=== NAV1 Test Suite ===\n\n");

    printf("--- ARINC 424 Module (early) ---\n");
    {
        NAV_Sup_A424Database db;
        int ok = NAV_SUP_a424ParseFile("_noexist.a424", &db);
        TEST_TRUE("a424 missing", !ok);
    }
    {
        char buf[200];
        FILE *f = fopen("_ta424.dat", "w");
        memset(buf, 32, 199); buf[199] = 0;
        memcpy(buf, "SUSAPA", 6);
        memcpy(buf+6,  "KSEA", 4);
        memcpy(buf+10, "K1", 2);
        memcpy(buf+13, "SEA", 3);
        buf[21] = '1';
        memcpy(buf+22, "10000", 5);
        memcpy(buf+27, "119", 3);
        buf[30] = 'Y';
        memcpy(buf+32, "N47265700", 9);
        memcpy(buf+41, "W122182910", 10);
        memcpy(buf+51, "E0199", 5);
        memcpy(buf+56, "00429", 5);
        memcpy(buf+61, "250", 3);
        memcpy(buf+64, "SEA ", 4);
        memcpy(buf+68, "K1", 2);
        memcpy(buf+70, "18000", 5);
        memcpy(buf+75, "18000", 5);
        buf[80] = 'C';
        memcpy(buf+86, "NAS", 3);
        memcpy(buf+93, "SEATTLE-TACOMA INTL           ", 30);
        memcpy(buf+123, "04569", 5);
        memcpy(buf+128, "8808", 4);
        buf[132] = '\n'; buf[133] = 0;
        fputs(buf, f);
        fclose(f);
        NAV_Sup_A424Database db;
        int ok = NAV_SUP_a424ParseFile("_ta424.dat", &db);
        TEST_TRUE("a424 early ok", ok);
        TEST_TRUE("a424 early nairports", db.nairports == 1);
        NAV_SUP_a424Free(&db);
        remove("_ta424.dat");
    }

    printf("--- Distance & Bearing ---\n");
    TEST_NEAR("distanceToTarget(53,10,43,27) NM", NAV_SUP_distanceToTarget(53.0, 10.0, 43.0, 27.0), 905.0, 5.0);
    double hdg = NAV_SUP_headingToTarget(53.0, 10.0, 43.0, 27.0);
    TEST_NEAR("headingToTarget(53,10,43,27) deg", hdg, 125.0, 5.0);
    TEST_NEAR("finalBearing(53,10,43,27) deg", NAV_SUP_finalBearing(53.0, 10.0, 43.0, 27.0), 137.0, 5.0);

    printf("\n--- Destination Point ---\n");
    double dlat, dlon;
    NAV_SUP_destinationPoint(0.0, 0.0, 0.0, 60.0, &dlat, &dlon);
    TEST_NEAR("destPoint(0,0,N,60NM) lat", dlat, 1.0, 0.1);
    TEST_NEAR("destPoint(0,0,N,60NM) lon", dlon, 0.0, 0.1);
    NAV_SUP_destinationPoint(0.0, 0.0, 90.0, 60.0, &dlat, &dlon);
    TEST_NEAR("destPoint(0,0,E,60NM) lat", dlat, 0.0, 0.1);
    TEST_NEAR("destPoint(0,0,E,60NM) lon", dlon, 1.0, 0.1);

    printf("\n--- Midpoint ---\n");
    double mlat, mlon;
    NAV_SUP_midpoint(52.0, 0.0, 53.0, 1.0, &mlat, &mlon);
    TEST_NEAR("midpoint(52,0,53,1) lat", mlat, 52.5, 0.5);
    TEST_NEAR("midpoint(52,0,53,1) lon", mlon, 0.5, 0.5);

    printf("\n--- Cross-Track Distance ---\n");
    double xtd = NAV_SUP_crossTrackDistance(0.0, 0.0, 10.0, 0.0, 5.0, 0.5);
    TEST_NEAR("crossTrack(0,0->10,0 @5,0.5) NM", fabs(xtd), 30.0, 2.0);

    printf("\n--- Along-Track Distance ---\n");
    double atd = NAV_SUP_alongTrackDistance(0.0, 0.0, 10.0, 0.0, 5.0, 0.5);
    TEST_NEAR("alongTrack(0,0->10,0 @5,0.5) NM", atd, 300.0, 3.0);

    printf("\n--- Great Circle Waypoints ---\n");
    {
        double lats[5], lons[5];
        NAV_SUP_greatCircleWaypoints(52.0, 0.0, 53.0, 1.0, 5, lats, lons);
        TEST_NEAR("waypoints[0] lat", lats[0], 52.0, 0.01);
        TEST_NEAR("waypoints[0] lon", lons[0], 0.0, 0.01);
        TEST_NEAR("waypoints[2] lat", lats[2], 52.5, 0.5);
        TEST_NEAR("waypoints[2] lon", lons[2], 0.5, 0.5);
        TEST_NEAR("waypoints[4] lat", lats[4], 53.0, 0.01);
        TEST_NEAR("waypoints[4] lon", lons[4], 1.0, 0.01);
    }

    printf("\n--- Rhumb Line ---\n");
    TEST_NEAR("rhumbDist(0,0,10,0) NM", NAV_SUP_rhumbDistance(0.0, 0.0, 10.0, 0.0), 600.0, 1.0);
    TEST_NEAR("rhumbBrg(0,0,0,10) deg (along equator)", NAV_SUP_rhumbBearing(0.0, 0.0, 0.0, 10.0), 90.0, 0.1);
    TEST_NEAR("rhumbBrg(0,0,10,0) deg (north)", NAV_SUP_rhumbBearing(0.0, 0.0, 10.0, 0.0), 0.0, 0.1);
    TEST_NEAR("rhumbBrg(10,0,0,0) deg (south)", NAV_SUP_rhumbBearing(10.0, 0.0, 0.0, 0.0), 180.0, 0.1);
    TEST_NEAR("rhumbDist(0,0,0,10) NM (along equator)", NAV_SUP_rhumbDistance(0.0, 0.0, 0.0, 10.0), 600.0, 2.0);

    printf("\n--- Antipodal Point ---\n");
    NAV_SUP_antipodalPoint(50.0, 30.0, &dlat, &dlon);
    TEST_NEAR("antipode lat", dlat, -50.0, 0.1);
    TEST_NEAR("antipode lon", dlon, -150.0, 0.1);
    NAV_SUP_antipodalPoint(-33.0, -70.0, &dlat, &dlon);
    TEST_NEAR("antipode(Santiago) lat", dlat, 33.0, 0.1);
    TEST_NEAR("antipode(Santiago) lon", dlon, 110.0, 0.1);

    printf("\n--- Compass Direction ---\n");
    {
        char buf[8];
        NAV_SUP_bearingToCompass(0.0, buf, sizeof(buf));
        TEST_STR("bearing 0", buf, "N");
        NAV_SUP_bearingToCompass(90.0, buf, sizeof(buf));
        TEST_STR("bearing 90", buf, "E");
        NAV_SUP_bearingToCompass(180.0, buf, sizeof(buf));
        TEST_STR("bearing 180", buf, "S");
        NAV_SUP_bearingToCompass(270.0, buf, sizeof(buf));
        TEST_STR("bearing 270", buf, "W");
        NAV_SUP_bearingToCompass(22.5, buf, sizeof(buf));
        TEST_STR("bearing 22.5", buf, "NNE");
    }

    printf("\n--- DMS / Decimal Degrees ---\n");
    TEST_NEAR("dmsToDecimal(53,30,0,N)", NAV_SUP_dmsToDecimal(53, 30, 0.0, 'N'), 53.5, 0.0001);
    int d, m;
    double s;
    NAV_SUP_decimalToDMS(53.5, &d, &m, &s);
    TEST_TRUE("decimalToDMS(53.5) degrees", d == 53);
    TEST_TRUE("decimalToDMS(53.5) minutes", m == 30);
    TEST_NEAR("decimalToDMS(53.5) seconds", s, 0.0, 0.001);
    TEST_NEAR("dmsToDecimal(43,45,30,S)", NAV_SUP_dmsToDecimal(43, 45, 30.0, 'S'), -43.75833, 0.001);

    printf("\n--- DMM / Decimal ---\n");
    TEST_NEAR("dmmToDecimal(53,30.0,N)", NAV_SUP_dmmToDecimal(53, 30.0, 'N'), 53.5, 0.01);
    {
        double mins;
        char dir;
        NAV_SUP_decimalToDMM(-43.75833, &mins, &dir);
        TEST_NEAR("decimalToDMM(-43.75833) minutes", mins, 45.5, 0.1);
    }

    printf("\n--- Wind Correction ---\n");
    {
        double heading, gs, wca;
        NAV_SUP_windCorrection(90.0, 100.0, 90.0, 20.0, &heading, &gs, &wca);
        TEST_NEAR("windCorr (headwind) heading", heading, 90.0, 1.0);
        TEST_NEAR("windCorr (headwind) GS", gs, 80.0, 1.0);
        TEST_NEAR("windCorr (headwind) WCA", wca, 0.0, 0.5);
    }
    {
        double heading, gs, wca;
        NAV_SUP_windCorrection(90.0, 100.0, 270.0, 20.0, &heading, &gs, &wca);
        TEST_NEAR("windCorr (tailwind) heading", heading, 90.0, 1.0);
        TEST_NEAR("windCorr (tailwind) GS", gs, 120.0, 1.0);
    }
    {
        double heading, gs, wca;
        NAV_SUP_windCorrection(0.0, 100.0, 270.0, 20.0, &heading, &gs, &wca);
        TEST_NEAR("windCorr (crosswind) heading", heading, 348.5, 5.0);
        TEST_NEAR("windCorr (crosswind) GS", gs, 97.98, 3.0);
    }

    printf("\n--- Wind Components ---\n");
    {
        double xw, hw;
        NAV_SUP_windComponents(270.0, 20.0, 270.0, &xw, &hw);
        TEST_NEAR("windComp headwind", hw, 20.0, 0.5);
        TEST_NEAR("windComp crosswind", xw, 0.0, 0.5);
    }
    {
        double xw, hw;
        NAV_SUP_windComponents(270.0, 20.0, 360.0, &xw, &hw);
        TEST_NEAR("windComp(270,20,360) headwind", hw, 0.0, 0.5);
        TEST_NEAR("windComp(270,20,360) crosswind right", xw, 20.0, 1.0);
    }

    printf("\n--- ISA Atmosphere ---\n");
    {
        double t, p, r;
        NAV_SUP_isaAtmosphere(0.0, &t, &p, &r);
        TEST_NEAR("ISA sea level T", t, 15.0, 0.5);
        TEST_NEAR("ISA sea level P", p, 1013.25, 1.0);
        TEST_NEAR("ISA sea level rho", r, 1.225, 0.01);
    }
    {
        double t, p, r;
        NAV_SUP_isaAtmosphere(10000.0, &t, &p, &r);
        TEST_NEAR("ISA 10000ft T", t, -4.8, 1.0);
        TEST_NEAR("ISA 10000ft P", p, 696.8, 5.0);
    }

    printf("\n--- Pressure & Density Altitude ---\n");
    TEST_NEAR("pressureAlt(1000,29.92)", NAV_SUP_pressureAltitude(1000.0, 29.92), 1000.0, 50.0);
    TEST_NEAR("densityAlt(1000,15,29.92)", NAV_SUP_densityAltitude(1000.0, 15.0, 29.92), 1240.0, 50.0);

    printf("\n--- GPS Speed ---\n");
    TEST_NEAR("gpsSpeed 60NM in 1hr", NAV_SUP_gpsSpeed(0.0, 0.0, 1.0, 0.0, 3600.0), 60.0, 1.0);

    printf("\n--- NMEA Checksum ---\n");
    TEST_TRUE("nmeaChecksum valid", NAV_SUP_nmeaChecksum("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47"));
    TEST_TRUE("nmeaChecksum invalid", !NAV_SUP_nmeaChecksum("$GPGGA,123519*00"));
    TEST_TRUE("nmeaChecksum bad prefix", !NAV_SUP_nmeaChecksum("XGPGGA*47"));

    printf("\n--- Coordinate Validation ---\n");
    TEST_TRUE("isValidLat(0)", NAV_SUP_isValidLatitude(0.0));
    TEST_TRUE("isValidLat(90)", NAV_SUP_isValidLatitude(90.0));
    TEST_TRUE("isValidLat(-90)", NAV_SUP_isValidLatitude(-90.0));
    TEST_TRUE("isValidLat(91) invalid", !NAV_SUP_isValidLatitude(91.0));
    TEST_TRUE("isValidLon(0)", NAV_SUP_isValidLongitude(0.0));
    TEST_TRUE("isValidLon(180)", NAV_SUP_isValidLongitude(180.0));
    TEST_TRUE("isValidLon(181) invalid", !NAV_SUP_isValidLongitude(181.0));

    printf("\n--- Temperature Conversions ---\n");
    TEST_NEAR("cToF(0)", NAV_CONV_cToF(0.0), 32.0, 0.1);
    TEST_NEAR("cToF(100)", NAV_CONV_cToF(100.0), 212.0, 0.1);
    TEST_NEAR("fToC(32)", NAV_CONV_fToC(32.0), 0.0, 0.1);
    TEST_NEAR("fToC(212)", NAV_CONV_fToC(212.0), 100.0, 0.1);

    printf("\n--- Pressure Conversions ---\n");
    TEST_NEAR("hPaToInHg(1013.25)", NAV_CONV_hPaToInHg(1013.25), 29.92, 0.01);
    TEST_NEAR("inHgTohPa(29.92)", NAV_CONV_inHgTohPa(29.92), 1013.25, 0.1);
    TEST_NEAR("hPaToPsi(1013.25)", NAV_CONV_hPaToPsi(1013.25), 14.6959, 0.01);

    printf("\n--- Flight Management ---\n");
    TEST_NEAR("timeToAltitude(10000,15000,500)", NAV_SUP_timeToAltitude(10000.0, 15000.0, 500.0), 600.0, 1.0);
    TEST_NEAR("timeToAltitude(30000,10000,-1000)", NAV_SUP_timeToAltitude(30000.0, 10000.0, -1000.0), 1200.0, 1.0);
    TEST_NEAR("requiredVS(10000,15000,100,200)", NAV_SUP_requiredVerticalSpeed(10000.0, 15000.0, 100.0, 200.0), 166.67, 1.0);
    TEST_NEAR("topOfDescent(30000,1000,500,120)", NAV_SUP_topOfDescent(30000.0, 1000.0, 500.0, 120.0), 116.0, 1.0);
    TEST_NEAR("timeToWaypoint(100,200)", NAV_SUP_timeToWaypoint(100.0, 200.0), 1800.0, 1.0);
    TEST_NEAR("fuelEndurance(50,10)", NAV_SUP_fuelEndurance(50.0, 10.0), 5.0, 0.1);
    TEST_NEAR("fuelRange(50,10,120)", NAV_SUP_fuelRange(50.0, 10.0, 120.0), 600.0, 1.0);

    printf("\n--- Control Systems ---\n");
    {
        NAV_Sup_LowPassFilter lpf;
        NAV_SUP_lpfInit(&lpf, 0.0);
        TEST_NEAR("LPF first update", NAV_SUP_lpfUpdate(&lpf, 10.0, 0.5), 5.0, 0.01);
        TEST_NEAR("LPF second update", NAV_SUP_lpfUpdate(&lpf, 10.0, 0.5), 7.5, 0.01);
    }
    {
        NAV_Sup_SlewLimiter s;
        NAV_SUP_slewInit(&s, 0.0);
        double v = NAV_SUP_slewUpdate(&s, 100.0, 10.0, 1.0);
        TEST_NEAR("Slew first step", v, 10.0, 0.01);
        v = NAV_SUP_slewUpdate(&s, 100.0, 10.0, 1.0);
        TEST_NEAR("Slew second step", v, 20.0, 0.01);
    }
    {
        double buf[3];
        NAV_Sup_MovingAverage ma;
        NAV_SUP_maInit(&ma, buf, 3);
        NAV_SUP_maUpdate(&ma, 1.0);
        NAV_SUP_maUpdate(&ma, 2.0);
        TEST_NEAR("MA after 2 values", NAV_SUP_maUpdate(&ma, 3.0), 2.0, 0.01);
        TEST_NEAR("MA overwrite", NAV_SUP_maUpdate(&ma, 10.0), 5.0, 0.01);
    }
    {
        NAV_Sup_PIDController pid;
        NAV_SUP_pidInit(&pid, 1.0, 0.0, 0.0, -100.0, 100.0);
        TEST_NEAR("PID P-only", NAV_SUP_pidUpdate(&pid, 10.0, 0.0, 0.1), 10.0, 0.01);
    }
    {
        NAV_Sup_PIDController pid;
        NAV_SUP_pidInit(&pid, 0.0, 1.0, 0.0, -100.0, 100.0);
        double out = NAV_SUP_pidUpdate(&pid, 10.0, 0.0, 1.0);
        TEST_NEAR("PID I build", out, 10.0, 0.01);
        out = NAV_SUP_pidUpdate(&pid, 10.0, 0.0, 1.0);
        TEST_NEAR("PID I accumulate", out, 20.0, 0.01);
    }
    {
        NAV_Sup_ComplementaryFilter cf;
        NAV_SUP_cfInit(&cf, 0.0);
        TEST_NEAR("CompFilter first", NAV_SUP_cfUpdate(&cf, 10.0, 0.0, 0.01, 0.5), 5.0, 0.01);
        TEST_NEAR("CompFilter second", NAV_SUP_cfUpdate(&cf, 10.0, 0.0, 0.01, 0.5), 7.5, 0.01);
    }

    printf("\n--- NMEA Parser ---\n");
    {
        NAV_Sup_NMEAData nd;
        int r = NAV_SUP_nmeaParse("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47", &nd);
        TEST_TRUE("nmeaParse GGA return", r);
        TEST_TRUE("nmeaParse GGA hasFix", nd.hasFix);
        TEST_NEAR("nmeaParse GGA lat", nd.latitude, 48.1173, 0.001);
        TEST_NEAR("nmeaParse GGA lon", nd.longitude, 11.5167, 0.001);
        TEST_NEAR("nmeaParse GGA alt", nd.altitudeM, 545.4, 0.1);
        TEST_NEAR("nmeaParse GGA geoid", nd.geoidSepM, 46.9, 0.1);
        TEST_TRUE("nmeaParse GGA fixQuality", nd.fixQuality == 1);
        TEST_TRUE("nmeaParse GGA numSats", nd.numSats == 8);
        TEST_NEAR("nmeaParse GGA hdop", nd.hdop, 0.9, 0.01);
    }
    {
        NAV_Sup_NMEAData nd;
        int r = NAV_SUP_nmeaParse("$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A", &nd);
        TEST_TRUE("nmeaParse RMC return", r);
        TEST_TRUE("nmeaParse RMC hasFix", nd.hasFix);
        TEST_NEAR("nmeaParse RMC lat", nd.latitude, 48.1173, 0.001);
        TEST_NEAR("nmeaParse RMC lon", nd.longitude, 11.5167, 0.001);
        TEST_NEAR("nmeaParse RMC speed", nd.speedKts, 22.4, 0.1);
        TEST_NEAR("nmeaParse RMC track", nd.trackDeg, 84.4, 0.1);
        TEST_STR("nmeaParse RMC date", nd.dateUTC, "230394");
        TEST_NEAR("nmeaParse RMC magVar", nd.magVarDeg, -3.1, 0.1);
    }
    {
        NAV_Sup_NMEAData nd;
        int r = NAV_SUP_nmeaParse("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*00", &nd);
        TEST_TRUE("nmeaParse bad checksum", !r);
    }
    {
        NAV_Sup_NMEAData nd;
        int r = NAV_SUP_nmeaParse(NULL, &nd);
        TEST_TRUE("nmeaParse NULL", !r);
    }
    {
        NAV_Sup_NMEAData nd;
        int r = NAV_SUP_nmeaParse("$GPGGA,123519,4807.038,N,01131.000,E,0,00,0.0,0.0,M,0.0,M,,*7C", &nd);
        TEST_TRUE("nmeaParse no fix return", !r);
        TEST_TRUE("nmeaParse no fix hasFix", !nd.hasFix);
        TEST_NEAR("nmeaParse no fix lat parsed", nd.latitude, 48.1173, 0.001);
    }
    {
        NAV_Sup_NMEAData nd;
        int r = NAV_SUP_nmeaParse("$GPGSA,,,,,,,,,,,,,*", &nd);
        TEST_TRUE("nmeaParse unsupported type", !r);
    }
    {
        NAV_Sup_NMEAData nd;
        int r = NAV_SUP_nmeaParse("$GNGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*59", &nd);
        TEST_TRUE("nmeaParse GNGGA (any talker)", r);
        TEST_NEAR("nmeaParse GNGGA lat", nd.latitude, 48.1173, 0.001);
    }

    printf("\n--- Route Module ---\n");
    {
        NAV_Sup_Waypoint wpBuf[8];
        NAV_Sup_Route r;
        NAV_SUP_routeInit(&r, wpBuf, 8);
        TEST_TRUE("rInit count 0", r.count == 0);
        TEST_TRUE("rInit index -1", r.currentIndex == -1);

        int ok = NAV_SUP_routeAdd(&r, 48.1173, 11.5167, 545.4, "MMT", "MOOSMOOS", 2);
        TEST_TRUE("rAdd return", ok);
        TEST_TRUE("rAdd count 1", r.count == 1);
        TEST_STR("rAdd ident", r.waypoints[0].ident, "MMT");
        TEST_NEAR("rAdd lat", r.waypoints[0].latitude, 48.1173, 0.0001);

        NAV_SUP_routeAdd(&r, 33.94, -118.41, 125.0, "LAX", "LOS ANGELES", 1);
        TEST_TRUE("rAdd count 2", r.count == 2);
    }
    {
        NAV_Sup_Waypoint wpBuf[8];
        NAV_Sup_Route r;
        NAV_SUP_routeInit(&r, wpBuf, 8);
        NAV_SUP_routeAdd(&r, 1, 2, 100, "A", "", 0);
        NAV_SUP_routeAdd(&r, 3, 4, 200, "C", "", 0);
        int ok = NAV_SUP_routeInsert(&r, 1, 2, 3, 150, "B", "", 0);
        TEST_TRUE("rIns return", ok);
        TEST_TRUE("rIns count 3", r.count == 3);
        TEST_STR("rIns [1]", r.waypoints[1].ident, "B");
        NAV_SUP_routeRemove(&r, 1);
        TEST_TRUE("rRem count 2", r.count == 2);
        TEST_STR("rRem [1]", r.waypoints[1].ident, "C");
    }
    {
        NAV_Sup_Waypoint wpBuf[8];
        NAV_Sup_Route r;
        NAV_SUP_routeInit(&r, wpBuf, 8);
        NAV_SUP_routeAdd(&r, 1, 2, 100, "A", "", 0);
        NAV_SUP_routeClear(&r);
        TEST_TRUE("rClear count 0", r.count == 0);
        TEST_TRUE("rClear index -1", r.currentIndex == -1);
    }
    {
        NAV_Sup_Waypoint wpBuf[8];
        NAV_Sup_Route r;
        NAV_SUP_routeInit(&r, wpBuf, 8);
        NAV_SUP_routeAdd(&r, 1, 2, 100, "A", "", 0);
        NAV_SUP_routeAdd(&r, 3, 4, 200, "B", "", 0);
        NAV_SUP_routeAdd(&r, 5, 6, 300, "C", "", 0);
        TEST_TRUE("rSeq idx0 after add", r.currentIndex == 0);
        TEST_TRUE("rSeq advance", NAV_SUP_routeSequence(&r));
        TEST_TRUE("rSeq idx1", r.currentIndex == 1);
        NAV_SUP_routeSequence(&r);
        TEST_TRUE("rSeq end", !NAV_SUP_routeSequence(&r));
        TEST_TRUE("rDT invalid", !NAV_SUP_routeDirectTo(&r, 99));
        TEST_TRUE("rDT valid", NAV_SUP_routeDirectTo(&r, 0));
        TEST_TRUE("rDT idx0", r.currentIndex == 0);
    }
    {
        NAV_Sup_Waypoint wpBuf[8];
        NAV_Sup_Route r;
        double brg, dist;
        NAV_SUP_routeInit(&r, wpBuf, 8);
        NAV_SUP_routeAdd(&r, 48.0, 11.0, 100, "A", "", 0);
        NAV_SUP_routeAdd(&r, 49.0, 12.0, 200, "B", "", 0);
        int ok = NAV_SUP_routeLegInfo(&r, 1, &brg, &dist);
        TEST_TRUE("rLeg return", ok);
        TEST_NEAR("rLeg dist", dist, 72, 5);
        TEST_NEAR("rLeg brg", brg, 34, 3);
    }
    {
        NAV_Sup_Waypoint wpBuf[8];
        NAV_Sup_Route r;
        double lat, lon;
        NAV_SUP_routeInit(&r, wpBuf, 8);
        NAV_SUP_routeAdd(&r, 48.0, 11.0, 100, "A", "", 0);
        NAV_SUP_routeAdd(&r, 49.5, 11.0, 200, "B", "", 0);
        NAV_SUP_routeAdd(&r, 49.5, 12.5, 300, "C", "", 0);
        int ok = NAV_SUP_routeTurnAnticipation(&r, 300, 3, &lat, &lon);
        TEST_TRUE("rTurn return", ok);
        TEST_TRUE("rTurn lat between A and B", lat > 48.0 && lat < 49.5);
        TEST_NEAR("rTurn lon ~11", lon, 11.0, 0.3);
    }
    {
        NAV_Sup_Waypoint wpBuf[8], lbuf[8];
        NAV_Sup_Route r, lr;
        NAV_SUP_routeInit(&r, wpBuf, 8);
        NAV_SUP_routeAdd(&r, 48.1173, 11.5167, 545.4, "MMT", "MOOSMOOS", 2);
        NAV_SUP_routeAdd(&r, 33.9425, -118.4081, 125.0, "LAX", "LOS ANGELES", 1);
        int ok = NAV_SUP_routeSaveCSV("_troute.csv", &r);
        TEST_TRUE("rSave return", ok);
        ok = NAV_SUP_routeLoadCSV("_troute.csv", lbuf, 8, &lr);
        TEST_TRUE("rLoad return", ok);
        TEST_TRUE("rLoad count 2", lr.count == 2);
        TEST_STR("rLoad ident0", lr.waypoints[0].ident, "MMT");
        TEST_STR("rLoad name1", lr.waypoints[1].name, "LOS ANGELES");
        remove("_troute.csv");
    }

    printf("\n--- X-Plane Nav Module ---\n");
    {
        NAV_Sup_NavDatabase db;
        int ok = NAV_SUP_xpNavLoad("_noexist.dat", &db);
        TEST_TRUE("xpLoad missing", !ok);
    }
    {
        FILE *f = fopen("_tnav.dat", "w");
        fprintf(f, "# test\n");
        fprintf(f, "2   48.1173   11.516667      545.4  113.300 MMT MOOSMOOS\n");
        fprintf(f, "2   33.9425  -118.4081       125.0  115.700 LAX  LOS ANGELES INTL\n");
        fprintf(f, "3   51.5     -0.1            50.0   350.000 LON  LONDON NDB\n");
        fclose(f);
        NAV_Sup_NavDatabase db;
        int ok = NAV_SUP_xpNavLoad("_tnav.dat", &db);
        TEST_TRUE("xpLoad ok", ok);
        TEST_TRUE("xpLoad cnt 3", db.count == 3);
        TEST_STR("xpLoad id0", db.records[0].ident, "MMT");
        TEST_NEAR("xpLoad freq1", db.records[1].frequency, 115.7, 0.01);
        NAV_SUP_xpNavFree(&db);
        remove("_tnav.dat");
    }
    {
        FILE *f = fopen("_tfix.dat", "w");
        fprintf(f, "# test fixes\n");
        fprintf(f, "KAILA  -118.4081  33.9425\n");
        fprintf(f, "SABAT  -119.5     34.0\n");
        fclose(f);
        NAV_Sup_NavDatabase db;
        int ok = NAV_SUP_xpFixLoad("_tfix.dat", &db);
        TEST_TRUE("xpFix ok", ok);
        TEST_TRUE("xpFix cnt 2", db.count == 2);
        TEST_STR("xpFix id0", db.records[0].ident, "KAILA");
        TEST_NEAR("xpFix lat0", db.records[0].latitude, 33.9425, 0.001);
        NAV_SUP_xpNavFree(&db);
        remove("_tfix.dat");
    }
    {
        FILE *f = fopen("_tnav2.dat", "w");
        fprintf(f, "2   48.1173   11.516667  545.4  113.300 MMT MOOSMOOS\n");
        fprintf(f, "2   33.9425  -118.4081   125.0  115.700 LAX  LOS ANGELES INTL\n");
        fclose(f);
        NAV_Sup_NavDatabase db;
        NAV_SUP_xpNavLoad("_tnav2.dat", &db);
        NAV_Sup_NavRecord rec;
        int idx = NAV_SUP_xpNavFindByID(&db, "LAX", 0, &rec);
        TEST_TRUE("xpFindByID found", idx != 0);
        TEST_NEAR("xpFindByID lat", rec.latitude, 33.9425, 0.001);
        NAV_Sup_NavRecord nearRec;
        int ok = NAV_SUP_xpNavFindNearest(&db, 48.0, 11.5, -1, &nearRec);
        TEST_TRUE("xpFindNearest ok", ok);
        TEST_STR("xpFindNearest id", nearRec.ident, "MMT");
        ok = NAV_SUP_xpNavFindNearest(&db, 48.0, 11.5, 3, &nearRec);
        TEST_TRUE("xpFindNearest type filter no match", !ok);
        NAV_SUP_xpNavFree(&db);
        remove("_tnav2.dat");
    }
    {
        const char *fpl = "N0477F350 DCT SMO J501 RZS DCT BTY";
        NAV_Sup_Waypoint wps[16];
        int n = NAV_SUP_fplParseRoute(fpl, NULL, NULL, wps, 16);
        TEST_TRUE("fplParse fallback", n > 0);
        TEST_TRUE("fplParse count", n == 3);
        TEST_STR("fplParse wp0", wps[0].ident, "SMO");
    }
    {
        const char *fpl = "4530N01330E DCT KAILA";
        NAV_Sup_Waypoint wps[8];
        int n = NAV_SUP_fplParseRoute(fpl, NULL, NULL, wps, 8);
        TEST_TRUE("fplParse coord", n == 2);
        TEST_NEAR("fplParse lat 45.5", wps[0].latitude, 45.5, 0.1);
        TEST_NEAR("fplParse lon 13.5", wps[0].longitude, 13.5, 0.1);
    }

    printf("--- METAR Module ---\n");
    {
        /* KSEA basic METAR */
        NAV_Sup_MetarData m;
        int ok = NAV_SUP_metarParse(
            "KSEA 221653Z 21012G20KT 180V250 10SM -RA BKN025 OVC050 12/09 A2993", &m);
        TEST_TRUE("metar parse ok", ok);
        TEST_STR("metar icao", m.icao, "KSEA");
        TEST_TRUE("metar day", m.day == 22);
        TEST_TRUE("metar hour", m.hour == 16);
        TEST_TRUE("metar minute", m.minute == 53);
        TEST_TRUE("metar windDir", m.windDir == 210);
        TEST_TRUE("metar windSpeed", m.windSpeedKt == 12);
        TEST_TRUE("metar windGust", m.windGustKt == 20);
        TEST_TRUE("metar windVarLow", m.windVarLow == 180);
        TEST_TRUE("metar windVarHigh", m.windVarHigh == 250);
        TEST_TRUE("metar vis M", m.visibilityM == 16093);
        TEST_TRUE("metar vis SM_num", m.visibilitySM_num == 10);
        TEST_TRUE("metar vis SM_den", m.visibilitySM_den == 1);
        TEST_TRUE("metar skyCount", m.skyCount == 1);
        TEST_TRUE("metar cloudCount", m.cloudCount == 2);
        TEST_STR("metar weather phen", m.sky[0].phenomena, "RA");
        TEST_TRUE("metar weather int", m.sky[0].intensity == '-');
        TEST_TRUE("metar cloud0 code BKN", m.clouds[0].code == 3);
        TEST_TRUE("metar cloud0 alt", m.clouds[0].altitudeFt == 2500);
        TEST_TRUE("metar cloud1 code OVC", m.clouds[1].code == 4);
        TEST_TRUE("metar cloud1 alt", m.clouds[1].altitudeFt == 5000);
        TEST_NEAR("metar temp", m.tempC, 12.0, 0.01);
        TEST_NEAR("metar dewpoint", m.dewpointC, 9.0, 0.01);
        TEST_TRUE("metar QNH hPa", m.qnhHPa == 1014);
    }
    {
        /* VRB wind + metric visibility */
        NAV_Sup_MetarData m;
        NAV_SUP_metarParse("KJFK 221653Z VRB05KT 9999 FEW030 18/12 Q1015", &m);
        TEST_TRUE("metar VRB windDir", m.windDir == -1);
        TEST_TRUE("metar VRB windSpeed", m.windSpeedKt == 5);
        TEST_TRUE("metar VRB vis M", m.visibilityM == 9999);
        TEST_TRUE("metar VRB qnh", m.qnhHPa == 1015);
    }
    {
        /* Calm wind + negative temp + SM fraction */
        NAV_Sup_MetarData m;
        NAV_SUP_metarParse("PAFA 221653Z 00000KT 1/4SM FZFG BKN001 M25/M29 A3002", &m);
        TEST_TRUE("metar calm windDir", m.windDir == 0);
        TEST_TRUE("metar calm windSpeed", m.windSpeedKt == 0);
        TEST_TRUE("metar calm vis M", m.visibilityM == 402);
        TEST_TRUE("metar calm vis SM_num", m.visibilitySM_num == 1);
        TEST_TRUE("metar calm vis SM_den", m.visibilitySM_den == 4);
        TEST_STR("metar calm weather descr", m.sky[0].descriptor, "FZ");
        TEST_STR("metar calm weather phen", m.sky[0].phenomena, "FG");
        TEST_NEAR("metar neg temp", m.tempC, -25.0, 0.01);
        TEST_NEAR("metar neg dew", m.dewpointC, -29.0, 0.01);
        TEST_TRUE("metar calm qnh", m.qnhHPa == 1017);
    }
    {
        /* CAVOK + CB + NOSIG */
        NAV_Sup_MetarData m;
        NAV_SUP_metarParse("KLAX 221653Z 25008KT CAVOK 22/16 Q1013 NOSIG", &m);
        TEST_TRUE("metar CAVOK vis", m.visibilityM == -1);
        TEST_TRUE("metar CAVOK qnh", m.qnhHPa == 1013);
        TEST_STR("metar CAVOK trend", m.trend, "NOSIG");
    }
    {
        /* CB cloud type */
        NAV_Sup_MetarData m;
        NAV_SUP_metarParse("KOKC 221653Z 36015G25KT 10SM TSRA BKN050CB 28/20 Q1014", &m);
        TEST_TRUE("metar CB cloudCount", m.cloudCount >= 1);
        TEST_TRUE("metar CB code", m.clouds[0].code == 3);
        TEST_TRUE("metar CB alt", m.clouds[0].altitudeFt == 5000);
        TEST_STR("metar CB type", m.clouds[0].type, "CB");
        TEST_STR("metar CB descr", m.sky[0].descriptor, "TS");
        TEST_STR("metar CB phen", m.sky[0].phenomena, "RA");
    }
    {
        /* Mixed SM visibility: 1 1/2SM */
        NAV_Sup_MetarData m;
        NAV_SUP_metarParse("KXYZ 221653Z 00000KT 1 1/2SM BR OVC010 10/09 Q1019", &m);
        TEST_TRUE("metar mixed vis M", m.visibilityM == 2414);
        TEST_TRUE("metar mixed SM_num", m.visibilitySM_num == 3);
        TEST_TRUE("metar mixed SM_den", m.visibilitySM_den == 2);
        TEST_STR("metar mixed phen", m.sky[0].phenomena, "BR");
    }
    {
        /* Crosswind / headwind */
        NAV_Sup_MetarData m;
        NAV_SUP_metarParse("KSEA 221653Z 27015KT 10SM SCT010 12/09 Q1019", &m);
        double xw = NAV_SUP_metarCrosswind(&m, 180.0);
        double hw = NAV_SUP_metarHeadwind(&m, 180.0);
        TEST_NEAR("metar xwind", xw, -15.0, 0.01);
        TEST_NEAR("metar hwind", hw, 0.0, 0.01);
        double avg = NAV_SUP_metarWindAvg(&m);
        TEST_NEAR("metar windAvg", avg, 15.0, 0.01);
    }
    {
        /* null/empty */
        int ok = NAV_SUP_metarParse((const char *)0, (NAV_Sup_MetarData *)0);
        TEST_TRUE("metar null parse", !ok);
    }

    printf("\n--- ARINC 429 Module ---\n");
    {
        unsigned int w = (8) | (2 << 8) | (0x88B8 << 10) | (3 << 29);
        TEST_TRUE("a429 label", NAV_SUP_a429Label(w) == 8);
        TEST_TRUE("a429 sdi", NAV_SUP_a429SDI(w) == 2);
        TEST_TRUE("a429 data", NAV_SUP_a429Data(w) == 0x88B8);
        TEST_TRUE("a429 ssm", NAV_SUP_a429SSM(w) == 3);
    }
    {
        unsigned int w = 0;
        TEST_TRUE("a429 zero label", NAV_SUP_a429Label(w) == 0);
        TEST_TRUE("a429 zero sdi", NAV_SUP_a429SDI(w) == 0);
        TEST_TRUE("a429 zero data", NAV_SUP_a429Data(w) == 0);
        TEST_TRUE("a429 zero ssm", NAV_SUP_a429SSM(w) == 0);
    }
    {
        unsigned int w = 0xFFFFFFFF;
        TEST_TRUE("a429 allones label", NAV_SUP_a429Label(w) == 0xFF);
        TEST_TRUE("a429 allones sdi", NAV_SUP_a429SDI(w) == 3);
        TEST_TRUE("a429 allones data", NAV_SUP_a429Data(w) == 0x7FFFF);
        TEST_TRUE("a429 allones ssm", NAV_SUP_a429SSM(w) == 3);
    }
    {
        unsigned int w = 0x00000001;
        TEST_TRUE("a429 parity odd", NAV_SUP_a429ParityCheck(w));
    }
    {
        unsigned int w = 0x00000003;
        TEST_TRUE("a429 parity even", !NAV_SUP_a429ParityCheck(w));
    }
    {
        unsigned int w = 0x00000000;
        TEST_TRUE("a429 parity zero", !NAV_SUP_a429ParityCheck(w));
    }
    {
        NAV_Sup_ARINC429Word out;
        unsigned int w = (8) | (0 << 8) | (35000 << 10) | (3 << 29);
        NAV_SUP_a429Decode(w, &out);
        TEST_TRUE("a429 dec label", out.label == 8);
        TEST_NEAR("a429 dec alt", out.value, 35000.0, 0.5);
        TEST_STR("a429 dec name", out.name, "Baro Altitude");
        TEST_STR("a429 dec unit", out.unit, "ft");
        TEST_TRUE("a429 dec ssm 3", out.ssm == 3);
    }
    {
        double v = NAV_SUP_a429DataBNR(960, 0.125);
        TEST_NEAR("a429 BNR kt", v, 120.0, 0.5);
    }
    {
        double v = NAV_SUP_a429DataBCD(0x12345, 5);
        TEST_NEAR("a429 BCD time", v, 12345.0, 0.5);
    }
    {
        char name[40], unit[12];
        int enc;
        double lsb;
        int sig;
        int ok = NAV_SUP_a429LabelInfo(0010, name, unit, &enc, &lsb, &sig);
        TEST_TRUE("a429 labelInfo found", ok);
        TEST_STR("a429 labelInfo name", name, "Baro Altitude");
        TEST_STR("a429 labelInfo unit", unit, "ft");
        TEST_TRUE("a429 labelInfo enc BNR", enc == NAV_A429_ENCODING_BNR);
        TEST_TRUE("a429 labelInfo signed", sig);
    }
    {
        char name[40], unit[12];
        int enc;
        double lsb;
        int sig;
        int ok = NAV_SUP_a429LabelInfo(0200, name, unit, &enc, &lsb, &sig);
        TEST_TRUE("a429 labelInfo FMS ID", ok);
        TEST_STR("a429 labelInfo FMS name", name, "FMS ID");
    }
    {
        char name[40], unit[12];
        int enc;
        double lsb;
        int sig;
        int ok = NAV_SUP_a429LabelInfo(0777, name, unit, &enc, &lsb, &sig);
        TEST_TRUE("a429 labelInfo unknown", !ok);
    }
    {
        NAV_Sup_ARINC429Word out;
        unsigned int w = (34) | (0 << 8) | (0x12345 << 10) | (3 << 29);
        NAV_SUP_a429Decode(w, &out);
        TEST_NEAR("a429 dec UTC", out.value, 12345.0, 0.5);
        TEST_STR("a429 dec UTC name", out.name, "UTC Time");
    }

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
