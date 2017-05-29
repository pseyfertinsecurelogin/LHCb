// DetDesc
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// UTDet
#include "STDet/DeUTLayer.h"

typedef XmlUserDetElemCnv<DeUTLayer >        XmlUTLayerCnv;
DECLARE_CONVERTER_FACTORY(XmlUTLayerCnv)
