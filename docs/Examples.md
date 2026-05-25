# NAV1 API Examples

All snippets follow C89 discipline. Each shows the function signature and
a typical call with `printf` output. Snippets are standalone reference --
not intended to be compiled together.

---

## GEO -- Geodesy (14 functions)

```c
/* distanceToTarget / headingToTarget / finalBearing */
{
    double d  = NAV1_GEO_distanceToTarget(53.0, 10.0, 43.0, 27.0);
    double b  = NAV1_GEO_headingToTarget(53.0, 10.0, 43.0, 27.0);
    double fb = NAV1_GEO_finalBearing(53.0, 10.0, 43.0, 27.0);
    printf("EGLL-LIRF: %.1f NM, brg %.1f, fin %.1f\n", d, b, fb);
}
```

```c
/* destinationPoint -- position from lat/lon + bearing + distance */
{
    double lat, lon;
    NAV1_GEO_destinationPoint(0.0, 0.0, 0.0, 60.0, &lat, &lon);
    printf("dest N 60 NM from 0,0: lat=%.6f lon=%.6f\n", lat, lon);
}
```

```c
/* midpoint */
{
    double lat, lon;
    NAV1_GEO_midpoint(52.0, 0.0, 53.0, 1.0, &lat, &lon);
    printf("mid: lat=%.6f lon=%.6f\n", lat, lon);
}
```

```c
/* crossTrackDistance */
{
    double d = NAV1_GEO_crossTrackDistance(0.0, 0.0, 10.0, 0.0,
                                           5.0, 0.5);
    printf("x-track=%.4f NM\n", d);
}
```

```c
/* dmsToDecimal / decimalToDMS */
{
    double dec = NAV1_GEO_dmsToDecimal(53, 30, 0.0, 'N');
    int d, m;
    double s;
    NAV1_GEO_decimalToDMS(dec, &d, &m, &s);
    printf("53.5 deg = %d deg %d min %.0f sec\n", d, m, s);
}
```

```c
/* geodeticToEcef / ecefToGeodetic */
{
    double x, y, z;
    NAV1_GEO_geodeticToEcef(52.0, 5.0, 100.0, &x, &y, &z);
    printf("ECEF: x=%.0f y=%.0f z=%.0f\n", x, y, z);
    {
        double lat, lon, alt;
        if (NAV1_GEO_ecefToGeodetic(x, y, z, &lat, &lon, &alt))
            printf("geo: lat=%.6f lon=%.6f alt=%.1f\n", lat, lon, alt);
    }
}
```

```c
/* normalizeAngle / shortestAngularDistance */
{
    double n = NAV1_GEO_normalizeAngle(370.0);
    double d = NAV1_GEO_shortestAngularDistance(10.0, 350.0);
    printf("norm=%.1f shortDist=%.1f\n", n, d);
}
```

```c
/* intersectingRadials */
{
    double lat, lon;
    if (NAV1_GEO_intersectingRadials(48.0, 10.0, 45.0,
                                     46.0, 12.0, 90.0, &lat, &lon))
        printf("int radial: lat=%.6f lon=%.6f\n", lat, lon);
}
```

```c
/* intermediatePoint */
{
    double lat, lon;
    NAV1_GEO_intermediatePoint(0.0, 0.0, 10.0, 0.0, 0.5,
                                &lat, &lon);
    printf("halfway: lat=%.6f lon=%.6f\n", lat, lon);
}
```

---

## CONV -- Unit Conversions (36 functions, 18 pairs)

```c
/* Length: mToFt / ftToM */
{
    double ft = NAV1_CONV_mToFt(100.0);
    double m  = NAV1_CONV_ftToM(328.084);
    printf("100 m = %.2f ft, 328 ft = %.2f m\n", ft, m);
}
```

```c
/* NM: nmToM / mToNm */
{
    double m  = NAV1_CONV_nmToM(1.0);
    double nm = NAV1_CONV_mToNm(1852.0);
    printf("1 NM = %.0f m, 1852 m = %.4f NM\n", m, nm);
}
```

```c
/* NM: nmToKm / kmToNm */
{
    double km = NAV1_CONV_nmToKm(1.0);
    double nm = NAV1_CONV_kmToNm(1.852);
    printf("1 NM = %.3f km, 1.852 km = %.4f NM\n", km, nm);
}
```

```c
/* NM / Mile: nmToMi / miToNm */
{
    double mi = NAV1_CONV_nmToMi(1.0);
    double nm = NAV1_CONV_miToNm(1.151);
    printf("1 NM = %.3f mi, 1.151 mi = %.3f NM\n", mi, nm);
}
```

```c
/* km / Mile: kmToMi / miToKm */
{
    double mi = NAV1_CONV_kmToMi(1.0);
    double km = NAV1_CONV_miToKm(0.6214);
    printf("1 km = %.4f mi, 0.6214 mi = %.4f km\n", mi, km);
}
```

```c
/* Speed: knotsToMs / msToKnots */
{
    double ms = NAV1_CONV_knotsToMs(100.0);
    double kt = NAV1_CONV_msToKnots(51.444);
    printf("100 kt = %.3f m/s, 51.44 m/s = %.1f kt\n", ms, kt);
}
```

```c
/* Speed: knotsToKmh / kmhToKnots */
{
    double kmh = NAV1_CONV_knotsToKmh(100.0);
    double kt  = NAV1_CONV_kmhToKnots(185.2);
    printf("100 kt = %.1f km/h, 185.2 km/h = %.1f kt\n", kmh, kt);
}
```

