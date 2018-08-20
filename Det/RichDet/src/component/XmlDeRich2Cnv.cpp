#ifndef XMLDERICH2CNV_H
#define XMLDERICH2CNV_H 1

// Include files
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "RichDet/DeRich2.h"

typedef XmlUserDetElemCnv< DeRich2 > XmlDeRich2Cnv;
DECLARE_CONVERTER( XmlDeRich2Cnv )

#endif // XMLDERICH2CNV_H
