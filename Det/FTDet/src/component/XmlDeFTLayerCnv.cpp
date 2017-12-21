#ifndef XMLDEFTLAYERCNV_H
#define XMLDEFTLAYERCNV_H 1

// DetDesc
#include "DetDescCnv/XmlUserDetElemCnv.h"
// FTDet
#include "FTDet/DeFTLayer.h"

typedef XmlUserDetElemCnv<DeFTLayer>       XmlDeFTLayerCnv;
DECLARE_CONVERTER( XmlDeFTLayerCnv )

#endif //XMLDEFTLAYERCNV_H
