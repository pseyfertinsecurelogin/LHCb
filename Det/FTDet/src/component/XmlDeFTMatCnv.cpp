#ifndef XMLDEFTMATCNV_H
#define XMLDEFTMATCNV_H 1

// DetDesc
#include "DetDescCnv/XmlUserDetElemCnv.h"
// FTDet
#include "FTDet/DeFTMat.h"
#include "GaudiKernel/Converter.h"

typedef XmlUserDetElemCnv<DeFTMat>       XmlDeFTMatCnv;
DECLARE_CONVERTER_FACTORY( XmlDeFTMatCnv )

#endif //XMLDEFTMATCNV_H

