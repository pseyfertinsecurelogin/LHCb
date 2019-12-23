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
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IService.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/Consumer.h"
// ============================================================================
// GenEvent
// ============================================================================
#include "Event/HepMCEvent.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/GenDecayChain.h"
#include "LoKi/Objects.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @class DumpHepMC
   *  Simple class to dump HepMC-infomration
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-05-04
   */
  class DumpHepMC : public Gaudi::Functional::Consumer<void( LHCb::HepMCEvent::Container const& )> {
  public:
    // ========================================================================
    /// standard initialization of the algorithm
    StatusCode initialize() override {
      return Consumer::initialize().andThen( [&] {
        /// get LoKi service
        svc<IService>( "LoKiSvc", true );
      } );
    }
    /// the only one essential method : execution of the algorithm
    void operator()( const LHCb::HepMCEvent::Container& ) const override;
    /** The standard constructor
     *  @param name algorithm instance name
     *  @param svc  Service Locator
     */
    DumpHepMC( const std::string& name, // algorithm instance name
               ISvcLocator*       svc )       // service locator
        : Consumer{name, svc, {"Input", LHCb::HepMCEventLocation::Default}} {}
    // ========================================================================
  private:
    // ========================================================================
    /// the maximal printout dephth
    Gaudi::Property<unsigned short> m_depth{this, "Depth", 10, "The maximal printout depth"};
    /// print vertex information ?
    Gaudi::Property<bool> m_vertex{this, "PrintVertex", true, "Print vertex information?"};
    /// print end-vertex information ?
    Gaudi::Property<bool> m_vertexe{this, "PrintEndVertex", true, "Print end-vertex information?"};
    /// printout mode  ( @see LoKi::DecayChainBase::Mode )
    Gaudi::Property<unsigned short> m_mode{this, "Mode", LoKi::DecayChainBase::LV_WITHPT,
                                           "Printout mode, see LoKi::DecayChainBase::Mode"};
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// the only one essential method : execution of the algorithm
void LoKi::DumpHepMC::operator()( const LHCb::HepMCEvent::Container& events ) const {

  LoKi::DecayChainBase::Mode mode = LoKi::DecayChainBase::LV_WITHPT;
  switch ( m_mode ) {
  case LoKi::DecayChainBase::LV_STD:
    mode = LoKi::DecayChainBase::LV_STD;
    break;
  case LoKi::DecayChainBase::LV_MASS:
    mode = LoKi::DecayChainBase::LV_MASS;
    break;
  case LoKi::DecayChainBase::LV_WITHPT:
    mode = LoKi::DecayChainBase::LV_WITHPT;
    break;
  case LoKi::DecayChainBase::LV_ONLYP:
    mode = LoKi::DecayChainBase::LV_ONLYP;
    break;
  case LoKi::DecayChainBase::LV_SHORT:
    mode = LoKi::DecayChainBase::LV_SHORT;
    break;
  case LoKi::DecayChainBase::LV_MPTYPHI:
    mode = LoKi::DecayChainBase::LV_MPTYPHI;
    break;
  case LoKi::DecayChainBase::LV_MPTETAPHI:
    mode = LoKi::DecayChainBase::LV_MPTETAPHI;
    break;
  case LoKi::DecayChainBase::LV_MPTY:
    mode = LoKi::DecayChainBase::LV_MPTY;
    break;
  case LoKi::DecayChainBase::LV_MPTETA:
    mode = LoKi::DecayChainBase::LV_MPTETA;
    break;
  default:
    mode = LoKi::DecayChainBase::LV_WITHPT;
    break;
  }

  // get the smart printer
  LoKi::GenDecayChain printer( m_depth, m_vertex.value(), mode, MSG::YELLOW, MSG::RED, m_vertexe.value() );

  MsgStream& log = always();
  //
  log << " HepMC event dump '" << inputLocation() << "' \n";
  //
  printer.print( &events,                 // input data
                 log.stream(),            // the stream
                 '\n',                    // the terminator
                 LoKi::Objects::_ALL_,    // accept
                 LoKi::Objects::_NONE_ ); // mark
  //
  log << endmsg;
}
// ============================================================================
/// Declaration of the Algorithm Factory
DECLARE_COMPONENT( LoKi::DumpHepMC )
// ============================================================================
// The END
// ============================================================================
