#ifndef XMLDERICHPMTCNV_H
#define XMLDERICHPMTCNV_H 1

// Include files
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "RichDet/DeRichPMT.h"

typedef XmlUserDetElemCnv< DeRichPMT > XmlDeRichPMTCnv;
DECLARE_CONVERTER( XmlDeRichPMTCnv )

#endif // XMLDERICHPMTCNV_H
