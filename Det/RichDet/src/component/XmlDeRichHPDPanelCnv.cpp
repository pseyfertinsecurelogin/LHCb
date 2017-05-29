#ifndef XMLDERICHHPDPANELCNV_CPP
#define XMLDERICHHPDPANELCNV_CPP 1

#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// Include files
#include "RichDet/DeRichHPDPanel.h"

typedef  XmlUserDetElemCnv<DeRichHPDPanel>  XmlDeRichHPDPanelCnv;
DECLARE_CONVERTER_FACTORY( XmlDeRichHPDPanelCnv )

#endif // XMLDERICHHPDPANELCNV_CPP
