#ifndef XMLDEFTQUARTERCNV_H
#define XMLDEFTQUARTERCNV_H 1

// DetDesc
#include "DetDescCnv/XmlUserDetElemCnv.h"
// FTDet
#include "FTDet/DeFTQuarter.h"
#include "GaudiKernel/Converter.h"

typedef XmlUserDetElemCnv<DeFTQuarter>       XmlDeFTQuarterCnv;
DECLARE_CONVERTER_FACTORY( XmlDeFTQuarterCnv )

#endif //XMLDEFTQUARTERCNV_H
