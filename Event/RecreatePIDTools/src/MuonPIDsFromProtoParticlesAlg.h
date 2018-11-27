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

//-----------------------------------------------------------------------------
/** @file MuonPIDsFromProtoParticlesAlg.h
 *
 * Header file for algorithm MuonPIDsFromProtoParticlesAlg
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date 29/03/2006
 */
//-----------------------------------------------------------------------------

#ifndef GLOBALRECO_MuonPIDsFromProtoParticlesAlg_H
#define GLOBALRECO_MuonPIDsFromProtoParticlesAlg_H 1

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

// Event
#include "Event/ProtoParticle.h"
#include "Event/MuonPID.h"

//-----------------------------------------------------------------------------
/** @class MuonPIDsFromProtoParticlesAlg MuonPIDsFromProtoParticlesAlg.h
 *
 *  Simple algorithm to recreate MuonPID data objects from the data
 *  stored in the ProtoParticles
 *
 *  @author Chris Jones
 *  @date   2006-08-01
 */
//-----------------------------------------------------------------------------

class MuonPIDsFromProtoParticlesAlg : public GaudiAlgorithm
{

public:

  /// Standard constructor
  MuonPIDsFromProtoParticlesAlg( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~MuonPIDsFromProtoParticlesAlg( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  /// Input location of ProtoParticles in TES
  std::string m_protoPloc;

  /// Output location of RichPIDs in TES
  std::string m_muonPIDloc;

  /// Location of Muon Tracks in TES
  std::string m_muonTrackLoc;

};

#endif // GLOBALRECO_MuonPIDsFromProtoParticlesAlg_H
