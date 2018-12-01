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
// Include files 
// ============================================================================
// STD & STL 
// ============================================================================
#include <fstream>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/CoreEngineActor.h"
#include "LoKi/Report.h"
// ============================================================================
/** @file 
 *
 *  Implementation file for class LoKi::Hybrid::CoreEngineActor
 *
 *  This file is a part of LoKi project - 
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
 *  contributions and advices from G.Raven, J.van Tilburg, 
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @date 2008-09-28 
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 */
// ============================================================================
// helper method to descrease number of lines 
// ============================================================================
template <class TYPE>
inline StatusCode LoKi::Hybrid::CoreEngineActor::_add 
( const std::string& name , const TYPE& cut ) const 
{ 
  if ( m_stack.empty() ) 
  {
    return LoKi::Report::Error
      ("LoKi:Hybrid::CoreEngineActor::addCut/Fun(): empty stack!") ;  
  }
  //
  const Entry& entry = m_stack.top() ;
  // check the tool
  if ( !entry.first ) 
  {
    return LoKi::Report::Error
      ("LoKi:Hybrid::CoreEngineActor::addCut/Fun(): LoKi::Hybrid::ICoreAntiFactory* is invalid!") ;  
  }
  // one more check 
  if ( name != entry.first->name() )
  {
    return LoKi::Report::Error
      ("LoKi:Hybrid::CoreEngineActor::addCut/Fun() : mismatch in LoKi::Hybrid::ICoreAntiFactory name!") ;  
  }
  // set the cut for the tool 
  entry.first->set ( cut ) ;
  // 
  return StatusCode::SUCCESS ;
} 
// ============================================================================
// accessor to the static instance 
// ============================================================================
LoKi::Hybrid::CoreEngineActor& LoKi::Hybrid::CoreEngineActor::instance() 
{
  static LoKi::Hybrid::CoreEngineActor s_holder ;
  return s_holder ;
}
// ============================================================================
// constructor 
// ============================================================================
LoKi::Hybrid::CoreEngineActor::CoreEngineActor() 
  : m_stack() {} 
// ============================================================================
// destructor
// ============================================================================
LoKi::Hybrid::CoreEngineActor::~CoreEngineActor() {} 
// ============================================================================
// disconnect the tool 
// ============================================================================
StatusCode LoKi::Hybrid::CoreEngineActor::disconnect 
( const LoKi::Hybrid::ICoreAntiFactory* factory  ) 
{
  if ( m_stack.empty() ) 
  { return LoKi::Report::Error ("LoKi:Hybrid::CoreEngineActor::disconnect: empty stack!") ; }
  //
  const Entry& entry = m_stack.top () ;
  //
  if ( entry.first == factory  ) { m_stack.pop() ; } /// remove the last entry
  else
  {
    return LoKi::Report::Error
      ("LoKi::Hybrid::CoreEngineActor::disconnect: mismatch in tools " ) ;
  } 
  ///
  return StatusCode::SUCCESS ;
}
// ============================================================================
// connect the hybrid tool for code translation 
// ============================================================================
StatusCode LoKi::Hybrid::CoreEngineActor::connect
( const LoKi::Hybrid::ICoreAntiFactory*  factory , 
  const LoKi::Context&                   context )
{
  //
  if ( !factory ) 
  {
    return LoKi::Report::Error
      ( "LoKi::Hybrid::CoreEngineActor::connect: Invalid factory" ) ;
  }
  m_stack.emplace ( factory , context ) ;
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
/* get the current context
 *  contex is valid only inbetween <code>connect/disconnect</code>
 *  @return the current active context 
 */
// ============================================================================
const LoKi::Context* LoKi::Hybrid::CoreEngineActor::context () const
{
  if ( m_stack.empty() ) 
  {
    LoKi::Report::Error ( "LoKi::Hybrid::CoreEngineActor::context: empty stack" ) ;
    return nullptr ;
  }
  const Entry& last = m_stack.top() ;
  return &last.second ;
}  
// ============================================================================

// ============================================================================
// propagate the function to the tool 
// ============================================================================
StatusCode LoKi::Hybrid::CoreEngineActor::process
( const std::string&        name , 
  const LoKi::Types::FCuts& cut  ) const 
{ return _add ( name , cut ) ; }
// ============================================================================
// propagate the function to the tool 
// ============================================================================
StatusCode LoKi::Hybrid::CoreEngineActor::process
( const std::string&        name , 
  const LoKi::Types::FFunc& cut  ) const 
{ return _add ( name , cut ) ; }
// ============================================================================
// propagate the function to the tool 
// ============================================================================
StatusCode LoKi::Hybrid::CoreEngineActor::process
( const std::string&        name , 
  const LoKi::Types::XCuts& cut  ) const 
{ return _add ( name , cut ) ; }
// ============================================================================
// propagate the function to the tool 
// ============================================================================
StatusCode LoKi::Hybrid::CoreEngineActor::process
( const std::string&        name , 
  const LoKi::Types::XFunc& cut  ) const 
{ return _add ( name , cut ) ; }
// ============================================================================
// propagate the function to the tool 
// ============================================================================
StatusCode LoKi::Hybrid::CoreEngineActor::process
( const std::string&        name , 
  const LoKi::Types::XMaps& cut  ) const 
{ return _add ( name , cut ) ; }
// ============================================================================
// propagate the function to the tool 
// ============================================================================
StatusCode LoKi::Hybrid::CoreEngineActor::process
( const std::string&           name , 
  const LoKi::Types::XFunVals& cut  ) const 
{ return _add ( name , cut ) ; }
// ============================================================================
// propagate the function to the tool 
// ============================================================================
StatusCode LoKi::Hybrid::CoreEngineActor::process
( const std::string&           name , 
  const LoKi::Types::XSources& cut  ) const 
{ return _add ( name , cut ) ; }
// ============================================================================




// ============================================================================
// The END 
// ============================================================================