```c
/* Speed: knotsToMph / mphToKnots */
{
    double mph = NAV1_CONV_knotsToMph(100.0);
    double kt  = NAV1_CONV_mphToKnots(115.078);
    printf("100 kt = %.3f mph, 115 mph = %.1f kt\n", mph, kt);
}
```

```c
/* Temperature: cToF / fToC */
{
    double f = NAV1_CONV_cToF(25.0);
    double c = NAV1_CONV_fToC(77.0);
    printf("25 C = %.1f F, 77 F = %.1f C\n", f, c);
}
```

```c
/* Pressure: hPaToInHg / inHgTohPa */
{
    double inhg = NAV1_CONV_hPaToInHg(1013.25);
    double hpa  = NAV1_CONV_inHgTohPa(29.92);
    printf("1013.25 hPa = %.2f inHg, 29.92 inHg = %.2f hPa\n", inhg, hpa);
}
```

```c
/* Pressure: hPaToPsi / psiTohPa */
{
    double psi = NAV1_CONV_hPaToPsi(1013.25);
    double hpa = NAV1_CONV_psiTohPa(14.7);
    printf("1013.25 hPa = %.4f psi, 14.7 psi = %.2f hPa\n", psi, hpa);
}
```

```c
/* Volume: galToL / lToGal */
{
    double l = NAV1_CONV_galToL(1.0);
    double g = NAV1_CONV_lToGal(3.785);
    printf("1 gal = %.4f L, 3.785 L = %.4f gal\n", l, g);
}
```

```c
/* Weight: lbToKg / kgToLb */
{
    double kg = NAV1_CONV_lbToKg(1.0);
    double lb = NAV1_CONV_kgToLb(0.454);
    printf("1 lb = %.4f kg, 0.454 kg = %.3f lb\n", kg, lb);
}
```

```c
/* NM / Ft: nmToFt / ftToNm */
{
    double ft = NAV1_CONV_nmToFt(1.0);
    double nm = NAV1_CONV_ftToNm(6076.12);
    printf("1 NM = %.2f ft, 6076 ft = %.4f NM\n", ft, nm);
}
```

```c
/* Angle: degToRad / radToDeg */
{
    double rad = NAV1_CONV_degToRad(180.0);
    double deg = NAV1_CONV_radToDeg(3.141593);
    printf("180 deg = %.6f rad, pi rad = %.4f deg\n", rad, deg);
}
```

```c
/* Metric: cmToInch / inchToCm */
{
    double in = NAV1_CONV_cmToInch(2.54);
    double cm = NAV1_CONV_inchToCm(1.0);
    printf("2.54 cm = %.2f in, 1 in = %.2f cm\n", in, cm);
}
```

```c
/* Metric: kgToTonne / tonneToKg */
{
    double t = NAV1_CONV_kgToTonne(1000.0);
    double k = NAV1_CONV_tonneToKg(1.0);
    printf("1000 kg = %.0f t, 1 t = %.0f kg\n", t, k);
}
```

```c
/* Metric: mToKm / kmToM */
{
    double km = NAV1_CONV_mToKm(1000.0);
    double m  = NAV1_CONV_kmToM(1.0);
    printf("1000 m = %.0f km, 1 km = %.0f m\n", km, m);
}
```

---

## NAV -- Advanced Navigation (11 functions, 9 snippets)

```c
/* alongTrackDistance */
{
    double d = NAV1_NAV_alongTrackDistance(0.0, 0.0, 10.0, 0.0, 5.0, 0.5);
    printf("along-track=%.4f NM\n", d);
}
```

```c
/* greatCircleWaypoints */
{
    double lats[5], lons[5];
    int i;
    NAV1_NAV_greatCircleWaypoints(52.0, 0.0, 53.0, 1.0, 5,
                                   lats, lons);
    for (i = 0; i < 5; ++i)
        printf("wp[%d]: lat=%.6f lon=%.6f\n", i, lats[i], lons[i]);
}
```

```c
/* rhumbDistance / rhumbBearing */
{
    double d = NAV1_NAV_rhumbDistance(0.0, 0.0, 10.0, 0.0);
    double b = NAV1_NAV_rhumbBearing(0.0, 0.0, 0.0, 10.0);
    printf("rhumb: dist=%.4f NM, brg=%.2f deg\n", d, b);
}
```

```c
/* antipodalPoint */
{
    double lat, lon;
    NAV1_NAV_antipodalPoint(50.0, 30.0, &lat, &lon);
    printf("antipode: lat=%.2f lon=%.2f\n", lat, lon);
}
```

```c
/* bearingToCompass */
{
    char buf[8];
    NAV1_NAV_bearingToCompass(22.5, buf, 8);
    printf("22.5 deg = %s\n", buf);
}
```

```c
/* closestPointOnCourse */
{
    double lat, lon;
    if (NAV1_NAV_closestPointOnCourse(0.0, 0.0, 10.0, 0.0, 5.0, 0.0,
                                       &lat, &lon))
        printf("closest: lat=%.6f lon=%.6f\n", lat, lon);
}
```

```c
/* leadPoint / fixRadialDistance */
{
    double lat, lon;
    NAV1_NAV_leadPoint(0.0, 0.0, 90.0, 0.5, 0.0, &lat, &lon);
    printf("lead pt: lat=%.6f lon=%.6f\n", lat, lon);
    {
        double lat2, lon2;
        NAV1_NAV_fixRadialDistance(48.0, 11.0, 90.0, 30.0,
                                    &lat2, &lon2);
        printf("fix radial: lat=%.6f lon=%.6f\n", lat2, lon2);
    }
}
```

