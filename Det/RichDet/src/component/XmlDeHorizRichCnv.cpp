#ifndef XMLDEHORIZRICHCNV_H 
#define XMLDEHORIZRICHCNV_H 1

#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// Include files
#include "RichDet/DeHorizRich.h"

typedef  XmlUserDetElemCnv<DeHorizRich>  XmlDeHorizRichCnv;
DECLARE_CONVERTER_FACTORY( XmlDeHorizRichCnv )

#endif // XMLDEHORIZRICHCNV_H
