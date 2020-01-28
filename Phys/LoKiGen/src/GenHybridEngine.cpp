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
 */
// ============================================================================
namespace {
  template <class TYPE>
  inline StatusCode _process( const std::string& name, const TYPE& cut ) {
    LoKi::Hybrid::GenEngineActor& actor = LoKi::Hybrid::GenEngineActor::instance();
    return actor.process( name, cut );
  }
} // namespace
// ============================================================================
// predicates
// ============================================================================
// add the cut
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process( const std::string& name, const LoKi::Types::GCuts& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// add the cut
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process( const std::string& name, const LoKi::Types::GVCuts& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// functions
// ============================================================================
// add the function
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process( const std::string& name, const LoKi::Types::GFunc& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// add the function
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process( const std::string& name, const LoKi::Types::GVFunc& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// maps
// ============================================================================
// add the map
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process( const std::string& name, const LoKi::Types::GMaps& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// add the map
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process( const std::string& name, const LoKi::Types::GVMaps& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// pipe
// ============================================================================
// add the pipe
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process( const std::string& name, const LoKi::Types::GPipes& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// add the pipe
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process( const std::string& name, const LoKi::Types::GVPipes& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// fun-vals
// ============================================================================
// add the fun-val
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process( const std::string& name, const LoKi::Types::GFunVals& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// add the fun-val
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process( const std::string& name, const LoKi::Types::GVFunVals& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// cut-vals
// ============================================================================
// add the cut-val
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process( const std::string& name, const LoKi::Types::GCutVals& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// sources
// ============================================================================
// add the source
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process( const std::string& name, const LoKi::Types::GSources& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// add the source
// ============================================================================
StatusCode LoKi::Hybrid::GenEngine::process( const std::string& name, const LoKi::Types::GVSources& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// get the current context
// ============================================================================
const LoKi::Context* LoKi::Hybrid::GenEngine::context() const {
  const LoKi::Hybrid::GenEngineActor& actor = LoKi::Hybrid::GenEngineActor::instance();
  return actor.context();
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
