#include "dll.h"
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

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
