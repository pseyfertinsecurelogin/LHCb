#ifndef XMLDERICH1CNV_H
#define XMLDERICH1CNV_H 1

// Include files
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "RichDet/DeRich1.h"

typedef XmlUserDetElemCnv< DeRich1 > XmlDeRich1Cnv;
DECLARE_CONVERTER( XmlDeRich1Cnv )

#endif // XMLDERICH1CNV_H
