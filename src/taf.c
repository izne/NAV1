#include "taf.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define TK 120

static const char *nextToken(const char *p, char *tok)
{
    int i = 0;
    while (*p == ' ') ++p;
    if (!*p) { tok[0] = '\0'; return (const char *)0; }
    while (*p && *p != ' ' && i < TK - 1) tok[i++] = *p++;
    tok[i] = '\0';
    return p;
}

static int strEnds(const char *s, const char *suf)
{
    int ls = (int)strlen(s);
    int lf = (int)strlen(suf);
    if (lf > ls) return 0;
    return strcmp(s + ls - lf, suf) == 0;
}

static int cloudCode(const char *s)
{
    if (strcmp(s, "FEW") == 0) return 1;
    if (strcmp(s, "SCT") == 0) return 2;
    if (strcmp(s, "BKN") == 0) return 3;
    if (strcmp(s, "OVC") == 0) return 4;
    if (strcmp(s, "VV")   == 0) return 5;
    return 0;
}

static int parseWindGroup(const char *tok, int *dir, int *speed, int *gust)
{
    char buf[32];
    int n, d, s, g;

    *speed = 0; *gust = 0; *dir = -1;

    if (strncmp(tok, "00000", 5) == 0) { *dir = 0; *speed = 0; return 1; }
    if (strncmp(tok, "VRB", 3) == 0) {
        *dir = -1;
        sscanf(tok + 3, "%d", &s);
        *speed = s;
        if (strstr(tok, "G")) { const char *gp = strstr(tok, "G"); sscanf(gp + 1, "%d", &g); *gust = g; }
        return 1;
    }
    strcpy(buf, tok);
    n = (int)strlen(buf);
    if (n < 5) return 0;
    if (strEnds(buf, "KT")) buf[n - 2] = '\0';
    else if (strEnds(buf, "KMH")) buf[n - 3] = '\0';
    else if (strEnds(buf, "MPS")) buf[n - 3] = '\0';
    if (sscanf(buf, "%3d%2d", &d, &s) >= 2) {
        *dir = d; *speed = s;
        { const char *gp = strchr(buf, 'G'); if (gp) { sscanf(gp + 1, "%d", &g); *gust = g; } }
        return 1;
    }
    return 0;
}

static int parseVisSM(const char *tok, int *num, int *den)
{
    char buf[32];
    *num = 0; *den = 0;
    if (strncmp(tok, "M", 1) == 0) {
        int n, d;
        if (sscanf(tok + 1, "%d/%dSM", &n, &d) == 2) { *num = n; *den = d; return 1; }
        return 0;
    }
    strcpy(buf, tok);
    if (strEnds(buf, "SM")) {
        buf[strlen(buf) - 2] = '\0';
        { const char *p = strchr(buf, '/');
          if (p) { int len = (int)(p - buf); char intPart[16]; strncpy(intPart, buf, len); intPart[len] = '\0'; sscanf(p + 1, "%d", den); sscanf(intPart, "%d", num); }
          else { sscanf(buf, "%d", num); *den = 1; }
        }
        return 1;
    }
    return 0;
}

static int parseCloudToken(const char *tok, int *code, int *alt, char *type)
{
    char buf[32];
    int a;
    const char *p;
    *alt = 0; type[0] = '\0';
    if (strncmp(tok, "VV", 2) == 0) { *code = 5; strcpy(buf, tok + 2); }
    else {
        if (strlen(tok) < 3) return 0;
        strncpy(buf, tok, 3); buf[3] = '\0';
        *code = cloudCode(buf);
        if (!*code) return 0;
        strcpy(buf, tok + 3);
    }
    p = buf;
    if (!*p) return 1;
    a = 0;
    while (*p && isdigit((unsigned char)*p)) { a = a * 10 + (*p - '0'); ++p; }
    *alt = a * 100;
    if (*p) strcpy(type, p);
    return 1;
}

