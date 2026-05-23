#include "arinc429.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    unsigned int label;
    const char *name;
    const char *unit;
    int encoding;
    double lsbWeight;
    int isSigned;
    int bcdDigits;
} LabelEntry;

static const LabelEntry labelTable[] = {
    {0010, "Baro Altitude",        "ft",     NAV1_A429_ENCODING_BNR,  1.0,                1, 0},
    {0011, "Radio Altitude",       "ft",     NAV1_A429_ENCODING_BNR,  1.0,                0, 0},
    {0016, "Indicated Airspeed",   "kt",     NAV1_A429_ENCODING_BNR,  0.125,              0, 0},
    {0017, "Mach",                 "Mach",   NAV1_A429_ENCODING_BNR,  0.001,               0, 0},
    {0022, "True Heading",         "deg",    NAV1_A429_ENCODING_BNR,  360.0 / 262144.0,    0, 0},
    {0023, "Mag Heading",          "deg",    NAV1_A429_ENCODING_BNR,  360.0 / 262144.0,    0, 0},
    {0026, "True Track",           "deg",    NAV1_A429_ENCODING_BNR,  360.0 / 262144.0,    0, 0},
    {0027, "Mag Track",            "deg",    NAV1_A429_ENCODING_BNR,  360.0 / 262144.0,    0, 0},
    {0030, "Ground Speed",         "kt",     NAV1_A429_ENCODING_BNR,  0.125,               0, 0},
    {0031, "True Airspeed",        "kt",     NAV1_A429_ENCODING_BNR,  0.125,               0, 0},
    {0032, "Vertical Speed",       "ft/min", NAV1_A429_ENCODING_BNR,  0.5,                 1, 0},
    {0033, "Wind Speed",           "kt",     NAV1_A429_ENCODING_BNR,  0.125,               0, 0},
    {0034, "Wind Direction",       "deg",    NAV1_A429_ENCODING_BNR,  360.0 / 262144.0,    0, 0},
    {0035, "Static Air Temp",      "degC",   NAV1_A429_ENCODING_BNR,  0.0078125,           0, 0},
    {0036, "Total Air Temp",       "degC",   NAV1_A429_ENCODING_BNR,  0.0078125,           0, 0},
    {0040, "Latitude",             "deg",    NAV1_A429_ENCODING_BNR,  180.0 / 262144.0,    1, 0},
    {0041, "Longitude",            "deg",    NAV1_A429_ENCODING_BNR,  180.0 / 262144.0,    1, 0},
    {0042, "UTC Time",             "sec",    NAV1_A429_ENCODING_BCD,  0.0,                 0, 6},
    {0050, "DME Distance",         "NM",     NAV1_A429_ENCODING_BNR,  0.03125,             0, 0},
    {0051, "Bearing to WPT",       "deg",    NAV1_A429_ENCODING_BNR,  360.0 / 262144.0,    0, 0},
    {0060, "Loc Deviation",        "DDM",    NAV1_A429_ENCODING_BNR,  0.000061,            1, 0},
    {0061, "GS Deviation",         "DDM",    NAV1_A429_ENCODING_BNR,  0.000061,            1, 0},
    {0100, "Selected Altitude",    "ft",     NAV1_A429_ENCODING_BNR,  1.0,                 0, 0},
    {0101, "Selected Heading",     "deg",    NAV1_A429_ENCODING_BNR,  360.0 / 262144.0,    0, 0},
    {0102, "Selected Airspeed",    "kt",     NAV1_A429_ENCODING_BNR,  0.125,               0, 0},
    {0110, "ILS Frequency",        "MHz",    NAV1_A429_ENCODING_BCD,  0.0,                 0, 5},
    {0111, "VOR Frequency",        "MHz",    NAV1_A429_ENCODING_BCD,  0.0,                 0, 5},
    {0112, "ADF Frequency",        "kHz",    NAV1_A429_ENCODING_BCD,  0.0,                 0, 5},
    {0113, "DME Frequency",        "MHz",    NAV1_A429_ENCODING_BCD,  0.0,                 0, 5},
    {0120, "DME Distance",         "NM",     NAV1_A429_ENCODING_BNR,  0.03125,             0, 0},
    {0200, "FMS ID",               "",       NAV1_A429_ENCODING_BCD,  0.0,                 0, 8},
    {0210, "Time to Go",           "min",    NAV1_A429_ENCODING_BNR,  0.0078125,           0, 0},
    {0211, "Distance to Go",       "NM",     NAV1_A429_ENCODING_BNR,  0.03125,             0, 0},
    {0212, "Fuel Flow",            "lb/hr",  NAV1_A429_ENCODING_BNR,  0.125,               0, 0},
    {0220, "Roll Command",         "deg",    NAV1_A429_ENCODING_BNR,  360.0 / 262144.0,    0, 0},
    {0221, "Pitch Command",        "deg",    NAV1_A429_ENCODING_BNR,  180.0 / 262144.0,    1, 0},
    {0270, "Data Loader Status",   "",       NAV1_A429_ENCODING_BCD,  0.0,                 0, 4},
    {0340, "FMS Waypoint Lat",     "deg",    NAV1_A429_ENCODING_BNR,  180.0 / 524288.0,    1, 0},
    {0341, "FMS Waypoint Lon",     "deg",    NAV1_A429_ENCODING_BNR,  180.0 / 524288.0,    1, 0},
    {0350, "FMS Leg Distance",     "NM",     NAV1_A429_ENCODING_BNR,  0.03125,             0, 0},
    {0351, "FMS Leg Bearing",      "deg",    NAV1_A429_ENCODING_BNR,  360.0 / 262144.0,    0, 0},
    {0360, "FMS Cross Track",      "NM",     NAV1_A429_ENCODING_BNR,  0.00390625,          0, 0},
};
static const int labelCount = sizeof(labelTable) / sizeof(labelTable[0]);

