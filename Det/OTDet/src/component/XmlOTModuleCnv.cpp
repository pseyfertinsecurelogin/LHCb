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
// $Id: XmlOTModuleCnv.cpp,v 1.3 2006-12-14 13:29:51 ranjard Exp $

/// DetDesc
#include "DetDescCnv/XmlUserDetElemCnv.h"
/// OTDet
#include "OTDet/DeOTModule.h"

typedef XmlUserDetElemCnv<DeOTModule> XmlOTModuleCnv;
DECLARE_CONVERTER( XmlOTModuleCnv )
