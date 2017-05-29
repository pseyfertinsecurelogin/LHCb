// Include files
#include "DetDescCnv/XmlUserConditionCnv.h"
#include "GaudiKernel/Converter.h"
#include "VeloDet/VeloAlignCond.h"

typedef  XmlUserConditionCnv<VeloAlignCond>  XmlVeloAlignCondCnv;
DECLARE_CONVERTER_FACTORY(XmlVeloAlignCondCnv)
