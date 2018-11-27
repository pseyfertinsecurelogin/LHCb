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
#include <numeric>
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
// ============================================================================
// LHCbKernel
// ============================================================================
#include "Kernel/Counters.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functor.h"
#include "LoKi/ICoreFactory.h"
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
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @class CounterAlg
   *  Simple algorithm to dealwith counters.
   *  It has two modes:
   *
   *    - "simple" mode, when certain counters are evaluated and are
   *       kept locally
   *
   *    - "store-in-TES", when the values are also stored in TES in form of
   *                      Gaudi::Numbers object at specified location
   *                      the default location is "Counters/<ALGNAME>"
   *
   *  @code
   *
   *    myCnt.Variables = {
   *       'nSpd'           : "CONTAINS('Raw/Spd/Digits')" ,
   *       'nVelo'          : "TrSOURCE('Rec/Track/Best', TrVELO) >> TrSIZE " ,
   *       'hasTrueMCDecay' : "MCSOURCE('MC/Particles','[B_s0 -> J/psi(1S) phi(1020)]CC') >> MCSIZE" ,
   *       'GEC-loose'      : "ACCEPT('Hlt::GEC/AcceptGESLoose')"
   *     }
   *
   *    ## use non-standard location:
   *    myCnt.Location = "/Event/MyCounters"
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV Ivan.BElyaev@cern.ch
   *  @date 2011-02-12
   */
  class CounterAlg : public GaudiAlgorithm
  {
  public:
    // ========================================================================
    /// the main method: execute
    StatusCode execute    ()  override;
    /// proper initialization
    StatusCode initialize ()  override;
    /// proper finalization
    StatusCode finalize   ()  override;
    // ========================================================================
  protected:
    // ========================================================================
    /** helper class to keep the N-tuple items
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-02-16
     */
    class Item final
    {
    public:
      // ====================================================================
      // the default constructor
      Item () = default;
      // ====================================================================
      /// the variable name
      std::string                                             m_name ;
      /// the functor itself
      LoKi::Assignable_t<LoKi::BasicFunctors<void>::Function> m_fun  = LoKi::BasicFunctors<void>::Constant ( -1.e+10 );
      /// helper counter
      StatEntity*                                             m_cnt  = nullptr;
      // ====================================================================
    } ;
  public:
    /** standard constructor
     *  @see GaudiAlgorithm
     *  @see      Algorithm
     *  @see      AlgFactory
     *  @see     IAlgFactory
     *  @param name the algorithm instance name
     *  @param pSvc pointer to Service Locator
     */
    CounterAlg
    ( const std::string& name , // the algorithm instance name
      ISvcLocator*       pSvc ) // pointer to the service locator
      : GaudiAlgorithm ( name , pSvc )
    {
      //
      //
      //
    }
    // ========================================================================
  protected:
    // ========================================================================
    /// update variables
    StatusCode updateItems () ;                // update variables
    // ========================================================================
    /// the preambulo
    std::string preambulo() const
    {
      const auto& lines = m_preambulo.value() ;
      return std::accumulate( lines.begin(), lines.end(),
                              std::string{},
                              [](std::string r, const std::string& line)
                              { return r.empty() ? line : r + '\n' + line; } );
    }
    // ========================================================================
  private:
    // ========================================================================
    /// map pf variables
    Gaudi::Property<std::map<std::string,std::string>> m_map   // map pf variables
        { this, "Variables" , {}, [=](auto&) {
              if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) return;
              Warning ( "Reintialization of Variables" ).ignore() ;
              StatusCode sc = updateItems () ;
              Assert ( sc.isSuccess() , "Unable to set 'Variables'"   , sc ) ;
          },
          "The map { 'name' : 'functor'} of variables: void -> double " };
    /// the preambulo
    Gaudi::Property<std::vector<std::string>> m_preambulo  // the preambulo
        { this, "Preambulo"  , {}, [=](auto&) {
              if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) { return ; }
              Warning ( "Reintialization of Preambulo" ).ignore() ;
              StatusCode sc = updateItems () ;
              Assert ( sc.isSuccess () , "Unable to set 'Preambulo'"   , sc ) ;
          },
          "The preambulo lines to be used for the temporary python script" };
    ///  factory for functor decoding
    Gaudi::Property<std::string>   m_factory    //  factory for functor decoding
      { this, "Factory", "LoKi::Hybrid::CoreFactory/CoreFactory:PUBLIC",
        [=](auto&) {
              if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) return ;
              Warning ( "Reintialization of HltFactory" ).ignore() ;
              StatusCode sc = updateItems () ;
              Assert ( sc.isSuccess() , "Unable to set 'Variables'"   , sc ) ;
        }, "The type/name of LoKiBender \"hybrid\" factory: void->double" };
    ///  TES-location of counters
    Gaudi::Property<std::string>    m_location   //  TES-location of counters
      { this, "Location"  , "Counters/" + name(), "TES-location of counters" };
    /// decoded vector of functors/items
    std::vector<Item>        m_items     ;
    // ========================================================================
  };
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// update  variables
// ============================================================================
StatusCode LoKi::CounterAlg::updateItems ()                 // update variables
{
  try {
      // get the factory
      LoKi::Hybrid::ICoreFactory* factory = tool<LoKi::Hybrid::ICoreFactory> ( m_factory , this ) ;
      //
      m_items.clear() ;
      m_items.reserve ( m_map.size() ) ;
      std::transform( m_map.begin(),m_map.end(),std::back_inserter(m_items),
                      [&](const auto& ivar) {
        Item item ;
        StatusCode sc = factory->get ( ivar.second , item.m_fun , preambulo() ) ;
        if ( sc.isFailure() )
        { throw Error
            ("Unable to decode " + ivar.first + " : " + ivar.second , sc ) ; }
        //
        item.m_name =           ivar.first   ;
        item.m_cnt  = &counter( ivar.first ) ;
        //
        //
        debug() << "The decoded variable name is '"
                << item.m_name << "'\t, the functor : '"
                << item.m_fun  << "'" << endmsg ;
        return item;
      } );
      //
      return release ( factory ) ; // we do not need the factory anymore
  } catch (const StatusCode& sc) {
      return sc;
  }
  //
}
// ============================================================================
// standard initialization
// ============================================================================
StatusCode LoKi::CounterAlg::initialize ()
{
  StatusCode sc = GaudiAlgorithm::initialize () ;
  if ( sc.isFailure() ) { return sc ; }
  svc<ILoKiSvc> ( "LoKiSvc" , true ) ;
  //
  // decode variables:
  //
  if  ( ! m_map.empty() )
  {
    sc = updateItems() ;
    if ( sc.isFailure () ) { return Error ( "Unable to decode Variables", sc ) ; }
  }
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
// standard finalization
// ============================================================================
StatusCode LoKi::CounterAlg::finalize   ()
{
  m_items . clear() ;
  return GaudiAlgorithm::finalize () ;
}
// ============================================================================
// the main method: execute
// ============================================================================
StatusCode LoKi::CounterAlg::execute ()
{
  setFilterPassed(true);
  //
  Gaudi::Numbers* numbers = nullptr ;
  //
  // get(create) counters in TES (if needed)
  if ( !m_location.empty() )
  { numbers = getOrCreate<Gaudi::Numbers,Gaudi::Numbers>
      ( evtSvc() , m_location.value() ) ; }
  //
  for ( auto item = m_items.cbegin() ; m_items.cend() != item ; ++item )
  {
    // evaluate the variable
    const double value = (item->m_fun)() ;
    // update TES object
    if ( numbers ) { numbers->counters().update ( item->m_name , value ) ; }
    // update local counters
    item->m_cnt->add ( value ) ;
  }
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================

// ============================================================================
/// the factory (needed for instantiation)
DECLARE_COMPONENT( LoKi::CounterAlg )
// ============================================================================

// ============================================================================
// The END
// ============================================================================
