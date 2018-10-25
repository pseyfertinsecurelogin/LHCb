/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
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
// MCEvent
// ============================================================================
#include "Event/MCParticle.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/FilterAlg.h"
#include "LoKi/MCTypes.h"
#include "LoKi/IMCHybridFactory.h"
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
namespace
{
  // ==========================================================================
  LoKi::BasicFunctors<LHCb::MCParticle::ConstVector>::BooleanConstant s_NONE =
    LoKi::BasicFunctors<LHCb::MCParticle::ConstVector>::BooleanConstant ( false ) ;
  // ==========================================================================
}
namespace LoKi
{
  // ==========================================================================
  /** @class MCFilter
   *  Simple filtering algorithm bases on LoKi/Bender "hybrid" framework
   *  for filtering according to MC -information
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-06-02
   */
  class MCFilter : public Gaudi::Functional::FilterPredicate<bool(const LHCb::MCParticle::Container&),
                                                             Gaudi::Functional::Traits::BaseClass_t<LoKi::FilterAlg> >
  {
  public:
    // ========================================================================
    /// the main method: execute
    bool operator() (const LHCb::MCParticle::Container& particles) const override;
    StatusCode finalize () override;
    // ========================================================================
    /** Decode the functor (use the factory)
     *  @see LoKi::FilterAlg
     *  @see LoKi::FilterAlg::decode
     *  @see LoKi::FilterAlg::i_decode
     */
    StatusCode decode () override
    {
      StatusCode sc = i_decode<LoKi::IMCHybridFactory> ( m_cut ) ;
      Assert ( sc.isSuccess() , "Unable to decode the functor!" ) ;
      return StatusCode::SUCCESS ;
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
    MCFilter
    ( const std::string& name ,    // the algorithm instance name
      ISvcLocator*       pSvc ) ; // pointer to the service locator
    // ========================================================================
    /// the copy constructor is disabled
    MCFilter ( const MCFilter& ) = delete;  // the copy constructor is disabled
    /// the assignement operator is disabled
    MCFilter& operator=( const MCFilter& ) = delete;// the assignement is disabled
    // ========================================================================
  private:
    // ========================================================================
    /// the functor itself
    LoKi::Types::MCCutVal m_cut = s_NONE; // the functor itself
    /// TES location of LHCb::MCParticle::Container object
    std::string m_location ;     // TES location of LHCb::MCParticle::Container

    mutable Gaudi::Accumulators::BinomialCounter<> m_passed{ this, "#passed" };
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
LoKi::MCFilter::MCFilter
( const std::string& name , // the algorithm instance name
  ISvcLocator*       pSvc ) // pointer to the service locator
  : FilterPredicate ( name , pSvc ,
                      KeyValue{ "Location", LHCb::MCParticleLocation::Default } )
{
  StatusCode sc = setProperty ( "Code" , "~MCEMPTY" ) ;
  Assert ( sc.isSuccess () , "Unable (re)set property 'Code'"    , sc ) ;
  sc = setProperty
    ( "Factory" , "LoKi::Hybrid::MCTool/MCFactory:PUBLIC" ) ;
  Assert ( sc.isSuccess () , "Unable (re)set property 'Factory'" , sc ) ;
}
// ============================================================================
// finalize
// ============================================================================
StatusCode LoKi::MCFilter::finalize ()
{
  m_cut = s_NONE ;
  return LoKi::FilterAlg::finalize () ;
}
// ============================================================================
// the main method: execute
// ============================================================================
bool LoKi::MCFilter::operator() (const LHCb::MCParticle::Container& particles) const // the main method
{
  if ( updateRequired() ) {
    StatusCode sc = const_cast<LoKi::MCFilter*>(this)->decode() ;
    Assert ( sc.isSuccess() , "Unable to decode the functor!" ) ;
  }
  //
  // copy all particles into single vector
  // and use the functor
  //
  const bool result = m_cut ( { particles.begin(), particles.end() } ) ;
  //
  // some statistics
  //
  m_passed += result ;
  //
  return result;
}
// ============================================================================
/// the factory (needed for instantiation)
DECLARE_COMPONENT( LoKi::MCFilter )
// ============================================================================
// The END
// ============================================================================