static int parseVisibility(const char *tok, int *visM, int *smNum, int *smDen)
{
    int smn, smd;
    *visM = -2; *smNum = 0; *smDen = 0;

    if (strcmp(tok, "CAVOK") == 0) { *visM = -1; return 1; }
    if (strcmp(tok, "////") == 0) { *visM = -2; return 1; }

    if (strEnds(tok, "SM") || strcmp(tok, "M1/4SM") == 0) {
        if (parseVisSM(tok, &smn, &smd)) {
            *smNum = smn; *smDen = smd;
            *visM = (int)((double)smn / smd * 1609.344 + 0.5);
            return 1;
        }
    }

    if (strEnds(tok, "M")) {
        int len = (int)strlen(tok);
        char buf[32]; strcpy(buf, tok); buf[len - 1] = '\0';
        if (sscanf(buf, "%d", visM) == 1) return 1;
    }

    {
        const char *p = tok;
        int allDigit = 1;
        for (p = tok; *p; ++p) if (!isdigit((unsigned char)*p)) { allDigit = 0; break; }
        if (allDigit) { sscanf(tok, "%d", visM); return 1; }
    }

    return 0;
}

static int parseTime(const char *tok, int *d, int *h, int *mn)
{
    int len = (int)strlen(tok);
    if (len >= 6) {
        char buf[16];
        strcpy(buf, tok);
        if (buf[len - 1] == 'Z') buf[len - 1] = '\0';
        if (sscanf(buf, "%2d%2d%2d", d, h, mn) == 3) return 1;
    }
    return 0;
}

