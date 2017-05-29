#ifndef XMLDERICHSYSTEMCNV_H 
#define XMLDERICHSYSTEMCNV_H 1

#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// Include files
#include "RichDet/DeRichSystem.h"

typedef  XmlUserDetElemCnv<DeRichSystem>  XmlDeRichSystemCnv;
DECLARE_CONVERTER_FACTORY( XmlDeRichSystemCnv )

#endif // XMLDERICHSYSTEMCNV_H
