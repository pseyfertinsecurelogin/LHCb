#ifndef XMLDERICHGASRADIATORCNV_H 
#define XMLDERICHGASRADIATORCNV_H 1

#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// Include files
#include "RichDet/DeRichGasRadiator.h"

typedef  XmlUserDetElemCnv<DeRichGasRadiator>  XmlDeRichGasRadiatorCnv;
DECLARE_CONVERTER_FACTORY( XmlDeRichGasRadiatorCnv )

#endif // XMLDERICHGASRADIATORCNV_H
