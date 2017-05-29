#ifndef XMLDERICHBEAMPIPECNV_H 
#define XMLDERICHBEAMPIPECNV_H 1

#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// Include files
#include "RichDet/DeRichBeamPipe.h"

typedef  XmlUserDetElemCnv<DeRichBeamPipe>  XmlDeRichBeamPipeCnv;
DECLARE_CONVERTER_FACTORY( XmlDeRichBeamPipeCnv )

#endif // XMLDERICHBEAMPIPECNV_H