static int bcdValue(unsigned int dataField, int digitCount)
{
    int i, val = 0;
    for (i = 0; i < digitCount; i++)
    {
        val = val * 10 + (int)((dataField >> (4 * (digitCount - 1 - i))) & 0xF);
    }
    return val;
}

static int octToUint(const char *s)
{
    unsigned int v = 0;
    while (*s >= '0' && *s <= '7')
        v = v * 8 + (*s++ - '0');
    return (int)v;
}

void NAV1_A429_a429Decode(unsigned int word, NAV1_ARINC429Word *out)
{
    unsigned int lab;
    int i;
    double val = 0.0;
    int encoding = NAV1_A429_ENCODING_UNKN;
    const char *name = NULL;
    const char *unit = "";
    if (!out)
        return;
    lab = word & 0xFF;
    out->label = lab;
    out->sdi = (word >> 8) & 0x3;
    out->dataField = (word >> 10) & 0x7FFFF;
    out->ssm = (word >> 29) & 0x3;
    out->parityOK = NAV1_A429_a429ParityCheck(word);
    for (i = 0; i < labelCount; i++)
    {
        if (labelTable[i].label == lab)
        {
            name = labelTable[i].name;
            unit = labelTable[i].unit;
            encoding = labelTable[i].encoding;
            if (encoding == NAV1_A429_ENCODING_BNR)
            {
                int signExt = 0;
                unsigned int d = out->dataField;
                if (labelTable[i].isSigned && (d & 0x40000))
                    signExt = (int)(d | 0xFFF80000);
                else
                    signExt = (int)(d & 0x7FFFF);
                val = (double)signExt * labelTable[i].lsbWeight;
            }
            else if (encoding == NAV1_A429_ENCODING_BCD)
            {
                val = (double)bcdValue(out->dataField, labelTable[i].bcdDigits);
            }
            else if (encoding == NAV1_A429_ENCODING_DISC)
            {
                val = (double)out->dataField;
            }
            break;
        }
    }
    out->encoding = encoding;
    out->value = val;
    if (name)
        strncpy(out->name, name, sizeof(out->name) - 1);
    else
        sprintf(out->name, "LABEL-%03o", lab);
    out->name[sizeof(out->name) - 1] = '\0';
    strncpy(out->unit, unit, sizeof(out->unit) - 1);
    out->unit[sizeof(out->unit) - 1] = '\0';
}

