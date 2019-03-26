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
#ifndef COMPONENT_L0MUONINPUTTOOL_H
#define COMPONENT_L0MUONINPUTTOOL_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "L0Interfaces/IL0MuonInputTool.h" // Interface

/** @class L0MuonInputTool L0MuonInputTool.h component/L0MuonInputTool.h
 *  Tool to retrieve the muon tiles in input of the L0Muon trigger
 *
 *  @author Julien Cogan
 *  @date   2010-02-25
 */
class L0MuonInputTool : public GaudiTool, virtual public IL0MuonInputTool {
public:
  /// Standard constructor
  L0MuonInputTool( const std::string& type, const std::string& name, const IInterface* parent );

  virtual ~L0MuonInputTool(); ///< Destructor
  StatusCode getTiles( std::vector<LHCb::MuonTileID>& tiles, std::string rootInTes = "" ) override;
  StatusCode getPads( std::vector<LHCb::MuonTileID>& pads, std::string rootInTes = "" ) override;

protected:
private:
  // Options
  std::string m_l0Context; ///< Suffix appended to data location in TES
};
#endif // COMPONENT_L0MUONINPUTTOOL_H
