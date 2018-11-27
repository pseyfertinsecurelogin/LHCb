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
// ============================================================================
// Include files
// ============================================================================
// HepMC
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"

// GenEvent
#include "Event/HepMCEvent.h"

// Local
#include "DumpHepMCDecay.h"
// ============================================================================
/** @class DumpHepMCTree
 *  simple class to dump HepMC::GenEvent obejcts
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-10-25
 */
struct DumpHepMCTree : DumpHepMCDecay
{
  using DumpHepMCDecay::DumpHepMCDecay;

  StatusCode execute() override;
  /** print the decay tree of the particle
   *  @param vertex  pointer to the vertex to be printed
   *  @param stream   output stream
   *  @param level    decay level
   *  @return statsu code
   */
  StatusCode printDecay
  ( HepMC::GenVertex* vertex                ,
    std::ostream&     stream    = std::cout ,
    unsigned int      level     = 0         ) const;
} ;
// ============================================================================
// Declaration of the Algorithm Factory
// ============================================================================
DECLARE_COMPONENT( DumpHepMCTree )
// ============================================================================
StatusCode DumpHepMCTree::execute()
{
  // get the stream
  MsgStream& log = info() ;
  log << " Tree dump [cut-off at " << m_levels << " levels] " << endmsg ;
  //
  for( const auto& addr : m_addresses ) {
    //
    LHCb::HepMCEvents* events = get<LHCb::HepMCEvents>( addr ) ;
    if( !events ) { continue ; }
    //
    log << " Container '"  << addr << "' " << endmsg ;
    for ( const auto& event : *events )
    {
      if ( !event ) { continue ; }                     // CONTINUE
      const HepMC::GenEvent* evt = event->pGenEvt() ;
      if ( !evt   ) { continue ; }                     // CONTINUE
      log << " #particles/vertices : "
          << evt->particles_size() << "/"
          << evt->vertices_size() << endmsg ;

      HepMC::GenVertex* signal = evt->signal_process_vertex() ;
      if ( !signal )
      {
        signal = evt->barcode_to_vertex( -1 ) ;
        if ( 0 != signal )
        { Warning ( "Signal_process_vertex is NULL, use -1 " ).ignore() ; }
        else
        { 
          Warning ( "Signal_process_vertex is NULL, skip!  " ).ignore() ;
          continue ;
        }
      }
      if ( log.isActive() )
      { printDecay ( signal , log.stream() , 0 ) ; }
    }
  }
  log << endmsg ;
  return StatusCode::SUCCESS ;
}
// ============================================================================
/** print the decay tree of the particle
 *  @param vertex  pointer to the vertex to be printed
 *  @param stream   output stream
 *  @param level    decay level
 *  @return statsu code
 */
// ============================================================================
StatusCode DumpHepMCTree::printDecay
( HepMC::GenVertex* vertex  ,
  std::ostream&     stream  ,
  unsigned int      level   ) const
{
  if ( !vertex ) { return StatusCode::FAILURE ; }
  std::for_each( vertex -> particles_begin ( HepMC::children ),
                 vertex -> particles_end   ( HepMC::children ),
                 [&](const HepMC::GenParticle* p)
                 { DumpHepMCDecay::printDecay( p ,stream ,level ) ; });
  return StatusCode::SUCCESS ;
}
// ============================================================================
/// The END
// ============================================================================