```c
/* courseIntercept */
{
    double lat, lon;
    if (NAV1_NAV_courseIntercept(48.0, 10.0, 45.0, 46.0, 12.0,
                                  90.0, 45.0, &lat, &lon))
        printf("intercept: lat=%.6f lon=%.6f\n", lat, lon);
}
```

```c
/* holdEntryMode */
{
    int mode = NAV1_NAV_holdEntryMode(90.0, 180.0, 1);
    printf("hold entry: %s\n", mode == 0 ? "direct" :
           mode == 1 ? "parallel" : "teardrop");
}
```

---

## AERO -- Aviation Math (19 functions, 14 snippets)

```c
/* windCorrection */
{
    double hdg, gs, wca;
    NAV1_AERO_windCorrection(90.0, 100.0, 180.0, 20.0,
                              &hdg, &gs, &wca);
    printf("hdg=%.2f GS=%.2f WCA=%.2f\n", hdg, gs, wca);
}
```

```c
/* windComponents */
{
    double xw, hw;
    NAV1_AERO_windComponents(270.0, 20.0, 360.0, &xw, &hw);
    printf("cross=%.1f head=%.1f\n", xw, hw);
}
```

```c
/* windTriangle */
{
    double trk, gs;
    NAV1_AERO_windTriangle(90.0, 100.0, 180.0, 20.0, &trk, &gs);
    printf("trk=%.2f GS=%.2f\n", trk, gs);
}
```

```c
/* isaAtmosphere */
{
    double tc, p, rho;
    NAV1_AERO_isaAtmosphere(10000.0, &tc, &p, &rho);
    printf("ISA 10k ft: T=%.2f C  P=%.2f hPa  rho=%.6f\n", tc, p, rho);
}
```

```c
/* pressureAltitude / densityAltitude */
{
    double pa = NAV1_AERO_pressureAltitude(1000.0, 29.92);
    double da = NAV1_AERO_densityAltitude(1000.0, 15.0, 29.92);
    printf("PA=%.2f ft  DA=%.2f ft\n", pa, da);
}
```

```c
/* speedOfSound / machNumber */
{
    double sos = NAV1_AERO_speedOfSound(15.0);
    double m   = NAV1_AERO_machNumber(440.0, 15.0);
    printf("SOS=%.4f kt  Mach=%.4f\n", sos, m);
}
```

```c
/* casToTas / tasToCas */
{
    double tas = NAV1_AERO_casToTas(100.0, 0.0, 15.0);
    double cas = NAV1_AERO_tasToCas(100.0, 0.0, 15.0);
    printf("CAS->TAS=%.4f  TAS->CAS=%.4f\n", tas, cas);
}
```

```c
/* turnRadius / turnRate / bankForRate */
{
    double r  = NAV1_AERO_turnRadius(120.0, 25.0);
    double rt = NAV1_AERO_turnRate(120.0, 25.0);
    double b  = NAV1_AERO_bankForRate(120.0, 3.0);
    printf("R=%.4f NM  rate=%.4f deg/s  bank=%.4f deg\n", r, rt, b);
}
```

```c
/* flightPathAngle / vsFromFlightPathAngle */
{
    double fpa = NAV1_AERO_flightPathAngle(500.0, 100.0);
    double vs  = NAV1_AERO_vsFromFlightPathAngle(3.0, 100.0);
    printf("FPA=%.4f deg  VS=%.2f fpm\n", fpa, vs);
}
```

```c
/* reciprocalHeading */
{
    double r = NAV1_AERO_reciprocalHeading(90.0);
    printf("recip(90)=%.0f\n", r);
}
```

```c
/* standardRateTurnBank */
{
    double b = NAV1_AERO_standardRateTurnBank(120.0);
    printf("std rate bank=%.4f deg\n", b);
}
```

```c
/* trueAltitude */
{
    double ta = NAV1_AERO_trueAltitude(10000.0, 10.0);
    printf("true alt=%.2f ft\n", ta);
}
```

```c
/* coldTemperatureAltCorrection */
{
    double corr = NAV1_AERO_coldTemperatureAltCorrection(5000.0, -20.0);
    printf("cold corr=%.2f ft\n", corr);
}
```

---

## GPS -- GPS Helpers (6 functions, 4 snippets)

```c
/* dmmToDecimal / decimalToDMM */
{
    double dec = NAV1_GPS_dmmToDecimal(48, 7.038, 'N');
    double min;
    char dir;
    NAV1_GPS_decimalToDMM(dec, &min, &dir);
    printf("dec=%.6f  dmm=%d %.4f %c\n", dec, 48, min, dir);
}
```

```c
/* gpsSpeed */
{
    double s = NAV1_GPS_gpsSpeed(0.0, 0.0, 1.0, 0.0, 3600.0);
    printf("GPS speed=%.4f kt\n", s);
}
```

```c
/* nmeaChecksum */
{
    int ok = NAV1_GPS_nmeaChecksum("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");
    printf("checksum: %s\n", ok ? "valid" : "invalid");
}
```

```c
/* isValidLatitude / isValidLongitude */
{
    printf("lat 0=%d  lat 91=%d  lon 180=%d  lon 181=%d\n",
           NAV1_GPS_isValidLatitude(0.0),
           NAV1_GPS_isValidLatitude(91.0),
           NAV1_GPS_isValidLongitude(180.0),
           NAV1_GPS_isValidLongitude(181.0));
}
```

---

## FLT -- Flight Management (10 functions, 7 snippets)

