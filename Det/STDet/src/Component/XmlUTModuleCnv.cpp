// DetDesc
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// UTDet
#include "STDet/DeUTModule.h"

typedef XmlUserDetElemCnv<DeUTModule>      XmlUTModuleCnv;
DECLARE_CONVERTER_FACTORY(XmlUTModuleCnv)
