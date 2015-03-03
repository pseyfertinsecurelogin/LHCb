#ifndef XMLDERICHPMTCNV_H 
#define XMLDERICHPMTCNV_H 1

// Include files

// GaudiKernel
#include "GaudiKernel/CnvFactory.h"

#include "RichDet/DeRichPMT.h"
#include "DetDescCnv/XmlUserDetElemCnv.h"

typedef  XmlUserDetElemCnv<DeRichPMT>  XmlDeRichPMTCnv;
DECLARE_CONVERTER_FACTORY( XmlDeRichPMTCnv )

#endif // XMLDERICHPMTCNV_H
