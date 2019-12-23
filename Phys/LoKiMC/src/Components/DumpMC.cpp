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
// MCEvent
// ============================================================================
#include "Event/MCParticle.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/MCDecayChain.h"
#include "LoKi/Objects.h"
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
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-06-03
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @class DumpMC
   *  Simple class to dump MC-information
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-05-04
   */
  class DumpMC : public Gaudi::Functional::Consumer<void( const LHCb::MCParticle::Container& )> {
  public:
    // ========================================================================
    /// standard initialization of the algorithm
    StatusCode initialize() override {
      StatusCode sc = GaudiAlgorithm::initialize();
      if ( sc.isFailure() ) { return sc; }
      /// get LoKi service
      svc<IService>( "LoKiSvc", true );
      return StatusCode::SUCCESS;
    }
    /// the only one essential method : execution of the algorithm
    void operator()( const LHCb::MCParticle::Container& ) const override;
    /** The standard constructor
     *  @param name algorithm instance name
     *  @param svc  Service Locator
     */
    DumpMC( const std::string& name, // algorithm instance name
            ISvcLocator*       svc )       // service locator
        : Consumer{name, svc, {"Input", LHCb::MCParticleLocation::Default}} {}
    // ========================================================================
  private:
    // ========================================================================
    /// the maximal printout dephth
    Gaudi::Property<unsigned short> m_depth{this, "Depth", 10, "The maximal printout depth"};
    /// print vertex information ?
    Gaudi::Property<bool> m_vertex{this, "PrintVertex", true, "Print vertex information?"};
    /// print end-vertex information ?
    Gaudi::Property<bool> m_vertexe{this, "PrintEndVertex", true, "Print end-vertex information?"};
    /// print only decay-vertices ?
    Gaudi::Property<bool> m_vertexd{this, "DecayOnlyVertices", true, "Decay-only vertices ?"};
    /// printout mode  ( @see LoKi::DecayChainBase::Mode )
    Gaudi::Property<unsigned short> m_mode{this, "Mode", LoKi::DecayChainBase::LV_WITHPT,
                                           "Printout mode, see LoKi::DecayChainBase::Mode"};
    // ========================================================================
  };
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// the only one essential method : execution of the algorithm
// ============================================================================
void LoKi::DumpMC::operator()( const LHCb::MCParticle::Container& particles ) const {
  //
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
  //
  // get the smart printer
  //
  LoKi::MCDecayChain printer( m_depth, m_vertex.value(), mode, MSG::YELLOW, MSG::RED, m_vertexe.value(),
                              m_vertexd.value() );
  //
  MsgStream& log = always();
  //
  log << " MC-particles dump '" << inputLocation() << "' \n";
  printer.print( &particles,              // the input
                 log.stream(),            // the stream
                 '\n',                    // the terminator
                 LoKi::Objects::_ALL_,    // accept
                 LoKi::Objects::_NONE_ ); // mark
  //
  log << endmsg;
  //
}
// ============================================================================
/// Declaration of the Algorithm Factory
DECLARE_COMPONENT( LoKi::DumpMC )
// ============================================================================
// The END
// ============================================================================