```c
/* timeToAltitude */
{
    double t = NAV1_FLT_timeToAltitude(10000.0, 15000.0, 500.0);
    printf("time to alt=%.2f sec\n", t);
}
```

```c
/* requiredVerticalSpeed / topOfDescent */
{
    double vs  = NAV1_FLT_requiredVerticalSpeed(30000.0, 1000.0,
                                                 100.0, 120.0);
    double tod = NAV1_FLT_topOfDescent(30000.0, 1000.0, 500.0, 120.0);
    printf("req VS=%.2f fpm  TOD=%.2f NM\n", vs, tod);
}
```

```c
/* timeToWaypoint */
{
    double t = NAV1_FLT_timeToWaypoint(100.0, 120.0);
    printf("time to wp=%.0f sec\n", t);
}
```

```c
/* fuelEndurance / fuelRange */
{
    double e = NAV1_FLT_fuelEndurance(50.0, 10.0);
    double r = NAV1_FLT_fuelRange(50.0, 10.0, 120.0);
    printf("endurance=%.1f hr  range=%.0f NM\n", e, r);
}
```

```c
/* fuelRequired / specificRange */
{
    double fr = NAV1_FLT_fuelRequired(300.0, 120.0, 10.0);
    double sr = NAV1_FLT_specificRange(120.0, 10.0);
    printf("fuel req=%.1f gal  spec range=%.1f NM/gal\n", fr, sr);
}
```

```c
/* alternateFuelRequired */
{
    double af = NAV1_FLT_alternateFuelRequired(100.0, 120.0, 10.0, 45.0);
    printf("alternate fuel=%.2f gal\n", af);
}
```

```c
/* criticalFuel */
{
    double cf = NAV1_FLT_criticalFuel(100.0, 30.0);
    printf("critical fuel=%.0f gal\n", cf);
}
```

---

## CTL -- Signal Processing (13 functions, 9 snippets)

```c
/* Low-pass filter */
{
    NAV1_LowPassFilter f;
    double out;
    NAV1_CTL_lpfInit(&f, 0.0);
    out = NAV1_CTL_lpfUpdate(&f, 5.0, 0.5);
    printf("lpf=%.1f\n", out);
}
```

```c
/* Slew limiter */
{
    NAV1_SlewLimiter s;
    double out;
    NAV1_CTL_slewInit(&s, 0.0);
    out = NAV1_CTL_slewUpdate(&s, 100.0, 10.0, 1.0);
    printf("slew=%.1f (1st step limited)\n", out);
}
```

```c
/* Moving average */
{
    double buf[4];
    NAV1_MovingAverage ma;
    double out;
    NAV1_CTL_maInit(&ma, buf, 4);
    out = NAV1_CTL_maUpdate(&ma, 1.0);
    out = NAV1_CTL_maUpdate(&ma, 3.0);
    printf("ma after 2=%.1f\n", out);
}
```

```c
/* PID controller */
{
    NAV1_PIDController pid;
    double out;
    NAV1_CTL_pidInit(&pid, 1.0, 0.1, 0.0, -100.0, 100.0);
    out = NAV1_CTL_pidUpdate(&pid, 10.0, 0.0, 1.0);
    printf("pid=%.1f (P only)\n", out);
}
```

```c
/* Complementary filter */
{
    NAV1_ComplementaryFilter cf;
    double out;
    NAV1_CTL_cfInit(&cf, 0.0);
    out = NAV1_CTL_cfUpdate(&cf, 5.0, 0.0, 1.0, 0.5);
    printf("cf=%.1f\n", out);
}
```

```c
/* Angular slew */
{
    NAV1_AngularSlew as;
    double out;
    NAV1_CTL_angularSlewInit(&as, 0.0);
    out = NAV1_CTL_angularSlewUpdate(&as, -33.0, 2.0, 1.0);
    printf("ang slew=%.1f\n", out);
}
```

```c
/* Deadband */
{
    double d = NAV1_CTL_deadband(3.0, 5.0);
    printf("deadband inside=%.1f (expect 0)\n", d);
    d = NAV1_CTL_deadband(6.0, 5.0);
    printf("deadband outside=%.1f (expect 6)\n", d);
}
```

```c
/* Hysteresis */
{
    NAV1_Hysteresis h;
    double out;
    NAV1_CTL_hystInit(&h);
    out = NAV1_CTL_hystUpdate(&h, 1.0, 2.0, 4.0);
    printf("hyst=%.0f (below low)\n", out);
    out = NAV1_CTL_hystUpdate(&h, 3.0, 2.0, 4.0);
    printf("hyst=%.0f (in band, stays low)\n", out);
    out = NAV1_CTL_hystUpdate(&h, 5.0, 2.0, 4.0);
    printf("hyst=%.0f (above high, turns on)\n", out);
}
```

---

## NMEA -- NMEA Parser (3 functions, 3 snippets)

```c
/* nmeaParse -- parse $--GGA or $--RMC */
{
    NAV1_NMEAData nd;
    if (NAV1_NMEA_nmeaParse(
            "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47",
            &nd)) {
        printf("lat=%.6f lon=%.6f alt=%.1f fix=%d\n",
               nd.latitude, nd.longitude, nd.altitudeM, nd.fixQuality);
    }
}
```

```c
/* nmeaChecksumCompute */
{
    unsigned char val;
    char hex[4];
    val = NAV1_NMEA_nmeaChecksumCompute(
            "GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,",
            hex);
    printf("checksum: val=%u hex=%s\n", (unsigned)val, hex);
}
```

