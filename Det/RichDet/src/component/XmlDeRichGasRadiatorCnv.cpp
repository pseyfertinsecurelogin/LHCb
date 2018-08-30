#ifndef XMLDERICHGASRADIATORCNV_H
#define XMLDERICHGASRADIATORCNV_H 1

// Include files
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "RichDet/DeRichGasRadiator.h"

typedef XmlUserDetElemCnv< DeRichGasRadiator > XmlDeRichGasRadiatorCnv;
DECLARE_CONVERTER( XmlDeRichGasRadiatorCnv )

#endif // XMLDERICHGASRADIATORCNV_H
