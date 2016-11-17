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
#include <string>
#include <vector>
#include <array>
#include "boost/optional.hpp"

// Gaudi
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/VectorMap.h"

// base class
#include "GaudiAlg/GaudiTool.h"

// Event
#include "Event/MCTrackInfo.h"
#include "Event/MCTrackGeomCriteria.h"

// Interface
#include "MCInterfaces/IMCReconstructible.h"
#include "MCInterfaces/IMCParticleSelector.h"

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
class MCReconstructible : public GaudiTool,
                          virtual public IMCReconstructible,
                          virtual public IIncidentListener
{

public:

  /// Standard constructor
  MCReconstructible( const std::string& type,
                     const std::string& name,
                     const IInterface* parent);

  /// Initialize
  StatusCode initialize() override;

  /** Implement the handle method for the Incident service.
   *  This is used to inform the tool of software incidents.
   *  @param incident The incident identifier
   */
  void handle( const Incident& incident ) override;

  /// Get the reconstruction category for the given MCParticle
  IMCReconstructible::RecCategory reconstructible( const LHCb::MCParticle* mcPart ) const override;

  /// Is the MCParticle in the detector acceptance?
  bool inAcceptance( const LHCb::MCParticle* mcPart ) const override;

  /// Is the MCParticle reconstructible as given type
  bool isReconstructibleAs(const IMCReconstructible::RecCategory& category,
                           const LHCb::MCParticle* mcPart ) const override;

private: // methods

  /// Trest acceptance for charged particles
  bool accept_charged( const LHCb::MCParticle* mcPart ) const;

  /// Trest acceptance for neutral particles
  bool accept_neutral( const LHCb::MCParticle* mcPart ) const;

  /// get the MCTrackInfo object
  inline MCTrackInfo & mcTkInfo() const
  {
    if ( UNLIKELY(!m_tkInfo) ) {
      m_tkInfo = std::make_unique<MCTrackInfo>( evtSvc(), msgSvc() );
      if ( !m_tkInfo ) { Exception( "Failed to load MCTrackInfo" ); }
    }
    return *m_tkInfo;
  }

private: // data

  /// Acceptance parameters (neutrals)
  // misc CALO params. Hopefully to go into specific CALO reconstructibility tool
  double m_zECAL    = 12696.0*Gaudi::Units::mm ;
  double m_xECALInn = 363.3*Gaudi::Units::mm   ;
  double m_yECALInn = 363.3*Gaudi::Units::mm   ;
  double m_xECALOut = 3757.2*Gaudi::Units::mm  ;
  double m_yECALOut = 3030.0*Gaudi::Units::mm  ;

  /// Threshold for Et gammas reconstructibility
  double m_lowEt    = 200*Gaudi::Units::MeV    ;

  /// Allow primary particles
  bool m_allowPrimary;

  /// Pointer to MCTrackInfo object
  mutable std::unique_ptr<MCTrackInfo> m_tkInfo;

  /// MCParticle selector
  IMCParticleSelector * m_mcSel = nullptr;

  std::vector<std::string> m_chargedLongCriteria;
  std::vector<std::string> m_chargedUpstreamCriteria;
  std::vector<std::string> m_chargedDownstreamCriteria;
  std::vector<std::string> m_chargedVeloCriteria;
  std::vector<std::string> m_chargedTCriteria;

  // the boost::optional is required to delay the construction...
  std::array<boost::optional<std::pair<IMCReconstructible::RecCategory,
                                       LHCb::MC::MCTrackGeomCriteria>>,5> m_critMap;

};

#endif // SIMCOMPONENTS_MCReconstructible_H
