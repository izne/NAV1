#include "conv.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define M_PER_FT  0.3048
#define M_PER_NM  1852.0
#define KM_PER_NM 1.852
#define MI_PER_NM 1.15078
#define KM_PER_MI 1.60934
#define MS_PER_KT 0.514444

NAV1_EXPORT double NAV1_CONV_mToFt(double meters)
{
    return meters / M_PER_FT;
}

NAV1_EXPORT double NAV1_CONV_ftToM(double feet)
{
    return feet * M_PER_FT;
}

NAV1_EXPORT double NAV1_CONV_nmToM(double nm)
{
    return nm * M_PER_NM;
}

NAV1_EXPORT double NAV1_CONV_mToNm(double meters)
{
    return meters / M_PER_NM;
}

NAV1_EXPORT double NAV1_CONV_nmToKm(double nm)
{
    return nm * KM_PER_NM;
}

NAV1_EXPORT double NAV1_CONV_kmToNm(double km)
{
    return km / KM_PER_NM;
}

NAV1_EXPORT double NAV1_CONV_nmToMi(double nm)
{
    return nm * MI_PER_NM;
}

NAV1_EXPORT double NAV1_CONV_miToNm(double mi)
{
    return mi / MI_PER_NM;
}

NAV1_EXPORT double NAV1_CONV_kmToMi(double km)
{
    return km / KM_PER_MI;
}

NAV1_EXPORT double NAV1_CONV_miToKm(double mi)
{
    return mi * KM_PER_MI;
}

NAV1_EXPORT double NAV1_CONV_knotsToMs(double knots)
{
    return knots * MS_PER_KT;
}

NAV1_EXPORT double NAV1_CONV_msToKnots(double ms)
{
    return ms / MS_PER_KT;
}

NAV1_EXPORT double NAV1_CONV_knotsToKmh(double knots)
{
    return knots * KM_PER_NM;
}

NAV1_EXPORT double NAV1_CONV_kmhToKnots(double kmh)
{
    return kmh / KM_PER_NM;
}

NAV1_EXPORT double NAV1_CONV_cToF(double celsius)
{
    return celsius * 9.0 / 5.0 + 32.0;
}

NAV1_EXPORT double NAV1_CONV_fToC(double fahrenheit)
{
    return (fahrenheit - 32.0) * 5.0 / 9.0;
}

NAV1_EXPORT double NAV1_CONV_hPaToInHg(double hPa)
{
    return hPa / 33.86389;
}

NAV1_EXPORT double NAV1_CONV_inHgTohPa(double inHg)
{
    return inHg * 33.86389;
}

NAV1_EXPORT double NAV1_CONV_hPaToPsi(double hPa)
{
    return hPa * 0.01450377;
}

NAV1_EXPORT double NAV1_CONV_psiTohPa(double psi)
{
    return psi / 0.01450377;
}

#define L_PER_GAL 3.78541
#define KG_PER_LB 0.453592
#define FT_PER_NM 6076.12

NAV1_EXPORT double NAV1_CONV_galToL(double gal)
{
    return gal * L_PER_GAL;
}

NAV1_EXPORT double NAV1_CONV_lToGal(double l)
{
    return l / L_PER_GAL;
}

NAV1_EXPORT double NAV1_CONV_lbToKg(double lb)
{
    return lb * KG_PER_LB;
}

NAV1_EXPORT double NAV1_CONV_kgToLb(double kg)
{
    return kg / KG_PER_LB;
}

NAV1_EXPORT double NAV1_CONV_nmToFt(double nm)
{
    return nm * FT_PER_NM;
}

NAV1_EXPORT double NAV1_CONV_ftToNm(double ft)
{
    return ft / FT_PER_NM;
}

NAV1_EXPORT double NAV1_CONV_degToRad(double deg)
{
    return deg * M_PI / 180.0;
}

NAV1_EXPORT double NAV1_CONV_radToDeg(double rad)
{
    return rad * 180.0 / M_PI;
}

#define CM_PER_INCH 2.54

NAV1_EXPORT double NAV1_CONV_cmToInch(double cm)
{
    return cm / CM_PER_INCH;
}

NAV1_EXPORT double NAV1_CONV_inchToCm(double inch)
{
    return inch * CM_PER_INCH;
}

NAV1_EXPORT double NAV1_CONV_kgToTonne(double kg)
{
    return kg / 1000.0;
}

NAV1_EXPORT double NAV1_CONV_tonneToKg(double tonne)
{
    return tonne * 1000.0;
}
