#ifndef XMLDEFTQUARTERCNV_H
#define XMLDEFTQUARTERCNV_H 1

// DetDesc
#include "DetDescCnv/XmlUserDetElemCnv.h"
// FTDet
#include "FTDet/DeFTQuarter.h"

typedef XmlUserDetElemCnv<DeFTQuarter>       XmlDeFTQuarterCnv;
DECLARE_CONVERTER( XmlDeFTQuarterCnv )

#endif //XMLDEFTQUARTERCNV_H