NAV1_EXPORT int NAV1_TAF_tafParse(const char *raw, NAV1_TafData *m)
{
    char tok[TK];
    const char *cp;
    int state = 0;
    int inPeriod = 0;
    int pendingProb = 0;
    NAV1_TafPeriod *cur = 0;

    if (!raw || !*raw || !m) return 0;

    memset(m, 0, sizeof(*m));
    m->windDir = -2;
    m->windSpeedKt = -1;
    m->tempMinC = 999.0;
    m->tempMaxC = -999.0;
    m->windGustKt = 0;

    cp = raw;

    while (*cp) {
        cp = nextToken(cp, tok);
        if (!tok[0]) break;

        if (strcmp(tok, "RMK") == 0 || strcmp(tok, "REM") == 0) break;
        if (strcmp(tok, "//") == 0) continue;
        if (strcmp(tok, "=") == 0) break;

        if (state == 0) {
            if (strcmp(tok, "TAF") == 0) { state = 1; continue; }
            if (strcmp(tok, "TAF") != 0 && strlen(tok) >= 4
                && isalpha((unsigned char)*tok) && isupper((unsigned char)*tok)) {
                if (strncmp(tok, "TAF", 3) == 0) {
                    const char *p = tok + 3;
                    if (*p == ' ') p++;
                    if (*p) { strcpy(tok, p); cp = tok; }
                    state = 1; continue;
                }
            }
            if (strlen(tok) >= 4 && isalpha((unsigned char)*tok)) {
                strncpy(m->icao, tok, sizeof(m->icao) - 1);
                m->icao[sizeof(m->icao) - 1] = '\0';
                state = 2; continue;
            }
            state = 1;
        }

        if (state == 1) {
            if (strlen(tok) >= 4 && isalpha((unsigned char)*tok)) {
                strncpy(m->icao, tok, sizeof(m->icao) - 1);
                m->icao[sizeof(m->icao) - 1] = '\0';
                state = 2; continue;
            }
            state = 2;
        }

        if (state == 2) {
            int d, h, mn;
            if (parseTime(tok, &d, &h, &mn)) {
                m->day = d; m->hour = h; m->minute = mn;
                state = 3; continue;
            }
            state = 3;
        }

        if (state == 3) {
            int d, h, mn;
            if (parseTime(tok, &d, &h, &mn)) {
                m->validityDay = d; m->validityHour = h; m->validityMin = mn;
                state = 4; continue;
            }
            {
                int len = (int)strlen(tok);
                if (len >= 5 && strchr(tok, '/') != 0) {
                    state = 4; continue;
                }
            }
            state = 4;
        }

        if (state == 4) {
            if (strcmp(tok, "TEMPO") == 0 || strcmp(tok, "BECMG") == 0 ||
                strcmp(tok, "INTER") == 0) {
                if (m->periodCount < NAV1_TAF_MAX_PERIODS) {
                    cur = &m->periods[m->periodCount];
                    memset(cur, 0, sizeof(NAV1_TafPeriod));
                    strcpy(cur->changeType, tok);
                    m->periodCount++;
                    inPeriod = 1;
                    state = 99; continue;
                }
            }
            if (strncmp(tok, "FM", 2) == 0 && strlen(tok) >= 8) {
                char buf[32];
                int d, h, mn;
                if (m->periodCount < NAV1_TAF_MAX_PERIODS) {
                    cur = &m->periods[m->periodCount];
                    memset(cur, 0, sizeof(NAV1_TafPeriod));
                    strcpy(cur->changeType, "FM");
                    strcpy(buf, tok + 2);
                    buf[6] = '\0';
                    if (sscanf(buf, "%2d%2d%2d", &d, &h, &mn) == 3) {
                        cur->fromHour = h; cur->fromMin = mn;
                    }
                    m->periodCount++;
                    inPeriod = 1;
                    state = 4; continue;
                }
            }
            {
                int d, s, g;
                if (strEnds(tok, "KT") || strEnds(tok, "KMH") || strEnds(tok, "MPS") ||
                    strncmp(tok, "VRB", 3) == 0) {
                    if (parseWindGroup(tok, &d, &s, &g)) {
                        if (inPeriod && cur) {
                            cur->windDir = d;
                            cur->windSpeedKt = s;
                            cur->windGustKt = g;
                        } else {
                            m->windDir = d;
                            m->windSpeedKt = s;
                            m->windGustKt = g;
                        }
                        state = 5; continue;
                    }
                }
            }
            state = 5;
        }

        if (state == 5) {
            int vm, smn, smd;
            if (parseVisibility(tok, &vm, &smn, &smd)) {
                if (inPeriod && cur) {
                    cur->visibilityM = vm; cur->visibilitySM_num = smn; cur->visibilitySM_den = smd;
                }
                state = 6; continue;
            }
            state = 6;
        }

        if (state == 6) {
            if (tok[0] == 'R' && tok[1] != '\0') {
                state = 7; continue;
            }
            state = 7;
        }

        if (state == 7) {
            int cc, ca;
            char ctype[8];
            if (strcmp(tok, "SKC") == 0 || strcmp(tok, "CLR") == 0) {
                state = 8; continue;
            }
            if (strncmp(tok, "FEW", 3) == 0 || strncmp(tok, "SCT", 3) == 0 ||
                strncmp(tok, "BKN", 3) == 0 || strncmp(tok, "OVC", 3) == 0 ||
                strncmp(tok, "VV", 2) == 0) {
                if (parseCloudToken(tok, &cc, &ca, ctype)) {
                    {
                        NAV1_TafCloud *pc = inPeriod && cur ? cur->clouds : m->periods[0].clouds;
                        int *pcnt = inPeriod && cur ? &cur->cloudCount : &m->periods[0].cloudCount;
                        if (*pcnt < NAV1_TAF_MAX_CLOUDS) {
                            NAV1_TafCloud *c = &pc[*pcnt];
                            c->code = cc; c->altitudeFt = ca; strcpy(c->type, ctype);
                            (*pcnt)++;
                        }
                    }
                }
                continue;
            }
            state = 8;
        }

        if (state == 8) {
            if (tok[0] == 'T' && tok[1] != '\0') {
                char sign1 = tok[1];
                int temp1 = 0, temp2 = 0;
                if (strlen(tok) >= 5) {
                    if (tok[2] == 'X' && (tok[3] == 'M' || tok[3] == '0')) {
                        int off = (tok[3] == 'M') ? 3 : 3;
                        sscanf(tok + off, "%d", &temp1);
                        m->tempMaxC = (double)temp1;
                        if (tok[3] == 'M') m->tempMaxC = -m->tempMaxC;
                    }
                }
                if (strlen(tok) >= 7 && sign1 == 'X') {
                    int off = 4;
                    if (tok[off] == 'M') { m->tempMaxC = -(double)(tok[off+1]-'0'); off += 2; }
                }
            }
            if (strlen(tok) >= 3 && tok[0] == 'T' && isdigit((unsigned char)tok[1])) {
                int t1, t2;
                if (sscanf(tok + 1, "%2d%2d", &t1, &t2) == 2) {
                    if (t1 >= 100) { t1 = t1 - 100; m->tempMaxC = (double)t1; }
                    else if (t1 < 100) { m->tempMinC = (double)t1; }
                    if (t2 >= 100) { t2 = t2 - 100; m->tempMinC = (double)t2; }
                    else if (t2 < 100) { m->tempMinC = (double)t2; }
                }
                state = 9; continue;
            }
            state = 9;
        }

        if (state >= 9) {
            if (strncmp(tok, "PROB", 4) == 0) {
                sscanf(tok + 4, "%d", &pendingProb);
                state = 99;
                continue;
            }

            if (strcmp(tok, "TEMPO") == 0 || strcmp(tok, "BECMG") == 0 ||
                strcmp(tok, "INTER") == 0) {
                if (m->periodCount < NAV1_TAF_MAX_PERIODS) {
                    cur = &m->periods[m->periodCount];
                    memset(cur, 0, sizeof(NAV1_TafPeriod));
                    strcpy(cur->changeType, tok);
                    cur->prob = pendingProb;
                    pendingProb = 0;
                    m->periodCount++;
                    inPeriod = 1;
                    state = 99;
                    continue;
                }
            }

            if (strncmp(tok, "FM", 2) == 0 && strlen(tok) >= 8) {
                int d, h, mn;
                char buf[32];
                if (m->periodCount < NAV1_TAF_MAX_PERIODS) {
                    cur = &m->periods[m->periodCount];
                    memset(cur, 0, sizeof(NAV1_TafPeriod));
                    strcpy(cur->changeType, "FM");
                    cur->prob = pendingProb;
                    pendingProb = 0;
                    strcpy(buf, tok + 2);
                    buf[6] = '\0';
                    if (sscanf(buf, "%2d%2d%2d", &d, &h, &mn) == 3) {
                        cur->fromHour = h; cur->fromMin = mn;
                    }
                    m->periodCount++;
                    inPeriod = 1;
                    state = 4;
                    continue;
                }
            }

            state = 10;
        }
    }

    m->valid = 1;
    return 1;
}

