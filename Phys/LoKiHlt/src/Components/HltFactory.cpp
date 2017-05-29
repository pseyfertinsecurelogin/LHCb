// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiKernel/AlgTool.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ToStream.h"
#include "LoKi/AuxFunBase.h"
#include "LoKi/CacheFactory.h"
#include "LoKi/Functor.h"
#include "LoKi/HLTTypes.h"
#include "LoKi/HltLock.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/HybridBase.h"
#include "LoKi/IHltAntiFactory.h"
#include "LoKi/IHltFactory.h"
#include "LoKi/L0Types.h"
#include "LoKi/OdinTypes.h"

class IInterface;

// ============================================================================
// Local
// ============================================================================
/** @file
 *
 *  definitione and the implementation file for class LoKi::Hybrid::HltFactory
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
    /** @class HltFactory
     *
     *  Concrete impelmentation of LoKi::Hybrid::IHltFActory interface
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
    class HltFactory
      : public         LoKi::Hybrid::Base
      , public virtual LoKi::Hybrid::IHltFactory
      , public virtual LoKi::Hybrid::IHltAntiFactory
    {
      // ======================================================================
      // friend factory forn instantiation
      friend struct ToolFactory<LoKi::Hybrid::HltFactory> ;
      // ======================================================================
    public:
      // ======================================================================
      /// finalization   of the tool
      StatusCode finalize   ()  override;
      // ======================================================================
    public: // L0-functors
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&   pycode  ,
        LoKi::Types::L0_Cut& cuts    ,
        const std::string&   context ) override
      { return _get ( pycode , m_l0_cuts  , cuts , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&   pycode  ,
        LoKi::Types::L0_Fun& cuts    ,
        const std::string&   context ) override
      { return _get ( pycode , m_l0_func , cuts , context ) ; }
      // ======================================================================
    public: // ODIN-functors
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&   pycode  ,
        LoKi::Types::ODIN_Cut& cuts    ,
        const std::string&   context ) override
      { return _get ( pycode , m_o1_cuts  , cuts , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&   pycode  ,
        LoKi::Types::ODIN_Fun& cuts    ,
        const std::string&   context ) override
      { return _get ( pycode , m_o1_func , cuts , context ) ; }
      // ======================================================================
    public: // HLT-functors
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&    pycode  ,
        LoKi::Types::HLT_Cut& cuts    ,
        const std::string&    context ) override
      { return _get ( pycode , m_hlt_cuts  , cuts , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&    pycode  ,
        LoKi::Types::HLT_Fun& cuts    ,
        const std::string&    context ) override
      { return _get ( pycode , m_hlt_func , cuts , context ) ; }
      // ======================================================================
    public: // L0-functors
      // ======================================================================
      /// set the C++ predicate
      void set ( const LoKi::Types::L0_Cuts& cut ) override
      { LoKi::Hybrid::Base::_set ( m_l0_cuts , cut ) ; }
      /// set the C++ function
      void set ( const LoKi::Types::L0_Func& cut ) override
      { LoKi::Hybrid::Base::_set ( m_l0_func , cut ) ; }
      // ======================================================================
    public: // O1-functors
      // ======================================================================
      /// set the C++ predicate
      void set ( const LoKi::Types::ODIN_Cuts& cut ) override
      { LoKi::Hybrid::Base::_set ( m_o1_cuts , cut ) ; }
      /// set the C++ function
      void set ( const LoKi::Types::ODIN_Func& cut ) override
      { LoKi::Hybrid::Base::_set ( m_o1_func , cut ) ; }
      // ======================================================================
    public: // HLT-functors
      // ======================================================================
      /// set the C++ predicate
      void set ( const LoKi::Types::HLT_Cuts& cut ) override
      { LoKi::Hybrid::Base::_set ( m_hlt_cuts , cut ) ; }
      /// set the C++ function
      void set ( const LoKi::Types::HLT_Func& cut ) override
      { LoKi::Hybrid::Base::_set ( m_hlt_func , cut ) ; }
      // ======================================================================
    protected:
      // ======================================================================
      /// constrcutor
      HltFactory
      ( const std::string& type   ,
        const std::string& name   ,
        const IInterface*  parent ) ;
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled
      HltFactory () ;                    // the default constructor is disabled
      // the copy constructor is disabled
      HltFactory ( const HltFactory& )  ;   // the copy constructor is disabled
      // the assignement operator  is disabled
      HltFactory& operator=( const HltFactory& )  ;           // no assignement
      // ======================================================================
    private:
      // ======================================================================
      /// helper method to sdave many lines:
      template <class TYPE1,class TYPE2>
      inline StatusCode _get
      ( const std::string& pycode  ,
        std::unique_ptr<TYPE1>& local   ,
        TYPE2&             output  ,
        const std::string& context ) ;
      // ======================================================================
    private:
      // ======================================================================
      /// L0-functors :
      std::unique_ptr<LoKi::Types::L0_Cuts>    m_l0_cuts  ;        // predicate
      std::unique_ptr<LoKi::Types::L0_Func>    m_l0_func  ;        //  function
      /// ODIN-functors :
      std::unique_ptr<LoKi::Types::ODIN_Cuts>  m_o1_cuts  ;        // predicate
      std::unique_ptr<LoKi::Types::ODIN_Func>  m_o1_func  ;        //  function
      /// HLT-functors :
      std::unique_ptr<LoKi::Types::HLT_Cuts>   m_hlt_cuts ;        // predicate
      std::unique_ptr<LoKi::Types::HLT_Func>   m_hlt_func ;        //  function
      // ======================================================================
      typedef std::vector<std::string> Modules ;
      Modules     m_modules ;
      std::string m_actor   =  "LoKi.Hybrid.HltEngine()" ;
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
inline StatusCode LoKi::Hybrid::HltFactory::_get
( const std::string& pycode  ,
  std::unique_ptr<TYPE1>& local   ,
  TYPE2&             output  ,
  const std::string& context )
{
  // prepare the actual python code
  std::string code =
    makeCode  ( m_modules , m_actor , pycode , m_lines , context ) ;
  // define and lock the scope:
  LoKi::Hybrid::HltLock lock ( this ) ;   ///< ATTENTION: the scope is locked!!
  //
  // use the base class method
  StatusCode sc = LoKi::Hybrid::Base::_get_ ( code , local , output ) ;
  if ( sc.isFailure() )
  { return Error ( "Invalid object for the code '" + pycode + "' (hash: " +
    std::to_string(LoKi::Cache::makeHash(code)) + ")" ) ; } // RETURN
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
DECLARE_NAMESPACE_TOOL_FACTORY(LoKi::Hybrid,HltFactory)
// ============================================================================
// Standard constructor
// ============================================================================
LoKi::Hybrid::HltFactory::HltFactory
( const std::string& type   ,
  const std::string& name   ,
  const IInterface*  parent )
  : LoKi::Hybrid::Base ( type , name , parent )
{
  //
  declareInterface<LoKi::Hybrid::IHltFactory>     ( this ) ;
  declareInterface<LoKi::Hybrid::IHltAntiFactory> ( this ) ;
  //
  m_modules.push_back ( "LoKiHlt.decorators"   ) ;
  //
  declareProperty ( "Modules" , m_modules ,
                    "Python modules to be imported"          ) ;
  declareProperty ( "Actor"   , m_actor   ,
                    "The processing engine"                  ) ;
  declareProperty ( "Lines"   , m_lines   ,
                    "Additional Python lines to be executed" ) ;

  //
  //
  // Treat C++
  //
  m_cpplines.push_back ( "#include \"LoKi/LoKiHlt.h\""            ) ;
  m_cpplines.push_back ( "#include \"LoKi/LoKiNumbers.h\""        ) ;
  //
}
// ============================================================================
// finalization of the tool
// ============================================================================
StatusCode LoKi::Hybrid::HltFactory::finalize  ()
{
  // functors
  m_l0_cuts.reset();
  m_l0_func.reset();
  m_o1_cuts.reset();
  m_o1_func.reset();
  m_hlt_cuts.reset();
  m_hlt_func.reset();
  // finalize the base
  return LoKi::Hybrid::Base::finalize() ;
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
