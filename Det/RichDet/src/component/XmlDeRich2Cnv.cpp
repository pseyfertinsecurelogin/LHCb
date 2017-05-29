#ifndef XMLDERICH2CNV_H 
#define XMLDERICH2CNV_H 1

#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// Include files
#include "RichDet/DeRich2.h"

typedef  XmlUserDetElemCnv<DeRich2>  XmlDeRich2Cnv;
DECLARE_CONVERTER_FACTORY( XmlDeRich2Cnv )

#endif // XMLDERICH2CNV_H
