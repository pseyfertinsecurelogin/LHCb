#ifndef XMLDERICHPMTPANELCNV_CPP
#define XMLDERICHPMTPANELCNV_CPP 1

#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "GaudiKernel/Converter.h"
// Include files
#include "RichDet/DeRichPMTPanel.h"

typedef  XmlUserDetElemCnv<DeRichPMTPanel>  XmlDeRichPMTPanelCnv;
DECLARE_CONVERTER_FACTORY( XmlDeRichPMTPanelCnv )

#endif // XMLDERICHPMTPANELCNV_CPP
