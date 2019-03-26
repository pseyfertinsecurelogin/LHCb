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
// GenEvent
// ============================================================================
#include "Event/HepMCEvent.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/FilterAlg.h"
#include "LoKi/GenTypes.h"
#include "LoKi/IGenHybridFactory.h"
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @class ODINFilter
   *  Simple filtering algorithm bases on LoKi/Bender "hybrid" framework
   *  for filtering according to Generator (HepMC) information
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-06-02
   */
  class GenFilter : public LoKi::FilterAlg {
  public:
    // ========================================================================
    /// the main method: execute
    StatusCode execute() override;
    StatusCode finalize() override;
    // ========================================================================
  public:
    // ========================================================================
    /** Decode the functor (use the factory)
     *  @see LoKi::FilterAlg
     *  @see LoKi::FilterAlg::decode
     *  @see LoKi::FilterAlg::i_decode
     */
    StatusCode decode() override {
      StatusCode sc = i_decode<LoKi::IGenHybridFactory>( m_cut );
      Assert( sc.isSuccess(), "Unable to decode the functor!" );
      return StatusCode::SUCCESS;
    }
    // ========================================================================
    /** standard constructor
     *  @see LoKi::FilterAlg
     *  @see GaudiAlgorithm
     *  @see      Algorithm
     *  @see      AlgFactory
     *  @see     IAlgFactory
     *  @param name the algorithm instance name
     *  @param pSvc pointer to Service Locator
     */
    GenFilter( const std::string& name, // the algorithm instance name
               ISvcLocator*       pSvc );     // pointer to the service locator
    // ========================================================================
  private:
    // ========================================================================
    /// the functor itself
    LoKi::Types::GCutVal m_cut; // the functor itself
    /// TES location of LHCb::HepMCEvent::Container object
    std::string m_location; // TES location of LHCb::HepMCEvent::Container
    // ========================================================================
  };
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
namespace {
  // ==========================================================================
  LoKi::BasicFunctors<LoKi::GenTypes::GenContainer>::BooleanConstant s_NONE{false};
  // ==========================================================================
} // namespace
// ============================================================================
/* standard constructor
 *  @see LoKi::FilterAlg
 *  @see GaudiAlgorithm
 *  @see      Algorithm
 *  @see      AlgFactory
 *  @see     IAlgFactory
 *  @param name the algorithm instance name
 *  @param pSvc pointer to Service Locator
 */
// ===========================================================================
LoKi::GenFilter::GenFilter( const std::string& name, // the algorithm instance name
                            ISvcLocator*       pSvc )      // pointer to the service locator
    : LoKi::FilterAlg( name, pSvc )
    // the functor itself
    , m_cut( s_NONE )
    // TES location of LHCb::HEpMCEvent::Constainer object
    , m_location( LHCb::HepMCEventLocation::Default ) {
  //
  declareProperty( "Location", m_location, "TES location of LHCb::HepMCEvent::Container object" );
  //
  StatusCode sc = setProperty( "Code", "~GEMPTY" );
  Assert( sc.isSuccess(), "Unable (re)set property 'Code'", sc );
  sc = setProperty( "Factory", "LoKi::Hybrid::GenTool/GenFactory:PUBLIC" );
  Assert( sc.isSuccess(), "Unable (re)set property 'Factory'", sc );
}
// ============================================================================
// finalize
// ============================================================================
StatusCode LoKi::GenFilter::finalize() {
  m_cut = s_NONE;
  return LoKi::FilterAlg::finalize();
}
// ============================================================================
// the main method: execute
// ============================================================================
StatusCode LoKi::GenFilter::execute() // the main method: execute
{
  if ( updateRequired() ) {
    StatusCode sc = decode();
    Assert( sc.isSuccess(), "Unable to decode the functor!" );
  }
  //
  // get HepMC information from TES
  //
  const LHCb::HepMCEvent::Container* events = get<LHCb::HepMCEvent::Container>( m_location );
  if ( 0 == events ) { return StatusCode::FAILURE; }
  //
  // copy all particles into single vector
  //
  LoKi::GenTypes::GenContainer particles;
  for ( LHCb::HepMCEvent::Container::const_iterator ievent = events->begin(); events->end() != ievent; ++ievent ) {
    const LHCb::HepMCEvent* event = *ievent;
    if ( 0 == event ) { continue; } // CONTINUE
    //
    const HepMC::GenEvent* evt = event->pGenEvt();
    if ( 0 == evt ) { continue; } // CONTINUE
    //
    particles.insert( particles.end(), evt->particles_begin(), evt->particles_end() );
  }
  //
  // use the functor
  //
  const bool result = m_cut( particles );
  //
  // some statistics
  //
  counter( "#passed" ) += result;
  //
  // set the filter:
  //
  setFilterPassed( result );
  //
  return StatusCode::SUCCESS;
}
// ============================================================================
/// the factory (needed for instantiation)
DECLARE_COMPONENT( LoKi::GenFilter )
// ============================================================================
// The END
// ============================================================================