```c
/* nmeaParseGSA */
{
    double pdop, hdop, vdop;
    if (NAV1_NMEA_nmeaParseGSA(
            "$GPGSA,A,3,23,29,07,08,09,18,26,28,,,,,1.5,0.9,1.2*35",
            &pdop, &hdop, &vdop))
        printf("PDOP=%.1f HDOP=%.1f VDOP=%.1f\n", pdop, hdop, vdop);
}
```

---

## RTE -- Route Management (15 functions, 8 snippets)

```c
/* routeInit / routeAdd / routeInsert / routeRemove */
{
    NAV1_Waypoint buf[10];
    NAV1_Route r;
    NAV1_RTE_routeInit(&r, buf, 10);
    NAV1_RTE_routeAdd(&r, 48.1173, 11.5167, 545.4, "MMT", "MOOSMOOS", 2);
    NAV1_RTE_routeAdd(&r, 33.9425, -118.4081, 125.0, "LAX",
                       "LOS ANGELES", 1);
    NAV1_RTE_routeInsert(&r, 1, 51.4775, -0.4614, 80.0, "EGLL",
                          "LONDON", 3);
    printf("count=%d\n", r.count);
    NAV1_RTE_routeRemove(&r, 1);
    printf("after remove count=%d\n", r.count);
    NAV1_RTE_routeClear(&r);
    printf("after clear count=%d\n", r.count);
}
```

```c
/* routeSequence / routeDirectTo */
{
    NAV1_Waypoint buf[10];
    NAV1_Route r;
    int idx;
    NAV1_RTE_routeInit(&r, buf, 10);
    NAV1_RTE_routeAdd(&r, 48.1173, 11.5167, 545.4, "MMT", "", 2);
    NAV1_RTE_routeAdd(&r, 33.9425, -118.4081, 125.0, "LAX", "", 1);
    idx = NAV1_RTE_routeSequence(&r);
    printf("seq idx=%d\n", idx);
    if (NAV1_RTE_routeDirectTo(&r, 1))
        printf("direct-to active index=%d\n", r.activeIndex);
}
```

```c
/* routeLegInfo */
{
    NAV1_Waypoint buf[4];
    NAV1_Route r;
    double brg, dist;
    NAV1_RTE_routeInit(&r, buf, 4);
    NAV1_RTE_routeAdd(&r, 48.1173, 11.5167, 545.4, "MMT", "", 2);
    NAV1_RTE_routeAdd(&r, 51.4775, -0.4614, 80.0, "LHR", "", 3);
    if (NAV1_RTE_routeLegInfo(&r, 0, &brg, &dist))
        printf("leg 0: brg=%.2f dist=%.4f NM\n", brg, dist);
}
```

```c
/* routeTurnAnticipation */
{
    NAV1_Waypoint buf[4];
    NAV1_Route r;
    double lat, lon;
    NAV1_RTE_routeInit(&r, buf, 4);
    NAV1_RTE_routeAdd(&r, 48.0, 11.0, 0.0, "A", "", 2);
    NAV1_RTE_routeAdd(&r, 49.0, 11.5, 0.0, "B", "", 2);
    NAV1_RTE_routeAdd(&r, 50.0, 12.0, 0.0, "C", "", 2);
    if (NAV1_RTE_routeTurnAnticipation(&r, 120.0, 3.0, &lat, &lon))
        printf("turn pt: lat=%.6f lon=%.6f\n", lat, lon);
}
```

```c
/* routeSaveCSV / routeLoadCSV -- requires docs/sample_route.csv */
{
    NAV1_Waypoint buf[10];
    NAV1_Route r;
    int n;
    NAV1_RTE_routeInit(&r, buf, 10);
    NAV1_RTE_routeAdd(&r, 48.1173, 11.5167, 545.4, "MMT", "MOOSMOOS", 2);
    if (NAV1_RTE_routeSaveCSV("my_route.csv", &r))
        printf("route saved\n");
    n = NAV1_RTE_routeLoadCSV("docs/sample_route.csv", buf, 10, &r);
    printf("loaded %d waypoints\n", n);
}
```

```c
/* routeReverse / routeFindByIdent */
{
    NAV1_Waypoint buf[4];
    NAV1_Route r;
    int idx;
    NAV1_RTE_routeInit(&r, buf, 4);
    NAV1_RTE_routeAdd(&r, 48.0, 11.0, 0.0, "A", "", 2);
    NAV1_RTE_routeAdd(&r, 49.0, 11.5, 0.0, "B", "", 2);
    NAV1_RTE_routeAdd(&r, 50.0, 12.0, 0.0, "C", "", 2);
    NAV1_RTE_routeReverse(&r);
    printf("after reverse [0]=%s [1]=%s [2]=%s\n",
           r.waypoints[0].ident, r.waypoints[1].ident,
           r.waypoints[2].ident);
    idx = NAV1_RTE_routeFindByIdent(&r, "B");
    printf("find B at index=%d\n", idx);
}
```

```c
/* routeTotalDistance / routeETA */
{
    NAV1_Waypoint buf[4];
    NAV1_Route r;
    NAV1_RTE_routeInit(&r, buf, 4);
    NAV1_RTE_routeAdd(&r, 48.1173, 11.5167, 545.4, "MMT", "", 2);
    NAV1_RTE_routeAdd(&r, 51.4775, -0.4614, 80.0, "LHR", "", 3);
    {
        double td = NAV1_RTE_routeTotalDistance(&r);
        printf("total dist=%.2f NM\n", td);
    }
    {
        double sec;
        if (NAV1_RTE_routeETA(&r, 120.0, &sec))
            printf("ETA=%.1f sec\n", sec);
    }
}
```

