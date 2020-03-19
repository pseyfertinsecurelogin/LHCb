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
#include "GaudiAlg/FilterPredicate.h"
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
  namespace {
    // ==========================================================================
    LoKi::BasicFunctors<LoKi::GenTypes::GenContainer>::BooleanConstant s_NONE{false};
    // ==========================================================================
  } // namespace
  // ==========================================================================
  /** @class ODINFilter
   *  Simple filtering algorithm bases on LoKi/Bender "hybrid" framework
   *  for filtering according to Generator (HepMC) information
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-06-02
   */
  class GenFilter : public Gaudi::Functional::FilterPredicate<bool( LHCb::HepMCEvent::Container const& ),
                                                              Gaudi::Functional::Traits::BaseClass_t<LoKi::FilterAlg>> {
  public:
    // ========================================================================
    /// the main method: execute
    bool       operator()( LHCb::HepMCEvent::Container const& ) const override;
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
      return sc;
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
    mutable Gaudi::Accumulators::BinomialCounter<> m_passed{this, "#passed"};
    // ========================================================================
    /// the functor itself
    LoKi::Types::GCutVal m_cut{s_NONE}; // the functor itself
    // ========================================================================
  };
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
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
    : FilterPredicate{name, pSvc, {"Location", LHCb::HepMCEventLocation::Default}} {
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
  return FilterPredicate::finalize();
}
// ============================================================================
// the main method: execute
// ============================================================================
bool LoKi::GenFilter::operator()( LHCb::HepMCEvent::Container const& events ) const {
  if ( updateRequired() ) {
    StatusCode sc = const_cast<LoKi::GenFilter*>( this )->decode();
    Assert( sc.isSuccess(), "Unable to decode the functor!" );
  }
  //
  // copy all particles into single vector
  //
  LoKi::GenTypes::GenContainer particles;
  for ( const LHCb::HepMCEvent* event : events ) {
    if ( !event ) { continue; } // CONTINUE
    const HepMC::GenEvent* evt = event->pGenEvt();
    if ( !evt ) { continue; } // CONTINUE
    particles.insert( particles.end(), evt->particles_begin(), evt->particles_end() );
  }
  //
  // use the functor
  //
  const bool result = m_cut( particles );
  //
  // some statistics
  //
  m_passed += result;
  //
  // set the filter:
  //
  return result;
}
// ============================================================================
/// the factory (needed for instantiation)
DECLARE_COMPONENT( LoKi::GenFilter )
// ============================================================================
// The END
// ============================================================================
