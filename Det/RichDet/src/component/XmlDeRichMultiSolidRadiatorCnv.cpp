#ifndef XMLDERICHMULTISOLIDRADIATORCNV_CPP
#define XMLDERICHMULTISOLIDRADIATORCNV_CPP 1

#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// Include files
#include "RichDet/DeRichMultiSolidRadiator.h"

typedef  XmlUserDetElemCnv<DeRichMultiSolidRadiator>  
XmlDeRichMultiSolidRadiatorCnv;
DECLARE_CONVERTER_FACTORY( XmlDeRichMultiSolidRadiatorCnv )

#endif // XMLDERICHMULTISOLIDRADIATORCNV_CPP
