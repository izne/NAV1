#include "dll.h"
#include "conv.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define VER "1.0"
#define VERSION "Angelov NAV System\nVersion " VER

static const double R = 6371e3; // Earth radius in metres

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    (void)hinstDLL;
    (void)lpvReserved;
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

NAV_EXPORT void Version()
{
    MessageBox(NULL, TEXT(VERSION), TEXT("NAV1.dll"), MB_OK | MB_ICONINFORMATION);
}

NAV_EXPORT void AboutNav()
{
    MessageBox(NULL, TEXT("Angelov NAV System v1.0\nNavigation Library for Windows"),
               TEXT("About NAV1"), MB_OK | MB_ICONINFORMATION);
}

NAV_EXPORT void Testcase(int test)
{
    char buffer[512];
    char result[384];

    switch (test)
    {
        case 1:
        {
            double dist = NAV_SUP_distanceToTarget(53.0, 10.0, 43.0, 27.0);
            snprintf(result, sizeof(result), "Distance: %.2f NM", dist);
            snprintf(buffer, sizeof(buffer), "Test %d: Haversine Distance\n%s", test, result);
            break;
        }
        case 2:
        {
            double hdg = NAV_SUP_headingToTarget(53.0, 10.0, 43.0, 27.0);
            snprintf(result, sizeof(result), "Heading: %.2f deg", hdg);
            snprintf(buffer, sizeof(buffer), "Test %d: Initial Bearing\n%s", test, result);
            break;
        }
        case 3:
        {
            double lat, lon;
            NAV_SUP_destinationPoint(0.0, 0.0, 0.0, 60.0, &lat, &lon);
            snprintf(result, sizeof(result), "Lat: %.4f, Lon: %.4f", lat, lon);
            snprintf(buffer, sizeof(buffer), "Test %d: Destination (0,0 → N 60NM)\n%s", test, result);
            break;
        }
        case 4:
        {
            double lat, lon;
            NAV_SUP_midpoint(52.0, 0.0, 53.0, 1.0, &lat, &lon);
            snprintf(result, sizeof(result), "Lat: %.4f, Lon: %.4f", lat, lon);
            snprintf(buffer, sizeof(buffer), "Test %d: Midpoint (52,0 → 53,1)\n%s", test, result);
            break;
        }
        case 5:
        {
            double brg = NAV_SUP_finalBearing(53.0, 10.0, 43.0, 27.0);
            snprintf(result, sizeof(result), "Final bearing: %.2f deg", brg);
            snprintf(buffer, sizeof(buffer), "Test %d: Final Bearing\n%s", test, result);
            break;
        }
        case 6:
        {
            double xtd = NAV_SUP_crossTrackDistance(0.0, 0.0, 10.0, 0.0, 5.0, 0.5);
            snprintf(result, sizeof(result), "XTD: %.2f NM", xtd);
            snprintf(buffer, sizeof(buffer), "Test %d: Cross-Track Distance\n%s", test, result);
            break;
        }
        case 7:
        {
            int d, m;
            double s;
            double dec = NAV_SUP_dmsToDecimal(53, 30, 0.0, 'N');
            NAV_SUP_decimalToDMS(dec, &d, &m, &s);
            snprintf(buffer, sizeof(buffer), "Test %d: DMS Round-Trip\n53°30'N → %.4f → %d°%d'%.1f\"",
                     test, dec, d, m, s);
            break;
        }
        case 8:
        {
            snprintf(result, sizeof(result), "100 NM = %.2f m", NAV_CONV_nmToM(100));
            snprintf(result + strlen(result), sizeof(result) - strlen(result),
                     "\n100 kts = %.2f m/s", NAV_CONV_knotsToMs(100));
            snprintf(result + strlen(result), sizeof(result) - strlen(result),
                     "\n1000 ft = %.2f m", NAV_CONV_ftToM(1000));
            snprintf(result + strlen(result), sizeof(result) - strlen(result),
                     "\n100 km = %.2f NM", NAV_CONV_kmToNm(100));
            snprintf(buffer, sizeof(buffer), "Test %d: Conversion Smoke Test\n%s", test, result);
            break;
        }
        default:
            snprintf(buffer, sizeof(buffer), "Unknown test ID: %d", test);
            break;
    }

    MessageBox(NULL, buffer, TEXT("Testcase"), MB_OK | MB_ICONINFORMATION);
}

NAV_EXPORT void CALLBACK Rundll32Version(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
    (void)hwnd;
    (void)hinst;
    (void)lpszCmdLine;
    (void)nCmdShow;
    Version();
}

NAV_EXPORT void CALLBACK RunTest(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
    (void)hwnd;
    (void)hinst;
    (void)nCmdShow;
    int testId = atoi(lpszCmdLine);
    Testcase(testId);
}

NAV_EXPORT double NAV_SUP_distanceToTarget(double lat1, double lon1, double lat2, double lon2)
{
    const double _f1 = lat1 * M_PI / 180;
    const double _f2 = lat2 * M_PI / 180;
    const double df = (lat2 - lat1) * M_PI / 180;
    const double dl = (lon2 - lon1) * M_PI / 180;
    const double a = sin(df / 2) * sin(df / 2) + cos(_f1) * cos(_f2) * sin(dl / 2) * sin(dl / 2);
    const double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R * c / 1852.0; // NM
}

