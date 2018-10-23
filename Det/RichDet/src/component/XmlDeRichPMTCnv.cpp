/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef XMLDERICHPMTCNV_H
#define XMLDERICHPMTCNV_H 1

// Include files
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "RichDet/DeRichPMT.h"

typedef XmlUserDetElemCnv< DeRichPMT > XmlDeRichPMTCnv;
DECLARE_CONVERTER( XmlDeRichPMTCnv )

#endif // XMLDERICHPMTCNV_H
