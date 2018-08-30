#ifndef XMLDEHORIZRICHCNV_H
#define XMLDEHORIZRICHCNV_H 1

// Include files
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "RichDet/DeHorizRich.h"

typedef XmlUserDetElemCnv< DeHorizRich > XmlDeHorizRichCnv;
DECLARE_CONVERTER( XmlDeHorizRichCnv )

#endif // XMLDEHORIZRICHCNV_H
