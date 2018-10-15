// ============================================================================
// Include files
// ============================================================================
#include "GaudiAlg/FilterPredicate.h"
// ============================================================================
// DAQEvent
// ============================================================================
#include "Event/ODIN.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/FilterAlg.h"
#include "LoKi/OdinTypes.h"
#include "LoKi/IHltFactory.h"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @class ODINFilter
   *  Simple filtering algorithm bases on LoKi/Bender "hybrid" approach
   *  for filtering according to ODIN bank
   *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
   *  @date 2008-09-23
   */
  class ODINFilter : public Gaudi::Functional::FilterPredicate<bool(const LHCb::ODIN&),
                                    Gaudi::Functional::Traits::BaseClass_t<LoKi::FilterAlg>>
  {
  public:
    // ========================================================================
    /// the main method: execute
    bool operator()(const LHCb::ODIN&) const override;
    // ========================================================================
    /** Decode the functor (use the factory)
     *  @see LoKi::FilterAlg
     *  @see LoKi::FilterAlg::decode
     *  @see LoKi::FilterAlg::i_decode
     */
    StatusCode decode () override
    {
      StatusCode sc = i_decode<LoKi::Hybrid::IHltFactory> ( m_cut ) ;
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
    ODINFilter
    ( const std::string& name , // the algorithm instance name
      ISvcLocator*       pSvc ) // pointer to the service locator
    : FilterPredicate( name , pSvc ,
                       KeyValue{"Location", LHCb::ODINLocation::Default } )
    {
      StatusCode sc = setProperty ( "Code" , "ODIN_NONE" ) ;
      Assert ( sc.isSuccess () , "Unable (re)set property 'Code'" ) ;
      sc = setProperty
        ( "Factory" ,
          0 == name.find ( "Hlt1" ) ?
          "LoKi::Hybrid::HltFactory/Hlt1HltFactory:PUBLIC" :
          0 == name.find ( "Hlt2" ) ?
          "LoKi::Hybrid::HltFactory/Hlt2HltFactory:PUBLIC" :
          "LoKi::Hybrid::HltFactory/HltFactory:PUBLIC"     ) ;
      Assert ( sc.isSuccess () , "Unable (re)set property 'Factory'" ) ;
    }
    // ========================================================================
    /// the copy constructor is disabled
    ODINFilter ( const ODINFilter& )  = delete;      // the copy constructor is disabled
    /// the assignement operator is disabled
    ODINFilter& operator=( const ODINFilter& )  = delete; // the assignement is disabled
    // ========================================================================
  private:
    // ========================================================================
    /// the functor itself
    LoKi::Types::ODIN_Cut  m_cut = {  LoKi::BasicFunctors<const LHCb::ODIN*>::BooleanConstant( false ) } ;                        // the functor itself
    mutable Gaudi::Accumulators::BinomialCounter<> m_passed{ this, "#passed" };
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// the main method: execute
bool LoKi::ODINFilter::operator() (const LHCb::ODIN& odin) const // the main method: execute
{
  if ( updateRequired() )
  {
    StatusCode sc = const_cast<LoKi::ODINFilter*>(this)->decode() ;
    Assert ( sc.isSuccess() , "Unable to decode the functor!" ) ;
  }
  // use the functor
  //
  const bool result = m_cut ( &odin ) ;
  //
  // some statistics
  m_passed += result ;
  //
  // set the filter:
  return result;
}
// ============================================================================
/// the factory (needed for instantiation)
DECLARE_COMPONENT( LoKi::ODINFilter )
// ============================================================================
// The END
// ============================================================================
