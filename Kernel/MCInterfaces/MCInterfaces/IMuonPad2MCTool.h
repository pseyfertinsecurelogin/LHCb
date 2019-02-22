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
#ifndef IMUONPAD2MCTOOL_H
#define IMUONPAD2MCTOOL_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IAlgTool.h"
#include "Kernel/MuonTileID.h"

namespace LHCb {
  class MCParticle;
}

/** @class IMuonPad2MCTool IMuonPad2MCTool.h
 *
 *
 *  @author Alessia Satta
 *  @date   2006-12-06
 */
struct IMuonPad2MCTool : extend_interfaces<IAlgTool> {

  DeclareInterfaceID( IMuonPad2MCTool, 2, 0 );

  virtual LHCb::MCParticle* Pad2MC( LHCb::MuonTileID value ) const                         = 0;
  virtual bool              isXTalk( LHCb::MuonTileID value, LHCb::MCParticle*& pp ) const = 0;
  virtual LHCb::MCParticle* PadNoXtalk2MC( LHCb::MuonTileID value ) const                  = 0;
};
#endif // IMUONPAD2MCTOOL_H
