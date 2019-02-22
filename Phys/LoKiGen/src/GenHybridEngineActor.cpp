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
// STD & STL
// ============================================================================
#include <fstream>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/GenHybridEngineActor.h"
#include "LoKi/Report.h"
// ============================================================================
/** @file
 *  Implementation file for class LoKi::Hybrid::GenEngineActor
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @date 2004-06-29
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 */
// ============================================================================
// helper method to descrease number of lines
// ============================================================================
template <class TYPE>
inline StatusCode LoKi::Hybrid::GenEngineActor::_add( const std::string& name, const TYPE& cut ) const {
  if ( m_stack.empty() ) { return LoKi::Report::Error( "LoKi:Hybrid::GenEngineActor::addCut/Fun(): empty stack!" ); }
  //
  const Entry& entry = m_stack.top();
  // check the tool
  if ( !entry.first ) {
    return LoKi::Report::Error( "LoKi:Hybrid::GenEngineActor::addCut/Fun(): LoKi::IGenHybridTool* is invalid!" );
  }
  // one more check
  if ( name != entry.first->name() ) {
    return LoKi::Report::Error( "LoKi:Hybrid::GenEngineActor::addCut/Fun() : mismatch in LoKi::IGenHybndrTool name!" );
  }
  // set the cut for the tool
  entry.first->set( cut );
  //
  return StatusCode::SUCCESS;
}
// ============================================================================
// accessor to the static instance
// ============================================================================
LoKi::Hybrid::GenEngineActor& LoKi::Hybrid::GenEngineActor::instance() {
  static LoKi::Hybrid::GenEngineActor s_holder;
  return s_holder;
}
// ============================================================================
// disconnect the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::disconnect( const LoKi::IGenHybridTool* factory ) {
  if ( m_stack.empty() ) { return LoKi::Report::Error( "LoKi:Hybrid::GenEngineActor::disconnect: empty stack!" ); }
  //
  const Entry& entry = m_stack.top();
  //
  if ( entry.first == factory ) {
    m_stack.pop();
  } /// remove the last entry
  else {
    return LoKi::Report::Error( "LoKi::Hybrid::GenEngineActor::disconnect: mismatch in tools " );
  }
  ///
  return StatusCode::SUCCESS;
}
// ============================================================================
// connect the hybrid tool for code translation
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::connect( const LoKi::IGenHybridTool* factory, const LoKi::Context& context ) {
  if ( !factory ) { return LoKi::Report::Error( "LoKi::Hybrid::GenEngineActor::connect: Invalid factory" ); }
  m_stack.emplace( factory, context );
  return StatusCode::SUCCESS;
}
// ============================================================================
/* get the current context
 *  contex is valid only inbetween <code>connect/disconnect</code>
 *  @return the current active context
 */
// ============================================================================
const LoKi::Context* LoKi::Hybrid::GenEngineActor::context() const {
  if ( m_stack.empty() ) {
    LoKi::Report::Error( "LoKi::Hybrid::GenEngineActor::context: empty stack" );
    return nullptr;
  }
  const Entry& last = m_stack.top();
  return &last.second;
}
// ============================================================================
// predicates
// ============================================================================
// propagate the cut to the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::process( const std::string& name, const LoKi::Types::GCuts& cut ) const {
  return _add( name, cut );
}
// ============================================================================
// propagate the cut to the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::process( const std::string& name, const LoKi::Types::GVCuts& cut ) const {
  return _add( name, cut );
}
// ============================================================================
// functions
// ============================================================================
// propagate the function to the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::process( const std::string& name, const LoKi::Types::GFunc& func ) const {
  return _add( name, func );
}
// ============================================================================
// propagate the function to the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::process( const std::string& name, const LoKi::Types::GVFunc& func ) const {
  return _add( name, func );
}
// ============================================================================
// maps
// ============================================================================
// propagate the map to the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::process( const std::string& name, const LoKi::Types::GMaps& func ) const {
  return _add( name, func );
}
// ============================================================================
// propagate the map to the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::process( const std::string& name, const LoKi::Types::GVMaps& func ) const {
  return _add( name, func );
}
// ============================================================================
// pipes
// ============================================================================
// propagate the pipe to the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::process( const std::string& name, const LoKi::Types::GPipes& func ) const {
  return _add( name, func );
}
// ============================================================================
// propagate the pipe  to the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::process( const std::string& name, const LoKi::Types::GVPipes& func ) const {
  return _add( name, func );
}
// ============================================================================
// fun-vals
// ============================================================================
// propagate the fun-val to the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::process( const std::string& name, const LoKi::Types::GFunVals& func ) const {
  return _add( name, func );
}
// ============================================================================
// propagate the fun-val to the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::process( const std::string& name, const LoKi::Types::GVFunVals& func ) const {
  return _add( name, func );
}
// ============================================================================
// cut-vals
// ============================================================================
// propagate the cut-val to the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::process( const std::string& name, const LoKi::Types::GCutVals& func ) const {
  return _add( name, func );
}
// ============================================================================
// sources
// ============================================================================
// propagate the source to the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::process( const std::string& name, const LoKi::Types::GSources& func ) const {
  return _add( name, func );
}
// ============================================================================
// propagate the source to the tool
// ============================================================================
StatusCode LoKi::Hybrid::GenEngineActor::process( const std::string& name, const LoKi::Types::GVSources& func ) const {
  return _add( name, func );
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
