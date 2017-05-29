#ifndef XMLDERICHSPHMIRRORCNV_H 
#define XMLDERICHSPHMIRRORCNV_H 1

#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// Include files
#include "RichDet/DeRichSphMirror.h"

typedef  XmlUserDetElemCnv<DeRichSphMirror>  XmlDeRichSphMirrorCnv;
DECLARE_CONVERTER_FACTORY( XmlDeRichSphMirrorCnv )

#endif // XMLDERICHSPHMIRRORCNV_H
