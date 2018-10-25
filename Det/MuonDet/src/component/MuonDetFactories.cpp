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
// Factories for converters for classes in MuonDetLib
#include "DetDescCnv/XmlUserConditionCnv.h"
#include "DetDescCnv/XmlUserDetElemCnv.h"

#include "MuonDet/MuonChamberGrid.h"
typedef XmlUserConditionCnv<MuonChamberGrid> XmlMuonGridCnv;

#include "MuonDet/DeMuonChamber.h"
typedef XmlUserDetElemCnv<DeMuonChamber> XmlDeMuonChamber;


#include "MuonDet/DeMuonRegion.h"
typedef XmlUserDetElemCnv<DeMuonRegion> XmlDeMuonRegion;

#include "MuonDet/DeMuonDetector.h"
typedef XmlUserDetElemCnv<DeMuonDetector> XmlDeMuonDetector;

DECLARE_CONVERTER( XmlMuonGridCnv )
DECLARE_CONVERTER( XmlDeMuonChamber )
DECLARE_CONVERTER( XmlDeMuonRegion )
DECLARE_CONVERTER( XmlDeMuonDetector )
