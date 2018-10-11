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
#ifndef COMPONENT_L0MUONOLERRORTOOL_H
#define COMPONENT_L0MUONOLERRORTOOL_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "L0Interfaces/IL0MuonOLErrorTool.h"            // Interface

#include "MuonKernel/MuonSystemLayout.h"


/** @class L0MuonOLErrorTool L0MuonOLErrorTool.h component/L0MuonOLErrorTool.h
 *  Tool to access the list of optical link in error in input of the L0muon trigger
 *
 *  @author Julien Cogan
 *  @date   2010-02-25
 */
class L0MuonOLErrorTool : public extends<GaudiTool, IL0MuonOLErrorTool> {
public:
  /// Standard constructor
  L0MuonOLErrorTool( const std::string& type,
                     const std::string& name,
                     const IInterface* parent);

  StatusCode getTiles(std::vector<LHCb::MuonTileID> & ols, std::string rootInTes="") override;

private:
  // Options
  std::string m_l0Context; ///< Suffix appended to data location in TES
};
#endif // COMPONENT_L0MUONOLERRORTOOL_H
