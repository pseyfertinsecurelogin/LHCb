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
// $Id: XmlITSensorCnv.cpp,v 1.1 2008-03-14 18:26:40 mneedham Exp $

// DetDesc
#include "DetDescCnv/XmlUserDetElemCnv.h"

// ITDet
#include "STDet/DeITSensor.h"

typedef XmlUserDetElemCnv<DeITSensor >        XmlITSensorCnv;
DECLARE_CONVERTER( XmlITSensorCnv )
