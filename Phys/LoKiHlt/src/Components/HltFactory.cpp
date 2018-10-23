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
// GaudiKernel
// ============================================================================
#include "GaudiKernel/System.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/HybridBase.h"
#include "LoKi/HltLock.h"
#include "LoKi/IHltFactory.h"
#include "LoKi/IHltAntiFactory.h"
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
      /// constrcutor
      HltFactory
      ( const std::string& type   ,
        const std::string& name   ,
        const IInterface*  parent ) ;
      // ======================================================================
    private:
      // ======================================================================
      /// helper method to sdave many lines:
      template <class TYPE1,class TYPE2>
      inline StatusCode _get
      ( const std::string&                                pycode  ,
        std::unique_ptr<LoKi::Functor<TYPE1,TYPE2>>&      local   ,
        LoKi::Assignable_t<LoKi::Functor<TYPE1,TYPE2>>&   output  ,
        const std::string&                                context ) ;
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
( const std::string&                                pycode  ,
  std::unique_ptr<LoKi::Functor<TYPE1,TYPE2>>&      local   ,
  LoKi::Assignable_t<LoKi::Functor<TYPE1,TYPE2>>&   output  ,
  const std::string&                                context )
{
  std::lock_guard guard ( m_mutex );
  // prepare the actual python code
  std::string code = makeCode ( m_modules , m_actor , pycode , m_lines , context ) ;
  // define and lock the scope:
  LoKi::Hybrid::HltLock lock  ( this , make_context () ) ;
  // use the base class method
  StatusCode sc = LoKi::Hybrid::Base::_get_ ( code , local , output ) ;
  if ( sc.isFailure() )
  { return Error ( "Invalid object for the code '" + pycode + "' (hash: " +
                   std::to_string(LoKi::Cache::makeHash(code)) + ")" ) ; } // RETURN
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
DECLARE_COMPONENT( LoKi::Hybrid::HltFactory )
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
