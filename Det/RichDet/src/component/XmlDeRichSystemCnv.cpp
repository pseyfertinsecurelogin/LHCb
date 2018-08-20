#ifndef XMLDERICHSYSTEMCNV_H
#define XMLDERICHSYSTEMCNV_H 1

// Include files
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "RichDet/DeRichSystem.h"

typedef XmlUserDetElemCnv< DeRichSystem > XmlDeRichSystemCnv;
DECLARE_CONVERTER( XmlDeRichSystemCnv )

#endif // XMLDERICHSYSTEMCNV_H
