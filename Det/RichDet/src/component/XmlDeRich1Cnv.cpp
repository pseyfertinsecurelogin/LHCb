#ifndef XMLDERICH1CNV_H 
#define XMLDERICH1CNV_H 1

#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// Include files
#include "RichDet/DeRich1.h"

typedef  XmlUserDetElemCnv<DeRich1>  XmlDeRich1Cnv;
DECLARE_CONVERTER_FACTORY( XmlDeRich1Cnv )

#endif // XMLDERICH1CNV_H
