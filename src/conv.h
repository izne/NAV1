#ifndef NAV1_CONV_H
#define NAV1_CONV_H

#include "nav1.h"


NAV_EXPORT double NAV1_CONV_mToFt(double meters);
NAV_EXPORT double NAV1_CONV_ftToM(double feet);
NAV_EXPORT double NAV1_CONV_nmToM(double nm);
NAV_EXPORT double NAV1_CONV_mToNm(double meters);
NAV_EXPORT double NAV1_CONV_nmToKm(double nm);
NAV_EXPORT double NAV1_CONV_kmToNm(double km);
NAV_EXPORT double NAV1_CONV_nmToMi(double nm);
NAV_EXPORT double NAV1_CONV_miToNm(double mi);
NAV_EXPORT double NAV1_CONV_kmToMi(double km);
NAV_EXPORT double NAV1_CONV_miToKm(double mi);


NAV_EXPORT double NAV1_CONV_knotsToMs(double knots);
NAV_EXPORT double NAV1_CONV_msToKnots(double ms);
NAV_EXPORT double NAV1_CONV_knotsToKmh(double knots);
NAV_EXPORT double NAV1_CONV_kmhToKnots(double kmh);


NAV_EXPORT double NAV1_CONV_cToF(double celsius);
NAV_EXPORT double NAV1_CONV_fToC(double fahrenheit);


NAV_EXPORT double NAV1_CONV_hPaToInHg(double hPa);
NAV_EXPORT double NAV1_CONV_inHgTohPa(double inHg);
NAV_EXPORT double NAV1_CONV_hPaToPsi(double hPa);
NAV_EXPORT double NAV1_CONV_psiTohPa(double psi);

#endif
