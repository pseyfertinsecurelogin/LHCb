#ifndef XMLDERICHSYSTEMCNV_H 
#define XMLDERICHSYSTEMCNV_H 1

// Include files
#include "RichDet/DeRichSystem.h"
#include "DetDescCnv/XmlUserDetElemCnv.h"

typedef  XmlUserDetElemCnv<DeRichSystem>  XmlDeRichSystemCnv;
DECLARE_CONVERTER( XmlDeRichSystemCnv )

#endif // XMLDERICHSYSTEMCNV_H
