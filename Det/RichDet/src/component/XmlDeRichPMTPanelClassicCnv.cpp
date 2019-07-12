/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef XMLDERICHPMTPANELCLASSICCNV_CPP
#define XMLDERICHPMTPANELCLASSICCNV_CPP 1

// Include files
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "RichDet/DeRichPMTPanelClassic.h"

typedef XmlUserDetElemCnv<DeRichPMTPanelClassic> XmlDeRichPMTPanelClassicCnv;
DECLARE_CONVERTER( XmlDeRichPMTPanelClassicCnv )

#endif // XMLDERICHPMTPANELCLASSICCNV_CPP