---

## XPL -- X-Plane Nav Database (6 functions, 5 snippets)

```c
/* xpNavLoad / xpFixLoad -- requires docs/sample_earth_nav.dat */
{
    NAV1_NavDatabase db;
    int n;
    n = NAV1_XPL_xpNavLoad("docs/sample_earth_nav.dat", &db);
    printf("loaded %d navaids\n", n);
    n = NAV1_XPL_xpFixLoad("docs/sample_earth_fix.dat", &db);
    printf("loaded %d fixes\n", n);
    NAV1_XPL_xpNavFree(&db);
}
```

```c
/* xpNavFindByID */
{
    NAV1_NavDatabase db;
    NAV1_NavRecord rec;
    int n = NAV1_XPL_xpNavLoad("docs/sample_earth_nav.dat", &db);
    if (NAV1_XPL_xpNavFindByID(&db, "MMT", 0, &rec))
        printf("MMT: lat=%.6f lon=%.6f freq=%.3f\n",
               rec.lat, rec.lon, rec.freq);
    NAV1_XPL_xpNavFree(&db);
}
```

```c
/* xpNavFindNearest */
{
    NAV1_NavDatabase db;
    NAV1_NavRecord rec;
    NAV1_XPL_xpNavLoad("docs/sample_earth_nav.dat", &db);
    if (NAV1_XPL_xpNavFindNearest(&db, 48.0, 11.0, 0, &rec))
        printf("nearest: %s lat=%.6f lon=%.6f\n",
               rec.ident, rec.lat, rec.lon);
    NAV1_XPL_xpNavFree(&db);
}
```

```c
/* fplParseRoute */
{
    NAV1_NavDatabase navdb, fixdb;
    NAV1_Waypoint wps[20];
    int n;
    NAV1_XPL_xpNavLoad("docs/sample_earth_nav.dat", &navdb);
    NAV1_XPL_xpFixLoad("docs/sample_earth_fix.dat", &fixdb);
    n = NAV1_XPL_fplParseRoute(
            "N0477F350 DCT SMO J501 RZS DCT BTY",
            &navdb, &fixdb, wps, 20);
    printf("parsed %d waypoints\n", n);
    NAV1_XPL_xpNavFree(&navdb);
}
```

---

## A429 -- ARINC 429 Word (13 functions, 7 snippets)

```c
/* a429Decode -- extract fields from a 32-bit word */
{
    NAV1_ARINC429Word w;
    NAV1_A429_a429Decode(0xA1234567, &w);
    printf("label=%02X sdi=%u data=%06X ssm=%u parity=%d\n",
           w.label, w.sdi, w.data, w.ssm,
           NAV1_A429_a429ParityCheck(0xA1234567));
}
```

```c
/* a429DataBNR / a429DataBCD */
{
    double bnr = NAV1_A429_a429DataBNR(0x88888, 0.5);
    double bcd = NAV1_A429_a429DataBCD(0x12345, 5);
    printf("BNR=%.2f BCD=%.0f\n", bnr, bcd);
}
```

```c
/* a429LabelInfo */
{
    char name[32], unit[16];
    int enc, signed_;
    double lsb;
    if (NAV1_A429_a429LabelInfo(0x10, name, unit, &enc, &lsb, &signed_))
        printf("label 010: %s [%s] enc=%s signed=%d lsb=%g\n",
               name, unit, enc == 0 ? "BNR" : "BCD", signed_, lsb);
}
```

```c
/* a429Encode / a429SetParity */
{
    unsigned int word;
    word = NAV1_A429_a429Encode(0x10, 1, 0x88888, 3);
    word = NAV1_A429_a429SetParity(word);
    printf("encoded word=0x%08X parity=%d\n", word,
           NAV1_A429_a429ParityCheck(word));
}
```

```c
/* a429EncodeBNR */
{
    unsigned int word;
    word = NAV1_A429_a429EncodeBNR(0x10, 1, 35000.0, 1.0, 0, 3);
    word = NAV1_A429_a429SetParity(word);
    printf("BNR encoded=0x%08X\n", word);
}
```

```c
/* a429EncodeBCD */
{
    unsigned int word;
    word = NAV1_A429_a429EncodeBCD(0x10, 1, 12345.0, 5, 3);
    word = NAV1_A429_a429SetParity(word);
    printf("BCD encoded=0x%08X\n", word);
}
```

---

## A424 -- ARINC 424 Database (7 functions, 5 snippets)

```c
/* a424ParseFile -- requires docs/sample_arinc424.txt */
{
    NAV1_A424Database db;
    if (NAV1_A424_a424ParseFile("docs/sample_arinc424.txt", &db)) {
        int i;
        for (i = 0; i < db.nairports; ++i)
            printf("airport: %s %s\n", db.airports[i].icao,
                   db.airports[i].name);
        NAV1_A424_a424Free(&db);
    }
}
```

```c
/* a424FindAirport */
{
    NAV1_A424Database db;
    if (NAV1_A424_a424ParseFile("docs/sample_arinc424.txt", &db)) {
        const NAV1_A424Airport *ap;
        ap = NAV1_A424_a424FindAirport(&db, "KSEA");
        if (ap)
            printf("KSEA: %s elev=%.0f ft\n", ap->name, ap->elevationFt);
        NAV1_A424_a424Free(&db);
    }
}
```

