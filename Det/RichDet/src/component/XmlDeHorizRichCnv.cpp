#ifndef XMLDEHORIZRICHCNV_H 
#define XMLDEHORIZRICHCNV_H 1

// Include files
/// GaudiKernel
#include "GaudiKernel/CnvFactory.h"

#include "RichDet/DeHorizRich.h"
#include "DetDescCnv/XmlUserDetElemCnv.h"

typedef  XmlUserDetElemCnv<DeHorizRich>  XmlDeHorizRichCnv;
DECLARE_CONVERTER_FACTORY( XmlDeHorizRichCnv )

#endif // XMLDEHORIZRICHCNV_H
