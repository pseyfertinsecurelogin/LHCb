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
// DetDesc
#include "DetDescCnv/XmlUserDetElemCnv.h"

// UTDet
#include "UTDet/DeUTLayer.h"

typedef XmlUserDetElemCnv<DeUTLayer >        XmlUTLayerCnv;
DECLARE_CONVERTER( XmlUTLayerCnv )