```c
/* a424FindWaypoint / a424FindNearestWaypoint */
{
    NAV1_A424Database db;
    if (NAV1_A424_a424ParseFile("docs/sample_arinc424.txt", &db)) {
        int idx;
        const NAV1_A424Waypoint *wp;
        wp = NAV1_A424_a424FindWaypoint(&db, "KEPEC");
        if (wp)
            printf("KEPEC: lat=%.6f lon=%.6f\n", wp->lat, wp->lon);
        idx = NAV1_A424_a424FindNearestWaypoint(&db, 47.5, 11.0);
        printf("nearest idx=%d\n", idx);
        NAV1_A424_a424Free(&db);
    }
}
```

```c
/* a424FindRunway */
{
    NAV1_A424Database db;
    if (NAV1_A424_a424ParseFile("docs/sample_arinc424.txt", &db)) {
        const NAV1_A424Runway *rw;
        rw = NAV1_A424_a424FindRunway(&db, "KSEA", "16R");
        if (rw)
            printf("16R: len=%.0f ft brg=%.0f\n", rw->lengthFt,
                   rw->bearingDeg);
        NAV1_A424_a424Free(&db);
    }
}
```

```c
/* a424FindILS */
{
    NAV1_A424Database db;
    if (NAV1_A424_a424ParseFile("docs/sample_arinc424.txt", &db)) {
        const NAV1_A424ILS *ils;
        ils = NAV1_A424_a424FindILS(&db, "KSEA", "16R");
        if (ils)
            printf("ILS 16R: freq=%.3f cat=%s\n", ils->freq, ils->cat);
        NAV1_A424_a424Free(&db);
    }
}
```

---

## METAR -- METAR Parser (5 functions, 5 snippets)

```c
/* metarParse */
{
    NAV1_MetarData m;
    if (NAV1_METAR_metarParse(
            "METAR KSEA 231753Z 18010G20KT 10SM -RA BKN020 OVC040 12/09 A2992",
            &m)) {
        printf("KSEA wind %03d%02dG%02dKT vis=%.1fSM temp=%dC dew=%dC\n",
               m.windDirDeg, m.windSpeedKts, m.windGustKts,
               m.visSM_num / (double)m.visSM_den,
               m.tempC, m.dewC);
    }
}
```

```c
/* metarWindAvg */
{
    NAV1_MetarData m;
    NAV1_METAR_metarParse(
            "METAR KSEA 231753Z 18010G20KT 10SM -RA BKN020 OVC040 12/09 A2992",
            &m);
    printf("wind avg=%.1f kt\n", NAV1_METAR_metarWindAvg(&m));
}
```

```c
/* metarCrosswind / metarHeadwind */
{
    NAV1_MetarData m;
    NAV1_METAR_metarParse(
            "METAR KSEA 231753Z 18010G20KT 10SM -RA BKN020 OVC040 12/09 A2992",
            &m);
    printf("xwind=%.1f hwind=%.1f (rwy 16)\n",
           NAV1_METAR_metarCrosswind(&m, 160.0),
           NAV1_METAR_metarHeadwind(&m, 160.0));
}
```

```c
/* metarTemperatureSpread */
{
    NAV1_MetarData m;
    NAV1_METAR_metarParse(
            "METAR KSEA 231753Z 18010G20KT 10SM -RA BKN020 OVC040 12/09 A2992",
            &m);
    printf("temp spread=%.1f C\n", NAV1_METAR_metarTemperatureSpread(&m));
}
```

---

## TAF -- TAF Parser (5 functions, 5 snippets)

```c
/* tafParse */
{
    NAV1_TafData t;
    if (NAV1_TAF_tafParse(
            "TAF KSEA 231730Z 2318/2424 18010G20KT 6SM -RA BKN020\n"
            "     FM240300 19012KT P6SM SCT035\n"
            "     TEMPO 2406/2409 3SM BR",
            &t)) {
        printf("KSEA TAF wind %03d%02dKT ceiling=%d ft\n",
               t.windDirDeg, t.windSpeedKts, NAV1_TAF_tafCeiling(&t));
    }
}
```

```c
/* tafWindAvg */
{
    NAV1_TafData t;
    NAV1_TAF_tafParse(
            "TAF KSEA 231730Z 2318/2424 18010G20KT 6SM -RA BKN020",
            &t);
    printf("wind avg=%.1f kt\n", NAV1_TAF_tafWindAvg(&t));
}
```

```c
/* tafCrosswind / tafHeadwind */
{
    NAV1_TafData t;
    NAV1_TAF_tafParse(
            "TAF KSEA 231730Z 2318/2424 18010G20KT 6SM -RA BKN020",
            &t);
    printf("xwind=%.1f hwind=%.1f (rwy 16)\n",
           NAV1_TAF_tafCrosswind(&t, 160.0),
           NAV1_TAF_tafHeadwind(&t, 160.0));
}
```

```c
/* tafCeiling */
{
    NAV1_TafData t;
    NAV1_TAF_tafParse(
            "TAF KSEA 231730Z 2318/2424 18010G20KT 6SM -RA BKN020",
            &t);
    printf("ceiling=%d ft\n", NAV1_TAF_tafCeiling(&t));
}
```

---

## PERF -- Aircraft Performance (7 functions, 4 snippets)

```c
/* climbGradient / fpaFromGradient */
{
    double gr  = NAV1_PERF_climbGradient(3.0);
    double fpa = NAV1_PERF_fpaFromGradient(5.24);
    printf("3 deg FPA -> %.4f%% 5.24%% -> %.4f deg\n", gr, fpa);
}
```

