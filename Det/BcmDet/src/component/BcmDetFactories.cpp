#include "BcmDet/DeBcmSens.h"
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"

typedef XmlUserDetElemCnv<DeBcmSens> XmlDeBcmSens;

#include "BcmDet/DeBcm.h"

typedef XmlUserDetElemCnv<DeBcm> XmlDeBcm;

DECLARE_CONVERTER_FACTORY(XmlDeBcmSens)
DECLARE_CONVERTER_FACTORY(XmlDeBcm)

