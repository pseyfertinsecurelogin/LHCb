// $Id: GenHybridEngine.cpp 155778 2013-04-29 10:03:16Z cattanem $
// ============================================================================
// Include files
// ============================================================================
// local
// ============================================================================
#include "LoKi/GenHybridEngine.h"
#include "LoKi/GenHybridEngineActor.h"
// ============================================================================
/** @file 
 *  Implementation file for class LoKi::Hybrid::GenEngine
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
 *  @date 2004-06-30 
 *
 *                    $Revision: 155778 $
 *  Last modification $Date: 2013-04-29 12:03:16 +0200 (Mon, 29 Apr 2013) $
 *                 by $Author: cattanem $
 */
// ============================================================================
// constructor
// ============================================================================
LoKi::Hybrid::GenEngine::GenEngine() {}
// ============================================================================
// destructor 
// ============================================================================
LoKi::Hybrid::GenEngine::~GenEngine() {}
// ============================================================================
namespace 
{
  template <class TYPE> 
  inline StatusCode _process 
  ( const std::string& name , const TYPE& cut )
  {
    LoKi::Hybrid::GenEngineActor& actor = LoKi::Hybrid::GenEngineActor::instance() ;
    return actor.process ( name , cut ) ;
  }
}
// ============================================================================
// predicates 
// ============================================================================
// add the cut 
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process
( const std::string&          name , 
  const LoKi::Types::GCuts&   cut  ) const { return _process ( name , cut ) ; }
// ============================================================================
// add the cut 
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process
( const std::string&          name , 
  const LoKi::Types::GVCuts&  cut  ) const { return _process ( name , cut ) ; }
// ============================================================================
// functions 
// ============================================================================
// add the function 
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process
( const std::string&          name , 
  const LoKi::Types::GFunc&   cut  ) const { return _process ( name , cut ) ; }
// ============================================================================
// add the function 
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process
( const std::string&          name , 
  const LoKi::Types::GVFunc&  cut  ) const { return _process ( name , cut ) ; }
// ============================================================================
// maps 
// ============================================================================
// add the map
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process
( const std::string&          name , 
  const LoKi::Types::GMaps&   cut  ) const { return _process ( name , cut ) ; }
// ============================================================================
// add the map 
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process
( const std::string&          name , 
  const LoKi::Types::GVMaps&  cut  ) const { return _process ( name , cut ) ; }
// ============================================================================
// pipe 
// ============================================================================
// add the pipe
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process
( const std::string&          name , 
  const LoKi::Types::GPipes&  cut  ) const { return _process ( name , cut ) ; }
// ============================================================================
// add the pipe
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process
( const std::string&          name , 
  const LoKi::Types::GVPipes& cut  ) const { return _process ( name , cut ) ; }
// ============================================================================
// fun-vals
// ============================================================================
// add the fun-val
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process
( const std::string&           name , 
  const LoKi::Types::GFunVals& cut  ) const { return _process ( name , cut ) ; }
// ============================================================================
// add the fun-val
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process
( const std::string&            name , 
  const LoKi::Types::GVFunVals& cut  ) const { return _process ( name , cut ) ; }
// ============================================================================
// cut-vals
// ============================================================================
// add the cut-val
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process
( const std::string&           name , 
  const LoKi::Types::GCutVals& cut  ) const { return _process ( name , cut ) ; }
// ============================================================================
// sources 
// ============================================================================
// add the source 
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process
( const std::string&          name , 
  const LoKi::Types::GSources& cut  ) const { return _process ( name , cut ) ; }
// ============================================================================
// add the source
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process
( const std::string&            name , 
  const LoKi::Types::GVSources& cut  ) const { return _process ( name , cut ) ; }
// ============================================================================

// ============================================================================
// The END 
// ============================================================================
