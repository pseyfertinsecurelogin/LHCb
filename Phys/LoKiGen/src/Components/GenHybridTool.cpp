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
// STD&STL
// ============================================================================
#include <mutex>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/System.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/HybridBase.h"
#include "LoKi/GenHybridLock.h"
#include "LoKi/IGenHybridTool.h"
#include "LoKi/IGenHybridFactory.h"
// ============================================================================
// Local
// ============================================================================
/** @file
 *  definitione and the implementation file for class LoKi::Hybrid::GenTool
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
    /** @class GenTool
     *
     *  Concrete impelmentation of LoKi::IGenHybridTool interface
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
    class GenTool
      : public       LoKi::Hybrid::Base
      , public virtual IGenHybridTool
      , public virtual IGenHybridFactory
    {
    public:
      // ======================================================================
      /// initialization of the tool
      StatusCode initialize ()  override;
      /// finalization   of the tool
      StatusCode finalize  ()  override;
      // ======================================================================
    public:
      // ======================================================================
      // predicates
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string& pycode  ,
        LoKi::Types::GCut& cuts    ,
        const std::string& context ) override
      { return _get ( pycode , m_gcuts  , cuts , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param cuts the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&  pycode  ,
        LoKi::Types::GVCut& cuts    ,
        const std::string&  context ) override
      { return _get ( pycode , m_gvcuts  , cuts , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // functions
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string& pycode  ,
        LoKi::Types::GFun& func    ,
        const std::string& context ) override
      { return _get ( pycode , m_gfunc  , func , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&  pycode  ,
        LoKi::Types::GVFun& func    ,
        const std::string&  context ) override
      { return _get ( pycode , m_gvfunc  , func , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // maps
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&  pycode  ,
        LoKi::Types::GMap&  func    ,
        const std::string&  context ) override
      { return _get ( pycode , m_gmap  , func , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&   pycode  ,
        LoKi::Types::GVMap&  func    ,
        const std::string&   context ) override
      { return _get ( pycode , m_gvmap  , func , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // pipes
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&  pycode  ,
        LoKi::Types::GPipe& func    ,
        const std::string&  context ) override
      { return _get ( pycode , m_gpipe  , func , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&   pycode  ,
        LoKi::Types::GVPipe& func    ,
        const std::string&   context ) override
      { return _get ( pycode , m_gvpipe  , func , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // fun-vals
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&    pycode  ,
        LoKi::Types::GFunVal& func    ,
        const std::string&    context ) override
      { return _get ( pycode , m_gfunval  , func , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&     pycode  ,
        LoKi::Types::GVFunVal& func    ,
        const std::string&     context ) override
      { return _get ( pycode , m_gvfunval  , func , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // cut-vals
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&    pycode  ,
        LoKi::Types::GCutVal& func    ,
        const std::string&    context ) override
      { return _get ( pycode , m_gcutval  , func , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // sources
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&     pycode  ,
        LoKi::Types::GSource&  func    ,
        const std::string&     context ) override
      { return _get ( pycode , m_gsource  , func , context ) ; }
      // ======================================================================
      /** "Factory": get the the object form python code
       *  @param pycode the python pseudo-code of the function
       *  @param func the placeholder for the result
       *  @return StatusCode
       */
      StatusCode get
      ( const std::string&      pycode  ,
        LoKi::Types::GVSource&  func    ,
        const std::string&      context ) override
      { return _get ( pycode , m_gvsource , func , context ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // predicates
      // ======================================================================
      /// set the C++ predicate for HepMC::GenParticle
      void set ( const LoKi::Types::GCuts&   cut ) override
      { LoKi::Hybrid::Base::_set ( m_gcuts , cut ) ; }
      /// set the C++ predicate for HepMC::GenVertex
      void set ( const LoKi::Types::GVCuts&   cut ) override
      { LoKi::Hybrid::Base::_set ( m_gvcuts , cut ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // functions
      // ======================================================================
      /// set the C++ function for HepMC::GenParticle
      void set ( const LoKi::Types::GFunc&   cut ) override
      { LoKi::Hybrid::Base::_set ( m_gfunc  , cut ) ; }
      /// set the C++ function for HepMC::GenVertex
      void set ( const LoKi::Types::GVFunc&   cut ) override
      { LoKi::Hybrid::Base::_set ( m_gvfunc  , cut ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // maps
      // ======================================================================
      /// set the C++ "map" for HepMC::GenParticle
      void set ( const LoKi::Types::GMaps&   cut ) override
      { LoKi::Hybrid::Base::_set ( m_gmap  , cut ) ; }
      /// set the C++ "map" for HepMC::GenVertex
      void set ( const LoKi::Types::GVMaps&   cut ) override
      { LoKi::Hybrid::Base::_set ( m_gvmap  , cut ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // pipes
      // ======================================================================
      /// set the C++ "pipe" for HepMC::GenParticle
      void set ( const LoKi::Types::GPipes&   cut ) override
      { LoKi::Hybrid::Base::_set ( m_gpipe  , cut ) ; }
      /// set the C++ "pipe" for HepMC::GenVertex
      void set ( const LoKi::Types::GVPipes&   cut ) override
      { LoKi::Hybrid::Base::_set ( m_gvpipe , cut ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // funvals
      // ======================================================================
      /// set the C++ "fun-val" for HepMC::GenParticle
      void set ( const LoKi::Types::GFunVals&   cut ) override
      { LoKi::Hybrid::Base::_set ( m_gfunval  , cut ) ; }
      /// set the C++ "pipe" for HepMC::GenVertex
      void set ( const LoKi::Types::GVFunVals&   cut ) override
      { LoKi::Hybrid::Base::_set ( m_gvfunval , cut ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // cutvals
      // ======================================================================
      /// set the C++ "cut-val" for HepMC::GenParticle
      void set ( const LoKi::Types::GCutVals&   cut ) override
      { LoKi::Hybrid::Base::_set ( m_gcutval  , cut ) ; }
      // ======================================================================
    public:
      // ======================================================================
      // sources
      // ======================================================================
      /// set the C++ "source" for HepMC::GenParticle
      void set ( const LoKi::Types::GSources&   cut ) override
      { LoKi::Hybrid::Base::_set ( m_gsource  , cut ) ; }
      /// set the C++ "source" for HepMC::GenVertex
      void set ( const LoKi::Types::GVSources&   cut ) override
      { LoKi::Hybrid::Base::_set ( m_gvsource , cut ) ; }
      // ======================================================================
      /// constrcutor
      GenTool
      ( const std::string& type   ,
        const std::string& name   ,
        const IInterface*  parent ) ;
      // ======================================================================
    private:
      // ======================================================================
      /// helper method to save many lines:
      template <class TYPE1,class TYPE2>
      inline StatusCode _get
      ( const std::string&                                pycode  ,
        std::unique_ptr<LoKi::Functor<TYPE1,TYPE2>>&      local   ,
        LoKi::Assignable_t<LoKi::Functor<TYPE1,TYPE2>>&   output  ,
        const std::string&                                context ) ;
      // ======================================================================
    protected:
      // ======================================================================
      //
      //   local holders of cuts
      //
      // predicates:
      std::unique_ptr<LoKi::Types::GCuts>      m_gcuts      ;
      std::unique_ptr<LoKi::Types::GVCuts>     m_gvcuts     ;
      // functions:
      std::unique_ptr<LoKi::Types::GFunc>      m_gfunc      ;
      std::unique_ptr<LoKi::Types::GVFunc>     m_gvfunc     ;
      // maps:
      std::unique_ptr<LoKi::Types::GMaps>      m_gmap       ;
      std::unique_ptr<LoKi::Types::GVMaps>     m_gvmap      ;
      // pipes:
      std::unique_ptr<LoKi::Types::GPipes>     m_gpipe      ;
      std::unique_ptr<LoKi::Types::GVPipes>    m_gvpipe     ;
      // fun-vals:
      std::unique_ptr<LoKi::Types::GFunVals>   m_gfunval    ;
      std::unique_ptr<LoKi::Types::GVFunVals>  m_gvfunval   ;
      // cut-vals:
      std::unique_ptr<LoKi::Types::GCutVals>   m_gcutval    ;
      // sources:
      std::unique_ptr<LoKi::Types::GSources>    m_gsource   ;
      std::unique_ptr<LoKi::Types::GVSources>   m_gvsource  ;
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
inline StatusCode LoKi::Hybrid::GenTool::_get
( const std::string&                                pycode  ,
  std::unique_ptr<LoKi::Functor<TYPE1,TYPE2>>&      local   ,
  LoKi::Assignable_t<LoKi::Functor<TYPE1,TYPE2>>&   output  ,
  const std::string&                                context )
{
  std::lock_guard guard ( m_mutex );
  // prepare the actual python code
  std::string code = makeCode  ( m_modules , m_actor , pycode , m_lines , context ) ;
  /// define and lock the scope:
  LoKi::Hybrid::GenLock lock ( this , make_context() ) ;
  // use the base class method
  StatusCode sc = LoKi::Hybrid::Base::_get_ ( code , local , output ) ;
  if ( sc.isFailure() )
  { return Error ( "Invalid object for the code '" + pycode + "' (hash: " +
                   std::to_string( LoKi::Cache::makeHash ( code ) ) + ")" ) ; } // RETURN
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
DECLARE_COMPONENT( LoKi::Hybrid::GenTool )
// ============================================================================
// Standard constructor
// ============================================================================
LoKi::Hybrid::GenTool::GenTool
( const std::string& type   ,
  const std::string& name   ,
  const IInterface*  parent )
  : LoKi::Hybrid::Base ( type , name , parent )
  , m_actor     ( "LoKi.Hybrid.GenEngine()" )
{
  //
  declareInterface<LoKi::IGenHybridTool>   ( this ) ;
  declareInterface<LoKi::IGenHybridFactory>( this ) ;
  //
  m_modules.push_back ( "LoKiGen.decorators"   ) ;
  // m_modules.push_back ( "LoKiGenMC.decorators" ) ;
  m_modules.push_back ( "LoKiCore.functions"   ) ;
  //
  declareProperty ( "Modules" , m_modules , "Python modules to be imported"          ) ;
  declareProperty ( "Actor"   , m_actor   , "The processing engine"                  ) ;
  declareProperty ( "Lines"   , m_lines   , "Additional Python lines to be executed" ) ;
}
// ============================================================================
// initialization of the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenTool::initialize ()
{
  StatusCode sc = LoKi::Hybrid::Base::initialize() ;
  if ( sc.isFailure() ) { return sc ; }
  if ( msgLevel ( MSG::DEBUG ) ) { m_lines.insert( m_lines.begin() , "dir()") ; }
  return StatusCode::SUCCESS ;
}
// ============================================================================
// finalization of the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenTool::finalize  ()
{
  // predicates:
  m_gcuts.reset();
  m_gvcuts.reset();
  // functions:
  m_gfunc.reset();
  m_gvfunc.reset();
  // maps:
  m_gmap.reset();
  m_gvmap.reset();
  // pipes:
  m_gpipe.reset();
  m_gvpipe.reset();
  // funvals:
  m_gfunval.reset();
  m_gvfunval.reset();
  // cutvals:
  m_gcutval.reset();
  // sources:
  m_gsource.reset();
  m_gvsource.reset();
  //
  // finalize the base
  return LoKi::Hybrid::Base::finalize() ;
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
