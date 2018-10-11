// ============================================================================
// Include files
// ============================================================================
#include "GaudiAlg/FilterPredicate.h"
// ============================================================================
// HltEvent
// ============================================================================
#include "Event/HltDecReports.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/FilterAlg.h"
#include "LoKi/HLTTypes.h"
#include "LoKi/IHltFactory.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/algorithm/string.hpp"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @class HDRFilter
   *  Simple filtering algorithm bases on LoKi/Bender "hybrid" approach
   *  for filtering according to LHCb::HltDecReports
   *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
   *  @date 2008-09-23
   */
  class HDRFilter : public Gaudi::Functional::FilterPredicate<bool(const LHCb::HltDecReports&),
                                                              Gaudi::Functional::Traits::BaseClass_t<LoKi::FilterAlg>>
  {
  public:
    // ========================================================================
    /// the main method: execute
    bool operator()(const LHCb::HltDecReports&) const override;
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
    HDRFilter
    ( const std::string& name ,   // the algorithm instance name
      ISvcLocator*       pSvc ) ; // pointer to the service locator
    // ========================================================================
    /// the copy constructor is disabled
    HDRFilter ( const HDRFilter& ) = delete;
    /// the assignement operator is disabled
    HDRFilter& operator=( const HDRFilter& ) = delete;
    // ========================================================================
  private:
    // ========================================================================
    /// the functor itself
    LoKi::Types::HLT_Cut  m_cut = { LoKi::BasicFunctors<const LHCb::HltDecReports*>::BooleanConstant( false ) }  ;                         // the functor itself
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
namespace
{
  //
  inline bool _hlt1_ ( std::string name )
  {
    boost::to_upper ( name ) ;
    return std::string::npos != name.find ("HLT1" ) ;
  }
  //
  inline bool _hlt2_ ( std::string name )
  {
    boost::to_upper ( name ) ;
    return std::string::npos != name.find ("HLT2" ) ;
  }
  inline
  //
  bool _ok_ ( const std::string& n1 ,
              const std::string& n2 )
  {
    const bool h1_1 = _hlt1_ ( n1 ) ;
    const bool h1_2 = _hlt1_ ( n2 ) ;
    const bool h2_1 = _hlt2_ ( n1 ) ;
    const bool h2_2 = _hlt2_ ( n2 ) ;
    //
    return
      ( h1_1 && h1_2 && !h2_1 && !h2_2 ) ||
      ( h2_1 && h2_2 && !h1_1 && !h1_2 ) ;
  }
  bool _ok_ ( const std::string& n1 ,
              const std::string& n2 ,
              const std::string& n3 )
  {
    const bool h1_1 = _hlt1_ ( n1 ) ;
    const bool h1_2 = _hlt1_ ( n2 ) ;
    const bool h1_3 = _hlt1_ ( n3 ) ;
    const bool h2_1 = _hlt2_ ( n1 ) ;
    const bool h2_2 = _hlt2_ ( n2 ) ;
    const bool h2_3 = _hlt2_ ( n3 ) ;
    //
    return
      ( h1_1 && h1_2 && h1_3 && !h2_1 && !h2_2 && !h2_3 ) ||
      ( h2_1 && h2_2 && h2_3 && !h1_1 && !h1_2 && !h1_3 ) ;
  }
  // special case: Hlt2*Hlt1*
  inline bool special_case ( const std::string& name )
  {
    return
      4 < name.size()          &&
      boost::algorithm::starts_with(name,"Hlt2") &&
      _hlt1_ ( name ) ;
  }
  // ==========================================================================
  std::string location(const std::string& name) {
      // TES location of LHCb::HltDecReports
      std::string loc = LHCb::HltDecReportsLocation::Default;
      if ( special_case ( name ) )
      { loc =  LHCb::HltDecReportsLocation::Hlt2Default ; }
      else if ( std::string::npos != name.find ( "Hlt1"  ) ||
                std::string::npos != name.find ( "HLT1"  ) )
      { loc =  LHCb::HltDecReportsLocation::Hlt1Default ; }
      else if ( std::string::npos != name.find ( "Hlt2"  ) ||
                std::string::npos != name.find ( "HLT2"  ) )
      { loc =  LHCb::HltDecReportsLocation::Hlt2Default ; }
      else if ( std::string::npos != name.find ( "Strip" ) ||
                std::string::npos != name.find ( "STRIP" ) )
      { loc =  "Strip/Phys/DecReports"                  ; }
      return loc;
  }

} // ==========================================================================
// ============================================================================
/*  standard constructor
 *  @see LoKi::FilterAlg
 *  @see GaudiAlgorithm
 *  @see      Algorithm
 *  @see      AlgFactory
 *  @see     IAlgFactory
 *  @param name the algorithm instance name
 *  @param pSvc pointer to Service Locator
 */
// ============================================================================
LoKi::HDRFilter::HDRFilter
( const std::string& name , // the algorithm instance name
  ISvcLocator*       pSvc ) // pointer to the service locator
: FilterPredicate( name , pSvc, KeyValue{ "Location", location(name) } )
{
  // DAMN: this isn't going to (always) work: what if Location is set first - and the test fails,
  //       and then afterwards code() is updated by the property mgr?
  const Property& prop = getProperty ( "Location" );
  const_cast<Property&>(prop).declareUpdateHandler( [=](Property&) {
    const auto& nam = this->name();
    const auto& loc = inputLocation();
  /// the special name
    if ( special_case ( nam ) )
  {
      std::string s ( nam , 5 ) ;
      if ( !_ok_ ( s       , loc ) )
      { Error    ( "Inconsistent setting of name&location/2"      ).ignore() ;  }
      if ( !_ok_ ( code()  , loc ) )
      { Error    ( "Inconsistent setting of code&location/2"      ).ignore() ;  }
      if ( !_ok_ ( s       , code() , loc ) )
      { Warning  ( "Inconsistent setting of name&code&location/2" ).ignore() ;  }
    } else if ( !_hlt1_ ( nam      )   &&
                !_hlt1_ ( this->code ()     )   &&
                !_hlt1_ ( loc  )   &&
                !_hlt2_ ( nam      )   &&
                !_hlt2_ ( this->code ()     )   &&
                !_hlt2_ ( loc  ) ) { /* stripping case? */ }
    else {
      if ( !_ok_ ( this->code () , loc ) )
      { Error    ( "Inconsistent setting of code&location      " ).ignore() ;  }
      if ( !_ok_ ( nam  , loc ) )
      { Warning  ( "Inconsistent setting of name&location      " ).ignore() ;  }
      if ( !_ok_ ( nam , this->code() , loc ) )
      { Warning  ( "Inconsistent setting of name&code&location " ).ignore() ;  }
  }
  });

  StatusCode sc = setProperty ( "Code" , "HLT_NONE" ) ;
  Assert ( sc.isSuccess () , "Unable (re)set property 'Code'" ) ;
  sc = setProperty
    ( "Factory" ,
      boost::algorithm::starts_with( name,  "Hlt1" ) ?
      "LoKi::Hybrid::HltFactory/Hlt1HltFactory:PUBLIC" :
      boost::algorithm::starts_with( name,  "Hlt2" ) ?
      "LoKi::Hybrid::HltFactory/Hlt2HltFactory:PUBLIC" :
      "LoKi::Hybrid::HltFactory/HltFactory:PUBLIC"     ) ;
  Assert ( sc.isSuccess () , "Unable (re)set property 'Factory'" ) ;
  //
}
// ============================================================================
// the main method: execute
bool LoKi::HDRFilter::operator()(const LHCb::HltDecReports& hdr) const // the main method
{
  if ( updateRequired() ) {
    // @TODO/@FIXME: get rid of this const_cast...
    StatusCode sc = const_cast<LoKi::HDRFilter*>(this)->decode() ;
    Assert ( sc.isSuccess() , "Unable to decode the functor!" ) ;
  }
  //
  // use the functor
  const bool result = m_cut ( &hdr ) ;
  //
  // some statistics
  counter ("#passed" ) += result ;

  return result;
}
// ============================================================================
/// the factory (needed for instantiation)
DECLARE_COMPONENT( LoKi::HDRFilter )
// ============================================================================
// The END
// ============================================================================