NAV1_EXPORT double NAV1_TAF_tafWindAvg(const NAV1_TafData *t)
{
    if (!t) return 0.0;
    return (double)t->windSpeedKt;
}

static double deg2rad(double d) { return d * 3.14159265358979323846 / 180.0; }

NAV1_EXPORT double NAV1_TAF_tafCrosswind(const NAV1_TafData *t, double rwyHdg)
{
    double d, ws;
    if (!t || t->windDir < 0) return 0.0;
    d = deg2rad(rwyHdg - (double)t->windDir);
    ws = (double)t->windSpeedKt;
    return ws * sin(d);
}

NAV1_EXPORT double NAV1_TAF_tafHeadwind(const NAV1_TafData *t, double rwyHdg)
{
    double d, ws;
    if (!t || t->windDir < 0) return 0.0;
    d = deg2rad((double)t->windDir - rwyHdg);
    ws = (double)t->windSpeedKt;
    return ws * cos(d);
}

NAV1_EXPORT int NAV1_TAF_tafCeiling(const NAV1_TafData *t)
{
    int i;
    if (!t) return 0;
    for (i = 0; i < t->periods[0].cloudCount; i++) {
        int code = t->periods[0].clouds[i].code;
        if (code == 3 || code == 4 || code == 5)
            return t->periods[0].clouds[i].altitudeFt;
    }
    return 0;
}