unsigned int NAV1_A429_a429Label(unsigned int word)
{
    return word & 0xFF;
}

unsigned int NAV1_A429_a429SDI(unsigned int word)
{
    return (word >> 8) & 0x3;
}

unsigned int NAV1_A429_a429Data(unsigned int word)
{
    return (word >> 10) & 0x7FFFF;
}

unsigned int NAV1_A429_a429SSM(unsigned int word)
{
    return (word >> 29) & 0x3;
}

int NAV1_A429_a429ParityCheck(unsigned int word)
{
    unsigned int v = word;
    int bits = 0;
    while (v)
    {
        bits++;
        v &= v - 1;
    }
    return (bits & 1) ? 1 : 0;
}

double NAV1_A429_a429DataBNR(unsigned int dataField, double lsbWeight)
{
    return (double)dataField * lsbWeight;
}

double NAV1_A429_a429DataBCD(unsigned int dataField, int digitCount)
{
    return (double)bcdValue(dataField, digitCount);
}

int NAV1_A429_a429LabelInfo(unsigned int label, char *outName, char *outUnit, int *outEncoding, double *outLsbWeight, int *outIsSigned)
{
    int i;
    for (i = 0; i < labelCount; i++)
    {
        if (labelTable[i].label == label)
        {
            if (outName)
                strncpy(outName, labelTable[i].name, 39);
            if (outUnit)
                strncpy(outUnit, labelTable[i].unit, 11);
            if (outEncoding)
                *outEncoding = labelTable[i].encoding;
            if (outLsbWeight)
                *outLsbWeight = labelTable[i].lsbWeight;
            if (outIsSigned)
                *outIsSigned = labelTable[i].isSigned;
            return 1;
        }
    }
    return 0;
}

unsigned int NAV1_A429_a429Encode(unsigned int label, unsigned int sdi, unsigned int dataField, unsigned int ssm)
{
    return (label & 0xFF) | ((sdi & 0x3) << 8)
         | ((dataField & 0x7FFFF) << 10) | ((ssm & 0x3) << 29);
}

unsigned int NAV1_A429_a429SetParity(unsigned int word)
{
    unsigned int v = word;
    int bits = 0;
    while (v)
    {
        bits++;
        v &= v - 1;
    }
    if (bits & 1)
        return word & 0x7FFFFFFF;
    return word | 0x80000000;
}

unsigned int NAV1_A429_a429EncodeBNR(unsigned int label, unsigned int sdi, double value, double lsbWeight, int isSigned, unsigned int ssm)
{
    int raw = (int)(value / lsbWeight + (value >= 0 ? 0.5 : -0.5));
    unsigned int dataField;
    if (isSigned && raw < 0)
        dataField = ((unsigned int)(-raw) & 0x7FFFF) | 0x40000;
    else
        dataField = (unsigned int)(raw & 0x7FFFF);
    return NAV1_A429_a429SetParity(NAV1_A429_a429Encode(label, sdi, dataField, ssm));
}

unsigned int NAV1_A429_a429EncodeBCD(unsigned int label, unsigned int sdi, double value, int digitCount, unsigned int ssm)
{
    int i, ival = (int)(value + 0.5);
    unsigned int dataField = 0;
    for (i = 0; i < digitCount; i++)
    {
        dataField |= (unsigned int)(ival % 10) << (4 * i);
        ival /= 10;
    }
    return NAV1_A429_a429SetParity(NAV1_A429_a429Encode(label, sdi, dataField, ssm));
}