```c
/* requiredRunwayLength */
{
    double dry = NAV1_PERF_requiredRunwayLength(2000.0, 0);
    double wet = NAV1_PERF_requiredRunwayLength(2000.0, 1);
    double ice = NAV1_PERF_requiredRunwayLength(2000.0, 2);
    printf("dry=%.0f wet=%.0f ice=%.0f\n", dry, wet, ice);
}
```

```c
/* crosswindExceedance / headwindFraction */
{
    double exc = NAV1_PERF_crosswindExceedance(270.0, 30.0, 360.0, 25.0);
    double hwf = NAV1_PERF_headwindFraction(270.0, 20.0, 360.0);
    printf("crosswind exceed=%.1f kt  headwind frac=%.2f\n", exc, hwf);
}
```

```c
/* takeoffDistance / landingDistance */
{
    double to  = NAV1_PERF_takeoffDistance(2000.0, 0, -10.0, 0.0, 0.0);
    double ldg = NAV1_PERF_landingDistance(2000.0, 0, 0.0, 0.0);
    printf("takeoff=%.0f ft  landing=%.0f ft\n", to, ldg);
}
```

---

## WB -- Weight & Balance (9 functions, 5 snippets)

```c
/* moment / cgFromMoments */
{
    double m  = NAV1_WB_moment(100.0, 200.0);
    double cg = NAV1_WB_cgFromMoments(20000.0, 100.0);
    printf("moment=%.0f kg-cm  CG=%.2f cm\n", m, cg);
}
```

```c
/* fuelWeight / fuelVolume */
{
    double w = NAV1_WB_fuelWeight(100.0, 0);   /* 0 = avgas */
    double v = NAV1_WB_fuelVolume(72.0, 0);
    printf("100 L avgas = %.1f kg  72 kg = %.1f L\n", w, v);
    w = NAV1_WB_fuelWeight(100.0, 1);           /* 1 = Jet-A */
    v = NAV1_WB_fuelVolume(80.4, 1);
    printf("100 L Jet-A = %.1f kg  80.4 kg = %.1f L\n", w, v);
}
```

```c
/* cogAdd / cogRemove */
{
    double cg = NAV1_WB_cogAdd(200.0, 100.0, 150.0, 50.0);
    printf("CG after add=%.3f cm\n", cg);
    cg = NAV1_WB_cogRemove(200.0, 100.0, 150.0, 50.0);
    printf("CG after remove=%.3f cm\n", cg);
}
```

```c
/* weightToTonne / tonneToWeight */
{
    double t = NAV1_WB_weightToTonne(1000.0);
    double w = NAV1_WB_tonneToWeight(1.0);
    printf("1000 kg = %.2f t  1 t = %.0f kg\n", t, w);
}
```

```c
/* cgEnvelopeCheck */
{
    int ok = NAV1_WB_cgEnvelopeCheck(150.0, 100.0, 200.0);
    printf("CG within envelope: %s\n", ok ? "yes" : "no");
}
```

---

## VNAV -- Vertical Navigation (6 functions, 4 snippets)

```c
/* verticalProfile */
{
    double fpa, vs, t;
    if (NAV1_VNAV_verticalProfile(35000.0, 1000.0, 100.0, 120.0,
                                   &fpa, &vs, &t))
        printf("descent: FPA=%.2f deg VS=%.0f fpm time=%.0f sec\n",
               fpa, vs, t);
}
```

```c
/* topOfClimb / topOfDescent */
{
    double toc = NAV1_VNAV_topOfClimb(30000.0, 500.0, 120.0);
    double tod = NAV1_VNAV_topOfDescent(30000.0, 500.0, 120.0);
    printf("TOC=%.1f NM  TOD=%.1f NM\n", toc, tod);
}
```

```c
/* requiredVSAtConstraint / altitudeAtDistance */
{
    double vs = NAV1_VNAV_requiredVSAtConstraint(35000.0, 10000.0,
                                                   80.0, 120.0);
    double alt = NAV1_VNAV_altitudeAtDistance(35000.0, -500.0, 120.0, 50.0);
    printf("req VS=%.0f fpm  alt at 50 NM=%.0f ft\n", vs, alt);
}
```

```c
/* stepClimb */
{
    double sc = NAV1_VNAV_stepClimb(10000.0, 20000.0, 500.0, 120.0);
    printf("step climb=%.1f NM\n", sc);
}
```

---

## HOLD -- Holding Pattern (4 functions, 3 snippets)

```c
/* patternInit / outboundHeading */
{
    NAV1_HoldPattern h;
    double oh;
    NAV1_HOLD_patternInit(&h, 48.0, 11.0, 90.0, 1, 10.0);
    oh = NAV1_HOLD_outboundHeading(&h);
    printf("right turn, inbound 090: outbound=%.1f\n", oh);
}
```

```c
/* legPoints */
{
    NAV1_HoldPattern h;
    double obLat, obLon, abLat, abLon;
    NAV1_HOLD_patternInit(&h, 48.0, 11.0, 90.0, 1, 10.0);
    NAV1_HOLD_legPoints(&h, &obLat, &obLon, &abLat, &abLon);
    printf("outbound: lat=%.6f lon=%.6f\n", obLat, obLon);
    printf("abeam:    lat=%.6f lon=%.6f\n", abLat, abLon);
}
```

```c
/* maxHoldingTime */
{
    double t = NAV1_HOLD_maxHoldingTime(50.0, 10.0);
    printf("max hold time=%.0f min\n", t / 60.0);
}
```
