// DetDesc
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// UTDet
#include "STDet/DeUTSector.h"

typedef XmlUserDetElemCnv<DeUTSector >        XmlUTSectorCnv;
DECLARE_CONVERTER_FACTORY(XmlUTSectorCnv)
