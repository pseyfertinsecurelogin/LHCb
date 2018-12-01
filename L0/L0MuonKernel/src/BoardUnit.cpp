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
#include "L0MuonKernel/BoardUnit.h"
#include "L0MuonKernel/CrateUnit.h"

L0Muon::BoardUnit::BoardUnit(){

}

L0Muon::BoardUnit::BoardUnit(LHCb::MuonTileID id):L0MUnit(id){
  
}

L0Muon::BoardUnit::BoardUnit(DOMNode* pNode):L0MUnit(pNode){
  
}



L0Muon::BoardUnit::~BoardUnit() {}
