#include "nmea.h"
#include "gps.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static const char *fieldPtr(const char *s, int idx, int *len)
{
    const char *p = s;
    int cur = 0;
    if (*p == '$') p++;
    while (*p && *p != '*' && *p != '\r' && *p != '\n') {
        if (cur == idx) {
            const char *start = p;
            int n = 0;
            while (p[n] && p[n] != ',' && p[n] != '*' && p[n] != '\r' && p[n] != '\n')
                n++;
            *len = n;
            return start;
        }
        if (*p == ',') cur++;
        p++;
    }
    *len = 0;
    return NULL;
}

static int copyField(const char *s, int idx, char *buf, int bufSize)
{
    int len;
    const char *p = fieldPtr(s, idx, &len);
    if (!p) return 0;
    int n = (len < bufSize - 1) ? len : (bufSize - 1);
    memcpy(buf, p, n);
    buf[n] = '\0';
    return n;
}

static double parseCoord(const char *s, int isLat)
{
    int degDigits = isLat ? 2 : 3;
    char degBuf[4] = {0};
    strncpy(degBuf, s, degDigits);
    int degrees = atoi(degBuf);
    double minutes = atof(s + degDigits);
    return degrees + minutes / 60.0;
}

static int parseGGA(const char *s, NAV1_NMEAData *out)
{
    char buf[32];
    int len;

    copyField(s, 1, out->timeUTC, sizeof(out->timeUTC));

    if ((len = copyField(s, 2, buf, sizeof(buf))) > 0) {
        out->latitude = parseCoord(buf, 1);
        if (copyField(s, 3, buf, sizeof(buf)) && buf[0] == 'S')
            out->latitude = -out->latitude;
    }

    if ((len = copyField(s, 4, buf, sizeof(buf))) > 0) {
        out->longitude = parseCoord(buf, 0);
        if (copyField(s, 5, buf, sizeof(buf)) && buf[0] == 'W')
            out->longitude = -out->longitude;
    }

    if (copyField(s, 6, buf, sizeof(buf)))
        out->fixQuality = atoi(buf);

    if (copyField(s, 7, buf, sizeof(buf)))
        out->numSats = atoi(buf);

    if (copyField(s, 8, buf, sizeof(buf)))
        out->hdop = atof(buf);

    if (copyField(s, 9, buf, sizeof(buf)))
        out->altitudeM = atof(buf);

    if (copyField(s, 11, buf, sizeof(buf)))
        out->geoidSepM = atof(buf);

    return out->fixQuality > 0;
}

static int parseRMC(const char *s, NAV1_NMEAData *out)
{
    char buf[32];
    int len;

    copyField(s, 1, out->timeUTC, sizeof(out->timeUTC));

    copyField(s, 9, out->dateUTC, sizeof(out->dateUTC));

    if ((len = copyField(s, 3, buf, sizeof(buf))) > 0) {
        out->latitude = parseCoord(buf, 1);
        if (copyField(s, 4, buf, sizeof(buf)) && buf[0] == 'S')
            out->latitude = -out->latitude;
    }

    if ((len = copyField(s, 5, buf, sizeof(buf))) > 0) {
        out->longitude = parseCoord(buf, 0);
        if (copyField(s, 6, buf, sizeof(buf)) && buf[0] == 'W')
            out->longitude = -out->longitude;
    }

    if (copyField(s, 7, buf, sizeof(buf)))
        out->speedKts = atof(buf);

    if (copyField(s, 8, buf, sizeof(buf)))
        out->trackDeg = atof(buf);

    if (copyField(s, 10, buf, sizeof(buf))) {
        out->magVarDeg = atof(buf);
        if (copyField(s, 11, buf, sizeof(buf)) && buf[0] == 'W')
            out->magVarDeg = -out->magVarDeg;
    }

    return 1;
}

int NAV1_NMEA_nmeaParse(const char *sentence, NAV1_NMEAData *out)
{
    memset(out, 0, sizeof(*out));
    out->altitudeM = -1.0;
    out->geoidSepM = -1.0;
    out->hdop = -1.0;
    out->speedKts = -1.0;
    out->trackDeg = -1.0;
    out->fixQuality = -1;
    out->numSats = -1;

    if (!sentence || sentence[0] != '$')
        return 0;

    if (!NAV1_GPS_nmeaChecksum(sentence))
        return 0;

    char type[8];
    if (!copyField(sentence, 0, type, sizeof(type)))
        return 0;

    int typeLen = strlen(type);
    if (typeLen < 3) return 0;
    const char *suffix = type + typeLen - 3;

    int fix = 0;
    out->hasFix = 0;

    if (strcmp(suffix, "GGA") == 0) {
        fix = parseGGA(sentence, out);
    } else if (strcmp(suffix, "RMC") == 0) {
        fix = parseRMC(sentence, out);
    } else {
        return 0;
    }

    out->hasFix = fix;
    return fix;
}

unsigned char NAV1_NMEA_nmeaChecksumCompute(const char *sentence, char *outHex)
{
    const char *star, *p;
    unsigned char cs = 0;
    if (!sentence || sentence[0] != '$') { outHex[0] = '0'; outHex[1] = '0'; outHex[2] = '\0'; return 0; }
    star = strchr(sentence, '*');
    for (p = sentence + 1; star ? (p < star) : (*p); p++)
        cs ^= (unsigned char)*p;
    sprintf(outHex, "%02X", cs);
    return cs;
}

int NAV1_NMEA_nmeaParseGSA(const char *sentence, double *pdop, double *hdop, double *vdop)
{
    char buf[32], type[8];
    *pdop = *hdop = *vdop = 0.0;
    if (!sentence || !copyField(sentence, 0, type, sizeof(type)))
        return 0;
    {
        int typeLen = (int)strlen(type);
        if (typeLen < 3) return 0;
        const char *suffix = type + typeLen - 3;
        if (strcmp(suffix, "GSA") != 0) return 0;
    }
    if (copyField(sentence, 15, buf, sizeof(buf)))
        *pdop = atof(buf);
    if (copyField(sentence, 16, buf, sizeof(buf)))
        *hdop = atof(buf);
    if (copyField(sentence, 17, buf, sizeof(buf)))
        *vdop = atof(buf);
    return 1;
}
