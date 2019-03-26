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
#ifndef COMPONENT_L0MUONMODIFYINPUTTOOL_H
#define COMPONENT_L0MUONMODIFYINPUTTOOL_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/RndmGenerators.h"
#include "L0Interfaces/IL0MuonModifyInputTool.h" // Interface

/** @class L0MuonModifyInputTool L0MuonModifyInputTool.h component/L0MuonModifyInputTool.h
 *
 *
 *  @author Julien Cogan
 *  @date   2013-01-21
 */
class L0MuonModifyInputTool : public GaudiTool, virtual public IL0MuonModifyInputTool {
public:
  /// Standard constructor
  L0MuonModifyInputTool( const std::string& type, const std::string& name, const IInterface* parent );

  virtual ~L0MuonModifyInputTool(); ///< Destructor

  StatusCode modifyInput( std::vector<LHCb::MuonTileID>& digits ) override;
  StatusCode initialize() override;
  StatusCode finalize() override { return StatusCode::SUCCESS; }

private:
  IRndmGenSvc* m_randSvc;
  // double m_params[5][4];
  std::vector<double> m_params;
};
#endif // COMPONENT_L0MUONMODIFYINPUTTOOL_H
