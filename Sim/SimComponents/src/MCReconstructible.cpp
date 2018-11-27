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
/** @file MCReconstructible.cpp
 *
 *  Implementation file for class : MCReconstructible
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date 28/02/2007
 */
//-----------------------------------------------------------------------------

// local
#include "MCReconstructible.h"

// the data
#include "Event/MCParticle.h"
#include "Event/MCTrackGeomCriteria.h"
#include "CaloDet/DeCalorimeter.h"
#include "LHCbMath/LHCbMath.h"

//-----------------------------------------------------------------------------

DECLARE_COMPONENT( MCReconstructible )

//=============================================================================
// Initialize
//=============================================================================
StatusCode MCReconstructible::initialize()
{
  const StatusCode sc = base_class::initialize();
  if ( sc.isFailure() ) return sc;

  // tools
  m_mcSel = tool<IMCParticleSelector>( "MCParticleSelector", "Selector", this );

  // Setup incident services
  incSvc()->addListener( this, IncidentType::BeginEvent );

  using namespace LHCb::MC;
  // n.b prioritized list - the order matters!
  m_critMap[0].emplace( ChargedLong,       m_chargedLongCriteria.value()      );
  m_critMap[1].emplace( ChargedUpstream,   m_chargedUpstreamCriteria.value()  );
  m_critMap[2].emplace( ChargedDownstream, m_chargedDownstreamCriteria.value());
  m_critMap[3].emplace( ChargedVelo,       m_chargedVeloCriteria.value()      );
  m_critMap[4].emplace( ChargedTtrack,     m_chargedTCriteria.value()         );

  // Calorimeter geometry
  DeCalorimeter* m_calo = getDet<DeCalorimeter>( DeCalorimeterLocation::Ecal );
  LHCb::CaloCellID refOut = LHCb::CaloCellID(2, 0, 6 ,0);
  LHCb::CaloCellID refInn = LHCb::CaloCellID(2, 2, 25 ,23);
  m_xECALInn = std::abs(  m_calo->cellX(refInn)  );
  m_yECALInn = std::abs(  m_calo->cellY(refInn)  );
  m_xECALOut = std::abs(  m_calo->cellX(refOut)  );
  m_yECALOut = std::abs(  m_calo->cellY(refOut)  );

  return sc;
}

// Method that handles various Gaudi "software events"
void MCReconstructible::handle ( const Incident& incident )
{
  if ( IncidentType::BeginEvent == incident.type() ) {
    m_tkInfo.reset();;
  }
}

bool MCReconstructible::inAcceptance( const LHCb::MCParticle* mcPart ) const
{
  return ( !mcPart ? false : ( 0 == mcPart->particleID().threeCharge() ?
                               accept_neutral(mcPart) : accept_charged(mcPart) ) );
}

//====================================================================
// Check if a neutral MCParticle is within the geomtrical acceptance
//====================================================================
bool MCReconstructible::accept_neutral( const LHCb::MCParticle* mcPart ) const
{
  // Acceptance stuff for neutrals
  // Temporary home here whilst a proper CALO tool is being prepared

  bool acc = false;
  if( !mcPart->originVertex() ) return false;
  const double x  = mcPart->originVertex()->position().x();
  const double y  = mcPart->originVertex()->position().y();
  const double z  = mcPart->originVertex()->position().z();
  double pz = mcPart->momentum().pz();
  if( pz < 0 )return false;
  pz = ( pz < 0 ? std::min(pz, -LHCb::Math::lowTolerance) :
                  std::max(pz, LHCb::Math::lowTolerance) );

  const double sx = mcPart->momentum().px()/pz;
  const double sy = mcPart->momentum().py()/pz;
  //debug() << "accepted " << x << " " << y << " " << z << " & " << sx << " " << sy << endmsg ;

  //  if( (mcPart->particleID().threeCharge()) == 0 ) {
  // For neutral particles, the ECAL front has to be hit
  // ---------------------------------------------------
  const double xECAL = x + sx * ( m_zECAL - z );
  const double yECAL = y + sy * ( m_zECAL - z );
  if( ((std::abs(xECAL) <= m_xECALOut) && (std::abs(yECAL) <= m_yECALOut))
      && ((std::abs(xECAL) >= m_xECALInn) || (std::abs(yECAL) >= m_yECALInn ))
      ) {
    if( mcPart->pt() >= m_lowEt ) {
      acc = true;
    }
  }

  return acc;
}

//====================================================================
// Check if a charged MCParticle is within the geomtrical acceptance
//====================================================================
bool MCReconstructible::accept_charged( const LHCb::MCParticle * mcPart ) const
{
  return mcTkInfo().accVelo(mcPart) ||
         mcTkInfo().accTT(mcPart)   ||
         mcTkInfo().accT(mcPart)     ;
}

//=============================================================================
//  Method to check if a particle is reconstructible
//=============================================================================
IMCReconstructible::RecCategory
MCReconstructible::reconstructible( const LHCb::MCParticle* mcPart ) const
{
  if ( !mcPart ) return NoClassification;

  // acceptance ?
  if ( !inAcceptance(mcPart) ) return OutsideAcceptance;

  // Base class MCParticle selection
  if ( m_mcSel->accept(mcPart) )
  {
    // Does MCParticle have a mother
    if ( m_allowPrimary || NULL != mcPart->mother() )
    {

      // charged or neutral
      const bool isCharged = mcPart->particleID().threeCharge() != 0;
      if ( isCharged ) {
        // n.b the order matters !
        auto cat = std::find_if( m_critMap.begin(), m_critMap.end(),
                                 [&](const std::optional<std::pair<IMCReconstructible::RecCategory,
                                                                   LHCb::MC::MCTrackGeomCriteria>>& crit)
                                 { return crit->second.accepted(mcTkInfo(),mcPart); } );
        if (cat!=m_critMap.end()) return (*cat)->first;
      } else { // neutral
        // only one type at the moment
        return Neutral;
      }

    } // has mother
  } // MCP selection

  // if get here, cannot reconstruct particle
  return NotReconstructible;
}


bool MCReconstructible:: isReconstructibleAs(
        const IMCReconstructible::RecCategory& category,
        const LHCb::MCParticle* mcPart ) const{

  // protect against the strip case
  if (category == NoClassification ){
    Warning("Called with no classification", StatusCode::SUCCESS);
    return false;
  }

  if (!inAcceptance(mcPart)) return false;

  const bool isCharged = mcPart->particleID().threeCharge() != 0;
  if (isCharged && category != Neutral && category != NotReconstructible){
    auto criteria = std::find_if( m_critMap.begin(), m_critMap.end(),
                                 [&](const std::optional<std::pair<IMCReconstructible::RecCategory,
                                                                   LHCb::MC::MCTrackGeomCriteria>>& crit)
                                 { return crit->first == category; } );
    if (criteria == m_critMap.end()) {
      Warning("Category not found - defaulting to false",StatusCode::SUCCESS);
      return false;
    }
    return (*criteria)->second.accepted(mcTkInfo(),mcPart) && m_mcSel->accept(mcPart);
  }
  if ( !isCharged && category == Neutral) return true;
  // stupid but true !
  return category == NotReconstructible;
}
