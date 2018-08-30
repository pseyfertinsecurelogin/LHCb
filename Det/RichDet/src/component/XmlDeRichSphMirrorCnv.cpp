#ifndef XMLDERICHSPHMIRRORCNV_H
#define XMLDERICHSPHMIRRORCNV_H 1

// Include files
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "RichDet/DeRichSphMirror.h"

typedef XmlUserDetElemCnv< DeRichSphMirror > XmlDeRichSphMirrorCnv;
DECLARE_CONVERTER( XmlDeRichSphMirrorCnv )

#endif // XMLDERICHSPHMIRRORCNV_H
