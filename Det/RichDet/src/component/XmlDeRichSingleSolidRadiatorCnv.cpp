#ifndef XMLDERICHSINGLESOLIDRADIATORCNV_CPP
#define XMLDERICHSINGLESOLIDRADIATORCNV_CPP 1

#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// Include files
#include "RichDet/DeRichSingleSolidRadiator.h"

typedef  XmlUserDetElemCnv<DeRichSingleSolidRadiator>  
XmlDeRichSingleSolidRadiatorCnv;
DECLARE_CONVERTER_FACTORY( XmlDeRichSingleSolidRadiatorCnv )

#endif // XMLDERICHSINGLESOLIDRADIATORCNV_CPP