NAV_EXPORT double NAV_SUP_headingToTarget(double lat1, double lon1, double lat2, double lon2)
{
    const double lat1_r = lat1 * M_PI / 180;
    const double lat2_r = lat2 * M_PI / 180;
    const double lon1_r = lon1 * M_PI / 180;
    const double lon2_r = lon2 * M_PI / 180;

    const double y = sin(lon2_r - lon1_r) * cos(lat2_r);
    const double x = cos(lat1_r) * sin(lat2_r) - sin(lat1_r) * cos(lat2_r) * cos(lon2_r - lon1_r);
    const double t = atan2(y, x);
    const double bearing = t * 180.0 / M_PI;

    return fmod(bearing + 360.0, 360.0);
}

NAV_EXPORT void NAV_SUP_destinationPoint(double lat1, double lon1, double bearing, double distNm, double *outLat, double *outLon)
{
    const double lat1_r = lat1 * M_PI / 180;
    const double lon1_r = lon1 * M_PI / 180;
    const double brng_r = bearing * M_PI / 180;
    const double d = distNm * 1852.0;
    const double dR = d / R;

    const double lat2_r = asin(sin(lat1_r) * cos(dR) + cos(lat1_r) * sin(dR) * cos(brng_r));
    const double lon2_r = lon1_r + atan2(sin(brng_r) * sin(dR) * cos(lat1_r), cos(dR) - sin(lat1_r) * sin(lat2_r));

    *outLat = lat2_r * 180 / M_PI;
    *outLon = fmod(lon2_r * 180 / M_PI + 540, 360) - 180;
}

NAV_EXPORT void NAV_SUP_midpoint(double lat1, double lon1, double lat2, double lon2, double *outLat, double *outLon)
{
    const double lat1_r = lat1 * M_PI / 180;
    const double lat2_r = lat2 * M_PI / 180;
    const double lon1_r = lon1 * M_PI / 180;
    const double lon2_r = lon2 * M_PI / 180;
    const double dl = lon2_r - lon1_r;

    const double Bx = cos(lat2_r) * cos(dl);
    const double By = cos(lat2_r) * sin(dl);

    const double lat3_r = atan2(sin(lat1_r) + sin(lat2_r), sqrt((cos(lat1_r) + Bx) * (cos(lat1_r) + Bx) + By * By));
    const double lon3_r = lon1_r + atan2(By, cos(lat1_r) + Bx);

    *outLat = lat3_r * 180 / M_PI;
    *outLon = fmod(lon3_r * 180 / M_PI + 540, 360) - 180;
}

NAV_EXPORT double NAV_SUP_finalBearing(double lat1, double lon1, double lat2, double lon2)
{
    const double lat1_r = lat1 * M_PI / 180;
    const double lat2_r = lat2 * M_PI / 180;
    const double lon1_r = lon1 * M_PI / 180;
    const double lon2_r = lon2 * M_PI / 180;

    const double y = sin(lon1_r - lon2_r) * cos(lat1_r);
    const double x = cos(lat2_r) * sin(lat1_r) - sin(lat2_r) * cos(lat1_r) * cos(lon1_r - lon2_r);
    const double t = atan2(y, x);

    return fmod(t * 180 / M_PI + 540, 360);
}

NAV_EXPORT double NAV_SUP_crossTrackDistance(double lat1, double lon1, double lat2, double lon2, double lat3, double lon3)
{
    const double lat1_r = lat1 * M_PI / 180;
    const double lon1_r = lon1 * M_PI / 180;
    const double lat2_r = lat2 * M_PI / 180;
    const double lon2_r = lon2 * M_PI / 180;
    const double lat3_r = lat3 * M_PI / 180;
    const double lon3_r = lon3 * M_PI / 180;

    const double df_13 = (lat3 - lat1) * M_PI / 180;
    const double dl_13 = (lon3 - lon1) * M_PI / 180;
    const double a13 = sin(df_13 / 2) * sin(df_13 / 2) + cos(lat1_r) * cos(lat3_r) * sin(dl_13 / 2) * sin(dl_13 / 2);
    const double c13 = 2 * atan2(sqrt(a13), sqrt(1 - a13));

    const double y13 = sin(lon3_r - lon1_r) * cos(lat3_r);
    const double x13 = cos(lat1_r) * sin(lat3_r) - sin(lat1_r) * cos(lat3_r) * cos(lon3_r - lon1_r);
    const double brng13 = atan2(y13, x13);

    const double y12 = sin(lon2_r - lon1_r) * cos(lat2_r);
    const double x12 = cos(lat1_r) * sin(lat2_r) - sin(lat1_r) * cos(lat2_r) * cos(lon2_r - lon1_r);
    const double brng12 = atan2(y12, x12);

    double arg = sin(c13) * sin(brng13 - brng12);
    if (arg > 1.0) arg = 1.0;
    if (arg < -1.0) arg = -1.0;
    const double xtd = asin(arg) * R;

    return xtd / 1852.0; // NM
}

NAV_EXPORT double NAV_SUP_dmsToDecimal(int degrees, int minutes, double seconds, char dir)
{
    double decimal = abs(degrees) + minutes / 60.0 + seconds / 3600.0;
    if (dir == 'S' || dir == 'W' || dir == 's' || dir == 'w')
        decimal = -decimal;
    return decimal;
}

NAV_EXPORT void NAV_SUP_decimalToDMS(double decimal, int *degrees, int *minutes, double *seconds)
{
    double absDec = fabs(decimal);
    *degrees = (int)absDec;
    double rem = (absDec - *degrees) * 60;
    *minutes = (int)rem;
    *seconds = (rem - *minutes) * 60;
}
