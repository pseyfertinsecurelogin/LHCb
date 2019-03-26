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
#include "LoKi/HltEngine.h"
#include "LoKi/HltEngineActor.h"
// ============================================================================
/** @file
 *  Implementation file for class LoKi::Hybrid::HltEngine
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
// ============================================================================
namespace {
  template <class TYPE>
  inline StatusCode _process( const std::string& name, const TYPE& cut ) {
    LoKi::Hybrid::HltEngineActor& actor = LoKi::Hybrid::HltEngineActor::instance();
    return actor.process( name, cut );
  }
} // namespace
// ============================================================================
// get the current context
// ============================================================================
const LoKi::Context* LoKi::Hybrid::HltEngine::context() const {
  const LoKi::Hybrid::HltEngineActor& actor = LoKi::Hybrid::HltEngineActor::instance();
  return actor.context();
}
// ============================================================================
// add the cut
// ============================================================================
StatusCode LoKi::Hybrid::HltEngine::process( const std::string& name, const LoKi::Types::L0_Cuts& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// add the cut
// ============================================================================
StatusCode LoKi::Hybrid::HltEngine::process( const std::string& name, const LoKi::Types::L0_Func& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// add the cut
// ============================================================================
StatusCode LoKi::Hybrid::HltEngine::process( const std::string& name, const LoKi::Types::ODIN_Cuts& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// add the cut
// ============================================================================
StatusCode LoKi::Hybrid::HltEngine::process( const std::string& name, const LoKi::Types::ODIN_Func& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// add the cut
// ============================================================================
StatusCode LoKi::Hybrid::HltEngine::process( const std::string& name, const LoKi::Types::HLT_Cuts& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// add the cut
// ============================================================================
StatusCode LoKi::Hybrid::HltEngine::process( const std::string& name, const LoKi::Types::HLT_Func& cut ) const {
  return _process( name, cut );
}
// ============================================================================
// The END
// ============================================================================
