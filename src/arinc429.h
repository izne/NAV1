#ifndef NAV1_ARINC429_H
#define NAV1_ARINC429_H

#include "nav1.h"

#define NAV_A429_ENCODING_BNR   0
#define NAV_A429_ENCODING_BCD   1
#define NAV_A429_ENCODING_DISC  2
#define NAV_A429_ENCODING_UNKN  3

#define NAV_A429_SSM_NORMAL     3
#define NAV_A429_SSM_TEST       2
#define NAV_A429_SSM_NODATA     1
#define NAV_A429_SSM_FAIL       0

typedef struct {
    unsigned int label;
    unsigned int sdi;
    unsigned int ssm;
    unsigned int dataField;
    int  parityOK;
    int  encoding;
    double value;
    char name[40];
    char unit[12];
} NAV_Sup_ARINC429Word;

NAV_EXPORT void        NAV_SUP_a429Decode(unsigned int word, NAV_Sup_ARINC429Word *out);
NAV_EXPORT unsigned int NAV_SUP_a429Label(unsigned int word);
NAV_EXPORT unsigned int NAV_SUP_a429SDI(unsigned int word);
NAV_EXPORT unsigned int NAV_SUP_a429Data(unsigned int word);
NAV_EXPORT unsigned int NAV_SUP_a429SSM(unsigned int word);
NAV_EXPORT int         NAV_SUP_a429ParityCheck(unsigned int word);
NAV_EXPORT double      NAV_SUP_a429DataBNR(unsigned int dataField, double lsbWeight);
NAV_EXPORT double      NAV_SUP_a429DataBCD(unsigned int dataField, int digitCount);
NAV_EXPORT int         NAV_SUP_a429LabelInfo(unsigned int label, char *outName, char *outUnit, int *outEncoding, double *outLsbWeight, int *outIsSigned);

#endif
