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
#include "LoKi/MCHybridLock.h"
#include "LoKi/IMCHybridTool.h"
#include "LoKi/IMCHybridFactory.h"
// ============================================================================
// Local
// ============================================================================
/** @file
 *  definitione and the implementation file for class LoKi::HybridTool
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2004-06-29
 */
namespace LoKi
{
  // ==========================================================================
  namespace Hybrid
  {
    // ========================================================================
    /** @class MCTool
     *
     *  Concrete impelmentation of LoKi::IMCHybridTool interface
     *  This file is a part of LoKi project -
     *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
     *
     *  The package has been designed with the kind help from
     *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
     *  contributions and advices from G.Raven, J.van Tilburg,
     *  A.Golutvin, P.Koppenburg have been used in the design.
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2004-06-29
     */
    class MCTool
      : public LoKi::Hybrid::Base
      , public virtual IMCHybridTool
      , public virtual IMCHybridFactory
    {
      // ======================================================================
      // friend factory forn instantiation
      friend struct ToolFactory<LoKi::Hybrid::MCTool> ;
      // ======================================================================
    public:
      // ======================================================================
      /// initialization of the tool
      virtual StatusCode initialize () ;
      /// finalization   of the tool
      virtual StatusCode finalize  () ;
      // ======================================================================
    public:
      // ======================================================================
      // predicates:
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context the code fragment to be pre-executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string& pycode    ,
        LoKi::Types::MCCut&  cuts    ,
        const std::string&   context )
      { return _get ( pycode , m_mccuts  , cuts , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @param context the code fragment to be pre-executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&   pycode  ,
        LoKi::Types::MCVCut& cuts    ,
        const std::string&   context )
      { return _get ( pycode , m_mcvcuts  , cuts , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // functions:
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @param context the code fragment to be pre-executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&  pycode  ,
        LoKi::Types::MCFun& func    ,
        const std::string&  context )
      { return _get ( pycode , m_mcfunc  , func , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @param context the code fragment to be pre-executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&   pycode  ,
        LoKi::Types::MCVFun& func    ,
        const std::string&   context )
      { return _get ( pycode , m_mcvfunc  , func , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // maps:
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @param context the code fragment to be pre-executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&  pycode  ,
        LoKi::Types::MCMap& func    ,
        const std::string&  context )
      { return _get ( pycode , m_mcmaps  , func , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @param context the code fragment to be pre-executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&   pycode  ,
        LoKi::Types::MCVMap& func    ,
        const std::string&   context )
      { return _get ( pycode , m_mcvmaps  , func , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // pipes:
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @param context the code fragment to be pre-executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&   pycode  ,
        LoKi::Types::MCPipe& func    ,
        const std::string&   context )
      { return _get ( pycode , m_mcpipe  , func , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @param context the code fragment to be pre-executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&    pycode  ,
        LoKi::Types::MCVPipe& func    ,
        const std::string&    context )
      { return _get ( pycode , m_mcvpipe  , func , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // fun-vals:
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @param context the code fragment to be pre-executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&     pycode  ,
        LoKi::Types::MCFunVal& func    ,
        const std::string&     context )
      { return _get ( pycode , m_mcfunvals , func , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @param context the code fragment to be pre-executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&      pycode  ,
        LoKi::Types::MCVFunVal& func    ,
        const std::string&      context )
      { return _get ( pycode , m_mcvfunvals  , func , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // cut-vals:
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @param context the code fragment to be pre-executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&     pycode  ,
        LoKi::Types::MCCutVal& func    ,
        const std::string&     context )
      { return _get ( pycode , m_mccutvals , func , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @param context the code fragment to be pre-executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&      pycode  ,
        LoKi::Types::MCVCutVal& func    ,
        const std::string&      context )
      { return _get ( pycode , m_mcvcutvals , func , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // sources:
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @param context the code fragment to be pre-executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&      pycode  ,
        LoKi::Types::MCSource&  func    ,
        const std::string&      context )
      { return _get ( pycode , m_mcsource , func , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @param context the code fragment to be pre-executed
       *  @return StatusCode
       */
      virtual StatusCode get
      ( const std::string&       pycode  ,
        LoKi::Types::MCVSource&  func    ,
        const std::string&       context )
      { return _get ( pycode , m_mcvsource , func , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // predicates:
      // ======================================================================
      /// set the C++ predicate for LHCb::MCParticle
      virtual void set ( const LoKi::Types::MCCuts&    cut )
      { LoKi::Hybrid::Base::_set ( m_mccuts  , cut ) ; }
      /// set the C++ predicate for LHCb::MCVertex
      virtual void set ( const LoKi::Types::MCVCuts&   cut )
      { LoKi::Hybrid::Base::_set ( m_mcvcuts , cut ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // functions:
      // ======================================================================
      /// set the C++ function for LHCb::MCParticle
      virtual void set ( const LoKi::Types::MCFunc&    cut )
      { LoKi::Hybrid::Base::_set ( m_mcfunc  , cut ) ; }
      /// set the C++ function for LHCb::MCVertex
      virtual void set ( const LoKi::Types::MCVFunc&   cut )
      { LoKi::Hybrid::Base::_set ( m_mcvfunc  , cut ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // maps:
      // ======================================================================
      /// set the C++ map for LHCb::MCParticle
      virtual void set ( const LoKi::Types::MCMaps&    cut )
      { LoKi::Hybrid::Base::_set ( m_mcmaps  , cut ) ; }
      /// set the C++ map for LHCb::MCVertex
      virtual void set ( const LoKi::Types::MCVMaps&   cut )
      { LoKi::Hybrid::Base::_set ( m_mcvmaps  , cut ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // pipes:
      // ======================================================================
      /// set the C++ pipe for LHCb::MCParticle
      virtual void set ( const LoKi::Types::MCPipes&    cut )
      { LoKi::Hybrid::Base::_set ( m_mcpipe  , cut ) ; }
      /// set the C++ pipe for LHCb::MCVertex
      virtual void set ( const LoKi::Types::MCVPipes&   cut )
      { LoKi::Hybrid::Base::_set ( m_mcvpipe  , cut ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // fun-vals:
      // ======================================================================
      /// set the C++ fun-val for LHCb::MCParticle
      virtual void set ( const LoKi::Types::MCFunVals&    cut )
      { LoKi::Hybrid::Base::_set ( m_mcfunvals  , cut ) ; }
      /// set the C++ fun-val for LHCb::MCVertex
      virtual void set ( const LoKi::Types::MCVFunVals&   cut )
      { LoKi::Hybrid::Base::_set ( m_mcvfunvals  , cut ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // cut-vals:
      // ======================================================================
      /// set the C++ fun-val for LHCb::MCParticle
      virtual void set ( const LoKi::Types::MCCutVals&    cut )
      { LoKi::Hybrid::Base::_set ( m_mccutvals  , cut ) ; }
      /// set the C++ fun-val for LHCb::MCVertex
      virtual void set ( const LoKi::Types::MCVCutVals&   cut )
      { LoKi::Hybrid::Base::_set ( m_mcvcutvals , cut ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // sources
      // ======================================================================
      /// set the C++ source for LHCb::MCParticle
      virtual void set ( const LoKi::Types::MCSources&    cut )
      { LoKi::Hybrid::Base::_set ( m_mcsource  , cut ) ; }
      /// set the C++ source  for LHCb::MCVertex
      virtual void set ( const LoKi::Types::MCVSources&   cut )
      { LoKi::Hybrid::Base::_set ( m_mcvsource , cut ) ; }
      // ======================================================================
    protected:
      // ======================================================================
      /// constrcutor
      MCTool
      ( const std::string& type   ,
        const std::string& name   ,
        const IInterface*  parent ) ;
      /// destrcutor
      virtual ~MCTool();
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled
      MCTool () ;                        // the default constructor is disabled
      /// the copy constructor is disabled
      MCTool( const MCTool& )  ;            // the copy constructor is disabled
      /// the assignement operator  is disabled
      MCTool& operator=( const MCTool& )  ;                   // no assignement
      // ======================================================================
    private:
      // ======================================================================
      /// helper method to sdave many lines:
      template <class TYPE1,class TYPE2>
      inline StatusCode _get
      ( const std::string&                                            pycode  ,
        std::unique_ptr<LoKi::Functor<TYPE1,TYPE2>>&                  local   ,
        typename LoKi::Assignable<LoKi::Functor<TYPE1,TYPE2> >::Type& output  ,
        const std::string&                                            context ) ;
      // ======================================================================
    protected:
      // ======================================================================
      // local holder of cuts
      // predicates:
      std::unique_ptr<LoKi::Types::MCCuts>      m_mccuts     ;
      std::unique_ptr<LoKi::Types::MCVCuts>     m_mcvcuts    ;
      // functions:
      std::unique_ptr<LoKi::Types::MCFunc>      m_mcfunc     ;
      std::unique_ptr<LoKi::Types::MCVFunc>     m_mcvfunc    ;
      // maps:
      std::unique_ptr<LoKi::Types::MCMaps>      m_mcmaps     ;
      std::unique_ptr<LoKi::Types::MCVMaps>     m_mcvmaps    ;
      // pipes:
      std::unique_ptr<LoKi::Types::MCPipes>     m_mcpipe     ;
      std::unique_ptr<LoKi::Types::MCVPipes>    m_mcvpipe    ;
      // funvals:
      std::unique_ptr<LoKi::Types::MCFunVals>   m_mcfunvals  ;
      std::unique_ptr<LoKi::Types::MCVFunVals>  m_mcvfunvals ;
      // cutvals:
      std::unique_ptr<LoKi::Types::MCCutVals>   m_mccutvals  ;
      std::unique_ptr<LoKi::Types::MCVCutVals>  m_mcvcutvals ;
      // sources:
      std::unique_ptr<LoKi::Types::MCSources>   m_mcsource   ;
      std::unique_ptr<LoKi::Types::MCVSources>  m_mcvsource  ;
      //
      typedef std::vector<std::string> Modules ;
      Modules     m_modules ;
      std::string m_actor   ;
      typedef std::vector<std::string> Lines   ;
      Lines       m_lines   ;
      // ======================================================================
    } ;
    // ========================================================================
  } //                                            end of namespace LoKi::Hybrid
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// helper method to sdave many lines:
// ============================================================================
template <class TYPE1,class TYPE2>
inline StatusCode
LoKi::Hybrid::MCTool::_get
( const std::string&                                            pycode  ,
  std::unique_ptr<LoKi::Functor<TYPE1,TYPE2>>&                  local   ,
  typename LoKi::Assignable<LoKi::Functor<TYPE1,TYPE2> >::Type& output  ,
  const std::string&                                            context )
{
  // prepare the actual python code
  std::string code = makeCode
    ( m_modules , m_actor , pycode , m_lines , context ) ;
  // define and lock the scope:
  LoKi::Hybrid::MCLock lock ( this ) ;      // ATTENTION: the scope is locked!!
  //
  // use the base class method
  StatusCode sc = LoKi::Hybrid::Base::_get_ ( code , local , output ) ;
  if ( sc.isFailure() )
  { return Error ( "Invalid object for the code '" + pycode + "'"    ) ; } // RETURN
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
DECLARE_NAMESPACE_TOOL_FACTORY(LoKi::Hybrid,MCTool)
// ============================================================================
// Standard constructor
// ============================================================================
LoKi::Hybrid::MCTool::MCTool
( const std::string& type   ,
  const std::string& name   ,
  const IInterface*  parent )
  : LoKi::Hybrid::Base ( type , name , parent )
  , m_actor     ( "LoKi.Hybrid.MCEngine()" )
{
  //
  declareInterface<LoKi::IMCHybridTool>   ( this ) ;
  declareInterface<LoKi::IMCHybridFactory>( this ) ;
  //
  m_modules.push_back ( "LoKiMC.decorators"    ) ;
  m_modules.push_back ( "LoKiGenMC.decorators" ) ;
  declareProperty ( "Modules" , m_modules , "Python modules to be imported"          ) ;
  declareProperty ( "Actor"   , m_actor   , "The processing engine"                  ) ;
  declareProperty ( "Lines"   , m_lines   , "Additional Python lines to be executed" ) ;
}
// ============================================================================
/// Destructor (virtual and protected)
// ============================================================================
LoKi::Hybrid::MCTool::~MCTool() {}
// ============================================================================
// initialization of the tool
// ============================================================================
StatusCode LoKi::Hybrid::MCTool::initialize ()
{
  StatusCode sc = LoKi::Hybrid::Base::initialize() ;
  if ( sc.isFailure() ) { return sc ; }
  if ( msgLevel ( MSG::DEBUG ) ) { m_lines.insert( m_lines.begin() , "dir()") ; }
  return StatusCode::SUCCESS ;
}
// ============================================================================
// finalization of the tool
// ============================================================================
StatusCode LoKi::Hybrid::MCTool::finalize  ()
{
  // predicates:
  m_mccuts.reset();
  m_mcvcuts.reset();
  // functions:
  m_mcfunc.reset();
  m_mcvfunc.reset();
  // maps:
  m_mcmaps.reset();
  m_mcvmaps.reset();
  // pipes:
  m_mcpipe.reset();
  m_mcvpipe.reset();
  // funvals:
  m_mcfunvals.reset();
  m_mcvfunvals.reset();
  // cutvals:
  m_mccutvals.reset();
  m_mcvcutvals.reset();
  // sources:
  m_mcsource.reset();
  m_mcvsource.reset();
  //
  // finalize the base
  return LoKi::Hybrid::Base::finalize() ;
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
