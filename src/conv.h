#ifndef NAV1_CONV_H
#define NAV1_CONV_H

#include "nav1.h"


NAV1_EXPORT double NAV1_CONV_mToFt(double meters);
NAV1_EXPORT double NAV1_CONV_ftToM(double feet);
NAV1_EXPORT double NAV1_CONV_nmToM(double nm);
NAV1_EXPORT double NAV1_CONV_mToNm(double meters);
NAV1_EXPORT double NAV1_CONV_nmToKm(double nm);
NAV1_EXPORT double NAV1_CONV_kmToNm(double km);
NAV1_EXPORT double NAV1_CONV_nmToMi(double nm);
NAV1_EXPORT double NAV1_CONV_miToNm(double mi);
NAV1_EXPORT double NAV1_CONV_kmToMi(double km);
NAV1_EXPORT double NAV1_CONV_miToKm(double mi);


NAV1_EXPORT double NAV1_CONV_knotsToMs(double knots);
NAV1_EXPORT double NAV1_CONV_msToKnots(double ms);
NAV1_EXPORT double NAV1_CONV_knotsToKmh(double knots);
NAV1_EXPORT double NAV1_CONV_kmhToKnots(double kmh);


NAV1_EXPORT double NAV1_CONV_cToF(double celsius);
NAV1_EXPORT double NAV1_CONV_fToC(double fahrenheit);


NAV1_EXPORT double NAV1_CONV_hPaToInHg(double hPa);
NAV1_EXPORT double NAV1_CONV_inHgTohPa(double inHg);
NAV1_EXPORT double NAV1_CONV_hPaToPsi(double hPa);
NAV1_EXPORT double NAV1_CONV_psiTohPa(double psi);

#endif
