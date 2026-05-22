#include "metar.h"
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

static int strStarts(const char *s, const char *pre)
{
    while (*pre)
        if (*pre++ != *s++) return 0;
    return 1;
}

static int strEnds(const char *s, const char *suf)
{
    int ls = (int)strlen(s);
    int lf = (int)strlen(suf);
    if (lf > ls) return 0;
    return strcmp(s + ls - lf, suf) == 0;
}



static const char *descriptorTbl[] = {
    "MI","PR","BC","DR","BL","SH","TS","FZ",(const char *)0
};
static const char *phenomenaTbl[] = {
    "DZ","RA","SN","SG","IC","PL","GR","GS","UP",
    "BR","FG","FU","VA","DU","SA","HZ","PY","PO","SQ",
    "FC","SS","DS",(const char *)0
};

static int isDescriptor(const char *s)
{
    int i;
    for (i = 0; descriptorTbl[i]; ++i)
        if (strcmp(s, descriptorTbl[i]) == 0) return 1;
    return 0;
}

static int isPhenomena(const char *s)
{
    int i;
    for (i = 0; phenomenaTbl[i]; ++i)
        if (strcmp(s, phenomenaTbl[i]) == 0) return 1;
    return 0;
}

static void classifyWeather(const char *tok, char *intensity,
                            char *descr, char *phen)
{
    const char *start = tok;
    char buf1[8], buf2[8], buf3[8];

    intensity[0] = '\0';
    descr[0]     = '\0';
    phen[0]      = '\0';

    if (*start == '+' || *start == '-') {
        intensity[0] = *start++;
        intensity[1] = '\0';
    }

    while (*start) {
        buf1[0] = start[0]; buf1[1] = start[1] ? start[1] : '\0';
        buf1[2] = '\0';
        if (!buf1[1]) break;
        if (!descr[0] && isDescriptor(buf1)) {
            strcpy(descr, buf1);
            start += 2;
            continue;
        }
        if (!phen[0] && isPhenomena(buf1)) {
            strcpy(phen, buf1);
            start += 2;
            continue;
        }

        if (phen[0] && !buf3[0] && isPhenomena(buf1)) {
            strcpy(buf3, buf1);
            start += 2;
            continue;
        }

        break;
    }
    if (buf3[0]) {
        strcat(phen, buf3);
    }
    if (!phen[0] && !descr[0] && intensity[0]) {

        strcpy(phen, tok + 1);
    }
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



static int parseWindGroup(const char *tok, int *dir, int *speed,
                          int *gust)
{
    char buf[32];
    int n, d, s, g;

    *speed = 0;
    *gust  = 0;
    *dir   = -1;


    if (strncmp(tok, "00000", 5) == 0) {
        *dir = 0; *speed = 0; return 1;
    }

    if (strncmp(tok, "VRB", 3) == 0) {
        *dir = -1;
        sscanf(tok + 3, "%d", &s);
        *speed = s;

        if (strstr(tok, "G")) {
            const char *gp = strstr(tok, "G");
            sscanf(gp + 1, "%d", &g);
            *gust = g;
        }
        return 1;
    }

    strcpy(buf, tok);
    n = (int)strlen(buf);
    if (n < 5) return 0;

    if (strEnds(buf, "KT"))   buf[n - 2] = '\0';
    else if (strEnds(buf, "KMH")) buf[n - 3] = '\0';
    else if (strEnds(buf, "MPS")) buf[n - 3] = '\0';

    if (sscanf(buf, "%3d%2d", &d, &s) >= 2) {
        *dir = d;
        *speed = s;

        {
            const char *gp = strchr(buf, 'G');
            if (gp) {
                sscanf(gp + 1, "%d", &g);
                *gust = g;
            }
        }
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
        if (sscanf(tok + 1, "%d/%dSM", &n, &d) == 2) {
            *num = n; *den = d;
            return 1;
        }
        return 0;
    }

    strcpy(buf, tok);
    if (strEnds(buf, "SM")) {
        buf[strlen(buf) - 2] = '\0';
        {
            const char *p = strchr(buf, '/');
            if (p) {
                int len = (int)(p - buf);
                char intPart[16];
                strncpy(intPart, buf, len); intPart[len] = '\0';
                sscanf(p + 1, "%d", den);
                sscanf(intPart, "%d", num);
            } else {
                sscanf(buf, "%d", num);
                *den = 1;
            }
        }
        return 1;
    }
    return 0;
}



static int parseTempDew(const char *tok, double *tempC, double *dewC)
{
    int t, d, st, sd;
    char buf[32];

    *tempC = 999.0;
    *dewC  = 999.0;

    if (!strchr(tok, '/')) return 0;
    strcpy(buf, tok);
    {
        char *slash = strchr(buf, '/');
        *slash = '\0';


        st = 1;
        if (buf[0] == 'M') { st = -1; memmove(buf, buf + 1, strlen(buf)); }
        if (sscanf(buf, "%d", &t) == 1) *tempC = (double)(t * st);


        sd = 1;
        if (slash[1] == 'M') { sd = -1; memmove(slash + 1, slash + 2, strlen(slash + 1)); }
        if (sscanf(slash + 1, "%d", &d) == 1) *dewC = (double)(d * sd);
    }
    return 1;
}



static int parsePressure(const char *tok, int *hPa)
{
    int val;
    *hPa = 0;
    if (*tok == 'Q' && strlen(tok) >= 4) {
        if (sscanf(tok + 1, "%d", &val) == 1) {
            *hPa = val;
            return 1;
        }
    }
    if (*tok == 'A' && strlen(tok) >= 5) {

        int ih, il;
        if (sscanf(tok + 1, "%2d%2d", &ih, &il) == 2) {
            double inHg = ih + il / 100.0;
            *hPa = (int)(inHg * 33.8639 + 0.5);
            return 1;
        }
    }
    return 0;
}



static int parseCloudToken(const char *tok, int *code, int *alt, char *type)
{
    char buf[32];
    int a;
    const char *p;

    *alt = 0; type[0] = '\0';


    if (strncmp(tok, "VV", 2) == 0) {
        *code = 5;
        strcpy(buf, tok + 2);
    } else {
        if (strlen(tok) < 3) return 0;
        strncpy(buf, tok, 3); buf[3] = '\0';
        *code = cloudCode(buf);
        if (!*code) return 0;
        strcpy(buf, tok + 3);
    }


    p = buf;
    if (!*p) return 1;

    a = 0;
    while (*p && isdigit((unsigned char)*p)) {
        a = a * 10 + (*p - '0');
        ++p;
    }
    *alt = a * 100;


    if (*p) strcpy(type, p);

    return 1;
}



NAV_EXPORT int NAV1_METAR_metarParse(const char *raw, NAV1_MetarData *m)
{
    char tok[TK], pToken[TK];
    const char *cp;
    int state = 0;


    int prevWasInt = 0;

    if (!raw || !*raw || !m) return 0;

    memset(m, 0, sizeof(*m));
    m->windDir     = -2;
    m->windSpeedKt = -1;
    m->visibilityM = -2;
    m->tempC       = 999.0;
    m->dewpointC   = 999.0;
    m->qnhHPa      = 0;
    m->windGustKt  = 0;
    m->windVarLow  = -1;
    m->windVarHigh = -1;

    cp = raw;
    pToken[0] = '\0';

    while (*cp) {
        cp = nextToken(cp, tok);
        if (!tok[0]) break;


        if (strcmp(tok, "RMK") == 0) break;
        if (strcmp(tok, "REM") == 0) break;
        if (strcmp(tok, "//")  == 0) continue;


        if (state == 0) {
            if (strlen(tok) >= 3 && isalpha((unsigned char)*tok)) {
                strncpy(m->icao, tok, sizeof(m->icao) - 1);
                m->icao[sizeof(m->icao) - 1] = '\0';
                state = 1;
                continue;
            }
        }


        if (state == 1) {
            int len = (int)strlen(tok);
            if (len == 7 && tok[6] == 'Z') {
                int d, h, mn;
                tok[6] = '\0';
                if (sscanf(tok, "%2d%2d%2d", &d, &h, &mn) == 3) {
                    m->day    = d;
                    m->hour   = h;
                    m->minute = mn;
                }
                state = 2;
                continue;
            }
        }


        if (state == 2) {
            int d, s, g;
            if (strEnds(tok, "KT") || strEnds(tok, "KMH") || strEnds(tok, "MPS") ||
                strncmp(tok, "VRB", 3) == 0) {
                if (parseWindGroup(tok, &d, &s, &g)) {
                    m->windDir     = d;
                    m->windSpeedKt = s;
                    m->windGustKt  = g;
                    state = 3;
                    continue;
                }
            }
        }


        if (state == 3) {
            int len = (int)strlen(tok);
            if (len == 7 && tok[3] == 'V') {
                int lo, hi;
                if (sscanf(tok, "%3dV%3d", &lo, &hi) == 2) {
                    m->windVarLow  = lo;
                    m->windVarHigh = hi;
                }
                state = 4;
                continue;
            }

            state = 4;
        }


        if (state == 4) {
            int smn, smd, vm;

            if (strcmp(tok, "CAVOK") == 0) {
                m->visibilityM = -1;
                state = 5; prevWasInt = 0;
                continue;
            }

            if (prevWasInt && strEnds(tok, "SM")) {
                int whole;
                sscanf(pToken, "%d", &whole);
                if (parseVisSM(tok, &smn, &smd)) {
                    m->visibilitySM_num = whole * smd + smn;
                    m->visibilitySM_den = smd;
                    m->visibilityM = (int)((double)m->visibilitySM_num / smd * 1609.344 + 0.5);
                }
                prevWasInt = 0; pToken[0] = '\0'; state = 5;
                continue;
            }

            if (strEnds(tok, "SM") || strcmp(tok, "M1/4SM") == 0) {
                if (parseVisSM(tok, &smn, &smd)) {
                    m->visibilitySM_num = smn;
                    m->visibilitySM_den = smd;
                    m->visibilityM = (int)((double)smn / smd * 1609.344 + 0.5);
                    state = 5; prevWasInt = 0;
                    continue;
                }
            }

            {
                const char *p = tok;
                int allDigit = 1, len = 0;
                for (p = tok; *p; ++p)
                    if (!isdigit((unsigned char)*p)) { allDigit = 0; break; }
                    else ++len;
                if (allDigit) {
                    if (len <= 2) {

                        strcpy(pToken, tok); prevWasInt = 1;
                        continue;
                    }

                    sscanf(tok, "%d", &vm);
                    m->visibilityM = vm;
                    state = 5; prevWasInt = 0;
                    continue;
                }
            }

            prevWasInt = 0; state = 5;
        }


        if (state == 5) {
            if (tok[0] == 'R' && tok[1] != '\0') {
                if (m->rvr[0])
                    strncat(m->rvr, " ", sizeof(m->rvr) - strlen(m->rvr) - 1);
                strncat(m->rvr, tok, sizeof(m->rvr) - strlen(m->rvr) - 1);
                continue;
            }
            state = 6;
        }


        if (state == 6) {
            char intensity[4], descr[16], phen[24];
            classifyWeather(tok, intensity, descr, phen);
            if (intensity[0] || descr[0] || phen[0]) {
                if (m->skyCount < NAV1_METAR_MAX_WEATHER) {
                    NAV1_MetarWeather *w = &m->sky[m->skyCount++];
                    w->intensity   = intensity[0] ? intensity[0] : ' ';
                    strcpy(w->descriptor, descr);
                    strcpy(w->phenomena, phen);
                }
                continue;
            }
            state = 7;
        }


        if (state == 7) {
            int cc, ca;
            char ctype[8];
            if (strcmp(tok, "SKC") == 0 || strcmp(tok, "CLR") == 0) {
                state = 8;
                continue;
            }
            if (strncmp(tok, "FEW", 3) == 0 || strncmp(tok, "SCT", 3) == 0 ||
                strncmp(tok, "BKN", 3) == 0 || strncmp(tok, "OVC", 3) == 0 ||
                strncmp(tok, "VV",  2) == 0) {
                if (parseCloudToken(tok, &cc, &ca, ctype)) {
                    if (m->cloudCount < NAV1_METAR_MAX_CLOUDS) {
                        NAV1_MetarCloud *c = &m->clouds[m->cloudCount++];
                        c->code        = cc;
                        c->altitudeFt  = ca;
                        strcpy(c->type, ctype);
                    }
                    continue;
                }
            }
            state = 8;
        }


        if (state == 8) {
            double t, d;
            if (parseTempDew(tok, &t, &d)) {
                m->tempC    = t;
                m->dewpointC = d;
                state = 9;
                continue;
            }
            state = 9;
        }


        if (state == 9) {
            int hp;
            if (parsePressure(tok, &hp)) {
                m->qnhHPa = hp;
                state = 10;
                continue;
            }
            state = 10;
        }


        if (state == 10) {
            if (strncmp(tok, "RE", 2) == 0 && strlen(tok) > 2) {
                continue;
            }
            if (strncmp(tok, "WS", 2) == 0) {
                continue;
            }
            state = 11;

        }


        if (state == 11) {
            if (strcmp(tok, "NOSIG") == 0 ||
                strcmp(tok, "BECMG") == 0 ||
                strcmp(tok, "TEMPO") == 0) {
                strncpy(m->trend, tok, sizeof(m->trend) - 1);
                state = 12;
                continue;
            }
        }

        if (state >= 12) break;

        prevWasInt = 0;
    }

    m->valid = 1;
    return 1;
}

NAV_EXPORT double NAV1_METAR_metarWindAvg(const NAV1_MetarData *m)
{
    if (!m) return 0.0;
    return (double)m->windSpeedKt;
}


static double deg2rad(double d) { return d * 3.14159265358979323846 / 180.0; }

NAV_EXPORT double NAV1_METAR_metarCrosswind(const NAV1_MetarData *m,
                                          double rwyHdg)
{
    double d, ws;
    if (!m || m->windDir < 0) return 0.0;
    d  = deg2rad(rwyHdg - (double)m->windDir);
    ws = (double)m->windSpeedKt;
    return ws * sin(d);
}

NAV_EXPORT double NAV1_METAR_metarHeadwind(const NAV1_MetarData *m,
                                         double rwyHdg)
{
    double d, ws;
    if (!m || m->windDir < 0) return 0.0;
    d  = deg2rad((double)m->windDir - rwyHdg);
    ws = (double)m->windSpeedKt;
    return ws * cos(d);
}
