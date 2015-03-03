// $Id: VoidFilter.cpp,v 1.1 2008-10-19 16:11:40 ibelyaev Exp $
// ============================================================================
// Include files 
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/AlgFactory.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/FilterAlg.h"
#include "LoKi/ICoreFactory.h"
// ============================================================================
namespace LoKi 
{
  // ==========================================================================
  /** @class VoidFilter 
   *  Simple filtering algorithm bases on LoKi/Bender "hybrid" approach
   *  for filtering according to "void" input data 
   *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
   *  @date 2008-09-23
   */
  class VoidFilter : public LoKi::FilterAlg 
  {
    // ========================================================================
    /// friend factory for instantiation 
    friend class AlgFactory<LoKi::VoidFilter> ;
    // ========================================================================
  public:
    // ========================================================================
    /// the main method: execute 
    virtual StatusCode execute () ;
    // ========================================================================
  public:
    // ========================================================================
    /** Decode the functor (use the factory)
     *  @see LoKi::FilterAlg
     *  @see LoKi::FilterAlg::decode
     *  @see LoKi::FilterAlg::i_decode
     */
    virtual StatusCode decode () 
    {
      StatusCode sc = i_decode<LoKi::Hybrid::ICoreFactory> ( m_cut ) ;
      Assert ( sc.isSuccess() , "Unable to decode the functor!" ) ;
      return StatusCode::SUCCESS ;
    }
    // ========================================================================
  protected:
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
    VoidFilter
    ( const std::string& name , // the algorithm instance name 
      ISvcLocator*       pSvc ) // pointer to the service locator
      : LoKi::FilterAlg ( name , pSvc ) 
      // the functor itself
      , m_cut ( LoKi::BasicFunctors<void>::BooleanConstant( false ) ) 
    {
      //
      StatusCode sc = setProperty ( "Code" , "FNONE" ) ;
      Assert ( sc.isSuccess () , "Unable (re)set property 'Code'"    , sc ) ;
      sc = setProperty 
        ( "Factory" , "LoKi::Hybrid::CoreFactory/CoreFactory::PUBLIC" ) ;
      Assert ( sc.isSuccess () , "Unable (re)set property 'Factory'" , sc ) ;
    } 
    /// virtual and protected destructor 
    virtual ~VoidFilter () {} ;
    // ========================================================================
  private:
    // ========================================================================
    /// the default constructor is disabled 
    VoidFilter () ;                      // the default constructor is disabled 
    /// the copy constructor is disabled 
    VoidFilter ( const VoidFilter& ) ;      // the copy constructor is disabled 
    /// the assignement operator is disabled 
    VoidFilter& operator=( const VoidFilter& ) ; // the assignement is disabled
    // ========================================================================
  private:
    // ========================================================================
    /// the functor itself 
    LoKi::Types::FCut    m_cut ;                        // the functor itself 
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace LoKi 
// ============================================================================
// the main method: execute 
// ============================================================================
StatusCode LoKi::VoidFilter::execute () // the main method: execute 
{
  if ( updateRequired() ) 
  {
    StatusCode sc = decode() ; 
    Assert ( sc.isSuccess() , "Unable to decode the functor!" ) ;
  }
  // 
  // use the functor 
  // 
  const bool result = m_cut () ;
  //
  // some statistics
  counter ("#passed" ) += result ;
  // 
  // set the filter:
  setFilterPassed ( result ) ;
  //
  return StatusCode::SUCCESS ;
}  
// ============================================================================
/// the factory (needed for instantiation)
DECLARE_NAMESPACE_ALGORITHM_FACTORY(LoKi,VoidFilter)
// ============================================================================
// The END
// ============================================================================
