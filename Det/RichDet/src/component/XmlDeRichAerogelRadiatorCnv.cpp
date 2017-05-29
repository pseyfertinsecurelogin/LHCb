#ifndef XMLDERICHAEROGELRADIATORCNV_H 
#define XMLDERICHAEROGELRADIATORCNV_H 1

#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// Include files
#include "RichDet/DeRichAerogelRadiator.h"

typedef  XmlUserDetElemCnv<DeRichAerogelRadiator>  XmlDeRichAerogelRadiatorCnv;
DECLARE_CONVERTER_FACTORY( XmlDeRichAerogelRadiatorCnv )

#endif // XMLDERICHAEROGELRADIATORCNV_H
