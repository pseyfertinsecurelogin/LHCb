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
#include "DumpHepMCDecay.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"

//-----------------------------------------------------------------------------
// Implementation file for class : DumpHepMCDecay
//
// 2004-02-18 : Ivan Belyaev
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( DumpHepMCDecay )

//=============================================================================
// Standard constructor
//=============================================================================
DumpHepMCDecay::DumpHepMCDecay( const std::string& name, ISvcLocator* isvc ) : GaudiAlgorithm( name, isvc ) {}

//=============================================================================
// Initialization of the algoritm
//=============================================================================
StatusCode DumpHepMCDecay::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize();
  if ( sc.isFailure() ) { return Error( "Unable to initialize 'GaudiAlgorithm' base ", sc ); }

  auto i = std::find_if( m_quarks.begin(), m_quarks.end(), []( PIDs::const_reference q ) {
    return q < LHCb::ParticleID::down || LHCb::ParticleID::top < q;
  } );
  return ( i != m_quarks.end() ) ? Error( " Invalid Quark ID=" + std::to_string( *i ) ) : StatusCode::SUCCESS;
}

//=============================================================================
// Execution of the algoritm
//=============================================================================
StatusCode DumpHepMCDecay::execute() {

  bool found = false;
  if ( !m_particles.empty() || !m_quarks.empty() ) {

    info() << " Decay dump [cut-off at " << m_levels << " levels] " << endmsg;

    for ( const auto& addr : m_addresses ) {

      LHCb::HepMCEvents* events = get<LHCb::HepMCEvents>( addr );
      if ( !events ) { continue; }

      info() << " Container '" << addr << "' " << endmsg;
      for ( const auto* event : *events ) {

        if ( !event ) { continue; } // CONTINUE
        const HepMC::GenEvent* evt = event->pGenEvt();
        if ( !evt ) { continue; } // CONTINUE

        for ( auto ip = evt->particles_begin(); evt->particles_end() != ip; ++ip ) {

          const HepMC::GenParticle* particle = *ip;
          if ( !particle ) { continue; } // CONTINUE

          bool print =
              ( m_particles.end() != std::find( m_particles.begin(), m_particles.end(), particle->pdg_id() ) ) ||
              std::any_of( m_quarks.begin(), m_quarks.end(),
                           // use LHCb flavour of ParticleID class !                  // NB
                           [p = LHCb::ParticleID( particle->pdg_id() )]( int q ) {
                             return p.hasQuark( LHCb::ParticleID::Quark( q ) );
                           } );
          if ( print ) {
            found = true;
            if ( info().isActive() ) {
              info() << std::endl;
              printDecay( particle, info().stream(), 0 ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
            }
          }
        }
      }
    }
    info() << endmsg;
  }

  if ( !found ) { Warning( " No specified Particles/Quarks are found! " ).ignore(); }

  return StatusCode::SUCCESS;
}

//=============================================================================
// print the decay tree of the particle
//=============================================================================
StatusCode DumpHepMCDecay::printDecay( const HepMC::GenParticle* particle, std::ostream& stream,
                                       unsigned int level ) const {
  if ( 0 == particle ) { return Error( " printDecay(): HepMC::GenParticle* points to NULL" ); }

  static char s_buf[24];
  stream << std::string( s_buf, s_buf + sprintf( s_buf, "%3d", level ) );

  {
    const unsigned int s_maxLevel = 10;
    const std::string  pName      = particleName( particle );
    if ( level < s_maxLevel ) {
      stream << " " << std::string( level * 2, ' ' ) << "|-> " << pName
             << std::string( ( s_maxLevel - level ) * 2, ' ' );
    } else {
      stream << " " << std::string( 2 * s_maxLevel, ' ' ) << "|-> " << pName;
    }
  }

  // print the particle itself
  particle->print( stream );

  const HepMC::GenVertex* vertex = particle->end_vertex();
  if ( 0 == vertex ) { return StatusCode::SUCCESS; } // RETURN

  if ( m_levels <= int( level ) ) { return StatusCode::SUCCESS; }

  // loop over all daughters
  typedef HepMC::GenVertex::particles_out_const_iterator IT;
  for ( IT ip = vertex->particles_out_const_begin(); vertex->particles_out_const_end() != ip; ++ip ) {
    const HepMC::GenParticle* daughter = *ip;
    if ( 0 == daughter ) { continue; }                                                                 // CONTINUE
    printDecay( daughter, stream, level + 1 ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ ); // RECURSION
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
namespace {

  /*
   * @fn adjust
   * adjust the particle name to the desired format
   *  @param name name to be adjusted
   *  @return adjusted name
   *
   */

  inline std::string adjust( const std::string& name ) {
    static const size_t s_maxSize = 12;
    std::string         tmp( name );
    auto                size = tmp.size();
    if ( s_maxSize > size ) {
      tmp += std::string( s_maxSize - size, ' ' );
    } else if ( s_maxSize < size ) {
      tmp                = std::string( tmp.begin(), tmp.begin() + s_maxSize );
      tmp[s_maxSize - 1] = '#';
    }

    return tmp;
  }

} // namespace

//=============================================================================
// get the particle name in the string fixed form
//=============================================================================
std::string DumpHepMCDecay::particleName( const HepMC::GenParticle* particle ) const {

  if ( !particle ) {
    Error( "particlename(): HepMC::GenParticle* points to NULL!" ).ignore();
    return adjust( "#INVALID****" );
  }

  if ( !m_ppSvc ) { m_ppSvc = svc<LHCb::IParticlePropertySvc>( "LHCb::ParticlePropertySvc", true ); }

  const int                     pdg_id = particle->pdg_id();
  const LHCb::ParticleProperty* pp     = nullptr;

  pp = m_ppSvc->find( LHCb::ParticleID( pdg_id ) );
  if ( pp ) { return adjust( pp->particle() ); }

  Warning( "particleName(): ParticleProperty* points to NULL for PDG=" + std::to_string( pdg_id ), StatusCode::SUCCESS,
           0 )
      .ignore();

  return adjust( "#UNKNOWN****" );
}

//=============================================================================
