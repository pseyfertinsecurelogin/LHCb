#ifndef XMLDERICHHPDCNV_H
#define XMLDERICHHPDCNV_H 1

// Include files

#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "RichDet/DeRichHPD.h"

typedef XmlUserDetElemCnv< DeRichHPD > XmlDeRichHPDCnv;
DECLARE_CONVERTER( XmlDeRichHPDCnv )

#endif // XMLDERICHHPDCNV_H
