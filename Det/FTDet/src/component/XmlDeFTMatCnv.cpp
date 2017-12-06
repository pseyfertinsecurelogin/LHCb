#ifndef XMLDEFTMATCNV_H
#define XMLDEFTMATCNV_H 1

// DetDesc
#include "DetDescCnv/XmlUserDetElemCnv.h"
// FTDet
#include "FTDet/DeFTMat.h"

typedef XmlUserDetElemCnv<DeFTMat>       XmlDeFTMatCnv;
DECLARE_CONVERTER( XmlDeFTMatCnv )

#endif //XMLDEFTMATCNV_H

