#ifndef XMLDERICHBEAMPIPECNV_H
#define XMLDERICHBEAMPIPECNV_H 1

// Include files
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "RichDet/DeRichBeamPipe.h"

typedef XmlUserDetElemCnv< DeRichBeamPipe > XmlDeRichBeamPipeCnv;
DECLARE_CONVERTER( XmlDeRichBeamPipeCnv )

#endif // XMLDERICHBEAMPIPECNV_H
