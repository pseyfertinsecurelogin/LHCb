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
/** @file MCReconstructible.h
 *
 *  Header file for class : MCReconstructible
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date 28/02/2007
 */
//-----------------------------------------------------------------------------

#ifndef SIMCOMPONENTS_MCReconstructible_H
#define SIMCOMPONENTS_MCReconstructible_H 1

// from STL
#include <array>
#include <optional>
#include <string>
#include <vector>

// Gaudi
#include "GaudiKernel/DataObjectHandle.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/VectorMap.h"

// base class
#include "GaudiAlg/GaudiTool.h"

// Event
#include "Event/MCTrackGeomCriteria.h"
#include "Event/MCTrackInfo.h"

// Interface
#include "MCInterfaces/IMCParticleSelector.h"
#include "MCInterfaces/IMCReconstructible.h"

//-----------------------------------------------------------------------------
/** @class MCReconstructible MCReconstructible.h
 *
 *  Tool to provide standardised Reconstructibility information for neutral
 *  and charged MCParticles
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2004-04-28
 */
//-----------------------------------------------------------------------------
class MCReconstructible : public extends<GaudiTool, IMCReconstructible> {

public:
  /// Standard constructor
  using base_class::base_class;

  /// Initialize
  StatusCode initialize() override;

  /// Get the reconstruction category for the given MCParticle
  IMCReconstructible::RecCategory reconstructible( const LHCb::MCParticle* mcPart ) const override;

  /// Is the MCParticle in the detector acceptance?
  bool inAcceptance( const LHCb::MCParticle* mcPart ) const override;

  /// Is the MCParticle reconstructible as given type
  bool isReconstructibleAs( const IMCReconstructible::RecCategory& category,
                            const LHCb::MCParticle*                mcPart ) const override;

private: // methods
  /// Trest acceptance for charged particles
  bool accept_charged( const LHCb::MCParticle* mcPart ) const;

  /// Trest acceptance for neutral particles
  bool accept_neutral( const LHCb::MCParticle* mcPart ) const;

private: // data
  /// Acceptance parameters (neutrals)
  // misc CALO params. Hopefully to go into specific CALO reconstructibility tool
  double m_zECAL    = 12696.0 * Gaudi::Units::mm;
  double m_xECALInn = 363.3 * Gaudi::Units::mm;
  double m_yECALInn = 363.3 * Gaudi::Units::mm;
  double m_xECALOut = 3757.2 * Gaudi::Units::mm;
  double m_yECALOut = 3030.0 * Gaudi::Units::mm;

  /// Threshold for Et gammas reconstructibility
  Gaudi::Property<double> m_lowEt{this, "NeutralEtMin", 200 * Gaudi::Units::MeV};

  /// Allow primary particles
  Gaudi::Property<bool> m_allowPrimary{this, "AllowPrimaryParticles", true};

  /// MCParticle selector
  IMCParticleSelector* m_mcSel = nullptr;

  Gaudi::Property<std::vector<std::string>> m_chargedLongCriteria{this, "ChargedLong", {"hasVeloAndT"}};
  Gaudi::Property<std::vector<std::string>> m_chargedUpstreamCriteria{this, "ChargedUpstream", {"hasVelo", "hasTT"}};
  Gaudi::Property<std::vector<std::string>> m_chargedDownstreamCriteria{this, "ChargedDownstream", {"hasT", "hasTT"}};
  Gaudi::Property<std::vector<std::string>> m_chargedVeloCriteria{this, "ChargedVelo", {"hasVelo"}};
  Gaudi::Property<std::vector<std::string>> m_chargedTCriteria{this, "ChargedTtrack", {"hasT"}};

  // the std::optional is required to delay the construction...
  std::array<std::optional<std::pair<IMCReconstructible::RecCategory, LHCb::MC::MCTrackGeomCriteria>>, 5> m_critMap;

  // data handle for the TrackInfo location
  DataObjectReadHandle<LHCb::MCProperty> m_track_info{this, "MCTrackInfo", LHCb::MCPropertyLocation::TrackInfo};
};

#endif // SIMCOMPONENTS_MCReconstructible_H
