#ifndef XMLDERICHHPDCNV_H 
#define XMLDERICHHPDCNV_H 1

// Include files

#include "RichDet/DeRichHPD.h"
#include "DetDescCnv/XmlUserDetElemCnv.h"

typedef  XmlUserDetElemCnv<DeRichHPD>  XmlDeRichHPDCnv;
DECLARE_CONVERTER( XmlDeRichHPDCnv )

#endif // XMLDERICHHPDCNV_H
