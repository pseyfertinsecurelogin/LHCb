// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/System.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/HybridBase.h"
#include "LoKi/CoreLock.h"
#include "LoKi/ICoreFactory.h"
#include "LoKi/ICoreAntiFactory.h"
// ============================================================================
// Local
// ============================================================================
/** @file
 *
 *  definition and the implementation file for class LoKi::Hybrid::CoreFactory
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2008-09-18
 */
namespace LoKi
{
  // ==========================================================================
  namespace Hybrid
  {
    // ========================================================================
    /** @class CoreFactory
     *
     *  Concrete impelmentation of LoKi::Hybrid::ICoreFactory interface
     *
     *  This file is a part of LoKi project -
     *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
     *
     *  The package has been designed with the kind help from
     *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
     *  contributions and advices from G.Raven, J.van Tilburg,
     *  A.Golutvin, P.Koppenburg have been used in the design.
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-18
     */
    class CoreFactory
      : public         LoKi::Hybrid::Base
      , public virtual LoKi::Hybrid::ICoreFactory
      , public virtual LoKi::Hybrid::ICoreAntiFactory
    {
      // ======================================================================
      // friend factory forn instantiation
      friend struct ToolFactory<LoKi::Hybrid::CoreFactory> ;
      // ======================================================================
    public:
      // ======================================================================
      /// finalization   of the tool
      StatusCode finalize   () override;
      // ======================================================================
    public: // (void->...)
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context context code fragment to be executed
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&   pycode       ,
        LoKi::Types::FCut&   cuts         ,
        const std::string&   context = "" ) override
      { return _get ( pycode , m_fcut  , cuts , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context context code fragment to be executed
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&   pycode       ,
        LoKi::Types::FFun&   cuts         ,
        const std::string&   context = "" ) override
      { return _get ( pycode , m_ffun  , cuts , context ) ; }
      // ======================================================================
    public: // (double->...)
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context context code fragment to be executed
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&   pycode       ,
        LoKi::Types::XCut&   cuts         ,
        const std::string&   context = "" ) override
      { return _get ( pycode , m_xcut  , cuts , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context context code fragment to be executed
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&   pycode       ,
        LoKi::Types::XFun&   cuts         ,
        const std::string&   context = "" ) override
      { return _get ( pycode , m_xfun  , cuts , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context context code fragment to be executed
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&  pycode ,
        LoKi::Types::XMap&  cuts  , // vector<double> -> vector<double>
        const std::string&  context = "" ) override
      { return _get ( pycode , m_xmap  , cuts , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context context code fragment to be executed
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&    pycode       ,
        LoKi::Types::XFunVal& cuts         , // vector<double> -> double
        const std::string&    context = "" ) override
      { return _get ( pycode , m_xfunval  , cuts , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context context code fragment to be executed
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&    pycode       ,
        LoKi::Types::XSource& cuts         ,  // void -> vector<double>
        const std::string&    context = "" ) override
      { return _get ( pycode , m_xsource  , cuts , context ) ; }
      // ======================================================================
    public: // "void -> ... "
      // ======================================================================
      /// set the C++ predicate for 'void'
      void set ( const LoKi::Types::FCuts& cut ) override
      { LoKi::Hybrid::Base::_set ( m_fcut , cut ) ; }
      /// set the C++ function  for 'void'
      void set ( const LoKi::Types::FFunc& cut ) override
      { LoKi::Hybrid::Base::_set ( m_ffun , cut ) ; }
      // ======================================================================
    public: // "double -> ... "
      // ======================================================================
      /// set the C++ predicate for 'double'
      void set ( const LoKi::Types::XCuts& cut ) override
      { LoKi::Hybrid::Base::_set ( m_xcut , cut ) ; }
      /// set the C++ function  for 'double'
      void set ( const LoKi::Types::XFunc& cut ) override
      { LoKi::Hybrid::Base::_set ( m_xfun , cut ) ; }
      // ======================================================================
    public: // some functional stuff:
      // ======================================================================
      /// set: vector<double> -> vector<double>
      void set ( const LoKi::Types::XMaps&    cut ) override
      { LoKi::Hybrid::Base::_set ( m_xmap    , cut ) ; }
      /// set: vector<double> ->        double
      void set ( const LoKi::Types::XFunVals& cut ) override
      { LoKi::Hybrid::Base::_set ( m_xfunval , cut ) ; }
      /// set: void  -> vector<double>
      void set ( const LoKi::Types::XSources& cut ) override
      { LoKi::Hybrid::Base::_set ( m_xsource , cut ) ; }
      // ======================================================================
    protected:
      // ======================================================================
      /// constrcutor
      CoreFactory
      ( const std::string& type   ,
        const std::string& name   ,
        const IInterface*  parent ) ;
      // ======================================================================
      // the copy constructor is disabled
      CoreFactory ( const CoreFactory& ) = delete;  // the copy constructor is disabled
      // the assignement operator  is disabled
      CoreFactory& operator=( const CoreFactory& ) = delete;         // no assignement
      // ======================================================================
    private:
      // ======================================================================
      /// helper method to save many lines:
      template <class TYPE1,class TYPE2>
      inline StatusCode _get
      ( const std::string&                                            pycode  ,
        std::unique_ptr<LoKi::Functor<TYPE1,TYPE2>>&                  local   ,
        typename LoKi::Assignable<LoKi::Functor<TYPE1,TYPE2> >::Type& output  ,
        const std::string&                                            context ) ;
      // ======================================================================
    private:
      // ======================================================================
      /// "void   -> ..."
      std::unique_ptr<LoKi::Types::FCuts>    m_fcut ;                     // predicate
      std::unique_ptr<LoKi::Types::FFunc>    m_ffun ;                     //  function
      /// "double -> ... "
      std::unique_ptr<LoKi::Types::XCuts>    m_xcut ;                     // predicate
      std::unique_ptr<LoKi::Types::XFunc>    m_xfun ;                     //  function
      /// functional
      std::unique_ptr<LoKi::Types::XMaps>    m_xmap ;                       // map/pipe
      std::unique_ptr<LoKi::Types::XFunVals> m_xfunval ;              // funval/element
      std::unique_ptr<LoKi::Types::XSources> m_xsource ;                      // source
      // ======================================================================
      typedef std::vector<std::string> Modules ;
      Modules     m_modules ;
      std::string m_actor   =  "LoKi.Hybrid.CoreEngine()" ;
      typedef std::vector<std::string> Lines   ;
      Lines       m_lines   ;
      // ======================================================================
    } ;
    // ========================================================================
  } // end of namespace LoKi::Hybrid
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// helper method to sdave many lines:
// ============================================================================
template <class TYPE1,class TYPE2>
inline StatusCode LoKi::Hybrid::CoreFactory::_get
( const std::string& pycode  ,
  std::unique_ptr<LoKi::Functor<TYPE1,TYPE2>>&                  local   ,
  typename LoKi::Assignable<LoKi::Functor<TYPE1,TYPE2> >::Type& output  ,
  const std::string& context )
{
  // prepare the actual python code
  std::string code =
    makeCode  ( m_modules , m_actor , pycode , m_lines , context ) ;
  /// define and lock the scope:
  LoKi::Hybrid::CoreLock lock ( this ) ;   // ATTENTION: the scope is locked!!
  // use the base class method
  StatusCode sc = LoKi::Hybrid::Base::_get_ ( code , local , output ) ;
  if ( sc.isFailure() )
  { return Error ( "Invalid object for the code '" + pycode + "' (hash: " +
    std::to_string(LoKi::Cache::makeHash(code)) + ")" ) ; } // RETURN
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
DECLARE_NAMESPACE_TOOL_FACTORY(LoKi::Hybrid,CoreFactory)
// ============================================================================
// Standard constructor
// ============================================================================
LoKi::Hybrid::CoreFactory::CoreFactory
( const std::string& type   ,
  const std::string& name   ,
  const IInterface*  parent )
  : LoKi::Hybrid::Base ( type , name , parent )
{
  //
  declareInterface<LoKi::Hybrid::ICoreFactory>     ( this ) ;
  declareInterface<LoKi::Hybrid::ICoreAntiFactory> ( this ) ;
  //
  m_modules.push_back ( "LoKiNumbers.decorators"   ) ;
  //
  declareProperty
    ( "Modules" , m_modules ,
      "Python modules to be imported"          ) ;
  declareProperty
    ( "Actor"   , m_actor   ,
      "The processing engine"                  ) ;
  declareProperty
    ( "Lines"   , m_lines   ,
      "Additional Python lines to be executed" ) ;
  // ==========================================================================
  // C++
  // ==========================================================================
  m_cpplines.push_back ( "#include \"LoKi/LoKiCore.h\""            ) ;
  m_cpplines.push_back ( "#include \"LoKi/LoKiNumbers.h\""         ) ;
  //
  // For Trigger
  if ( std::string::npos != name.find ("Hlt" )  ||
       std::string::npos != name.find ("Trg" )  ||
       std::string::npos != name.find ("HLT" )  ||
       std::string::npos != name.find ("TRG" )  ||
       std::string::npos != name.find ("Trig")  ||
       std::string::npos != name.find ("TRIG") )
  {
    m_cpplines.push_back ( "#include \"LoKi/LoKiTrack.h\""         ) ;
    m_cpplines.push_back ( "#include \"LoKi/LoKiProtoParticles.h\"" ) ;
    m_cpplines.push_back ( "#include \"LoKi/LoKiPhys.h\""           ) ;
    m_cpplines.push_back ( "#include \"LoKi/LoKiArrayFunctors.h\""  ) ;
    m_cpplines.push_back ( "#include \"LoKi/LoKiTrigger.h\""        ) ;
  }
  // ==========================================================================
}
// ============================================================================
// finalization of the tool
// ============================================================================
StatusCode LoKi::Hybrid::CoreFactory::finalize  ()
{
  // functors
  m_fcut.reset();
  m_ffun.reset();
  m_xcut.reset();
  m_xfun.reset();
  m_xmap.reset();
  m_xfunval.reset();
  m_xsource.reset();
  // finalize the base
  return LoKi::Hybrid::Base::finalize() ;
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
