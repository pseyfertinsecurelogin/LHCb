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
/** @file ChargedProtoParticleAddMuonInfo.h
 *
 * Header file for algorithm ChargedProtoParticleAddMuonInfo
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date 29/03/2006
 */
//-----------------------------------------------------------------------------

#ifndef GLOBALRECO_ChargedProtoParticleAddMuonInfo_H
#define GLOBALRECO_ChargedProtoParticleAddMuonInfo_H 1

// STL
#include <sstream>

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

// Muon
#include "Event/MuonPID.h"
#include "Event/ProtoParticle.h"

/** @class ChargedProtoParticleAddMuonInfo ChargedProtoParticleAddMuonInfo.h
 *
 *  Updates the MuonPID information stored in the ProtoParticles
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date 28/08/2009
 */

class ChargedProtoParticleAddMuonInfo : public GaudiAlgorithm {

public:
  using GaudiAlgorithm::GaudiAlgorithm;
  /// mapping type from Track to MuonPID data objects
  using TrackToMuonPID = std::map<const LHCb::Track*, const LHCb::MuonPID*>;

  StatusCode execute() override; ///< Algorithm execution

private:
  /// Load the muon data
  TrackToMuonPID getMuonData() const;

  /// Add (or update) the Muon information for the given ProtoParticle
  void updateMuon( LHCb::ProtoParticle* proto, TrackToMuonPID const& muonMap ) const;

private:
  DataObjectReadHandle<LHCb::MuonPIDs> m_muonPath{
      this, "InputMuonPIDLocation", LHCb::MuonPIDLocation::Offline}; ///< Location of MuonPID data objects in the TES
  DataObjectReadHandle<LHCb::ProtoParticles> m_protos{
      this, "ProtoParticleLocation",
      LHCb::ProtoParticleLocation::Charged}; ///< Location of the ProtoParticles in the TES
};

#endif // GLOBALRECO_ChargedProtoParticleAddMuonInfo_H
