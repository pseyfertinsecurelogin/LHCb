#ifndef XMLDERICHPMTCNV_H 
#define XMLDERICHPMTCNV_H 1

#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// Include files
#include "RichDet/DeRichPMT.h"

typedef  XmlUserDetElemCnv<DeRichPMT>  XmlDeRichPMTCnv;
DECLARE_CONVERTER_FACTORY( XmlDeRichPMTCnv )

#endif // XMLDERICHPMTCNV_H
