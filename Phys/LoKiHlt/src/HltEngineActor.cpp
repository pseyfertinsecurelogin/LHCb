// Include files 
// ============================================================================
// STD & STL 
// ============================================================================
#include <fstream>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/HltEngineActor.h"
#include "LoKi/Report.h"
// ============================================================================
/** @file 
 *
 *  Implementation file for class LoKi::Hybrid::HltEngineActor
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
inline StatusCode LoKi::Hybrid::HltEngineActor::_add 
( const std::string& name , const TYPE& cut ) const 
{
  if ( m_stack.empty() ) 
  { return LoKi::Report::Error
      ("LoKi:Hybrid::HltEngineActor::addCut/Fun(): empty stack!") ;  
  }
  //
  const Entry& entry = m_stack.top() ;
  // check the tool
  if ( !entry.first ) 
  {
    return LoKi::Report::Error
      ("LoKi:Hybrid::HltEngineActor::addCut/Fun(): LoKi::Hybrid::IHltAntiFactory* is invalid!") ;  
  }
  // one more check 
  if ( name != entry.first->name() )
  {
    return LoKi::Report::Error
      ("LoKi:Hybrid::HltEngineActor::addCut/Fun() : mismatch in LoKi::Hybrid::IHltAntiFactoryname!") ;  
  }
  // set the cut for the tool 
  entry.first->set ( cut ) ;
  // 
  return StatusCode::SUCCESS ;
}
// ============================================================================
// accessor to the static instance 
// ============================================================================
LoKi::Hybrid::HltEngineActor& LoKi::Hybrid::HltEngineActor::instance() 
{
  static LoKi::Hybrid::HltEngineActor s_holder ;
  return s_holder ;
}
// ============================================================================
// disconnect the tool 
// ============================================================================
StatusCode LoKi::Hybrid::HltEngineActor::disconnect
( const LoKi::Hybrid::IHltAntiFactory* factory ) 
{
  if ( m_stack.empty() ) 
  { return LoKi::Report::Error ("LoKi:Hybrid::GenEngineActor::disconnect: empty stack!") ; }
  //
  const Entry& entry = m_stack.top () ;
  //
  if ( entry.first == factory  ) { m_stack.pop() ; } /// remove the last entry
  else
  { return LoKi::Report::Error
      ("LoKi::Hybrid::HltEngineActor::disconnect: mismatch in tools " ) ;
  } 
  ///
  return StatusCode::SUCCESS ;
}
// ============================================================================
// connect the hybrid tool for code translation 
// ============================================================================
StatusCode LoKi::Hybrid::HltEngineActor::connect
( const LoKi::Hybrid::IHltAntiFactory* factory , 
  const LoKi::Context&                 context ) 
{
  if ( !factory ) 
  { return LoKi::Report::Error ( "LoKi::Hybrid::HltEngineActor::connect: Invalid factory" ) ; }
  m_stack.emplace ( factory , context ) ;
  return StatusCode::SUCCESS ;
}
// ============================================================================
/* get the current context
 *  contex is valid only inbetween <code>connect/disconnect</code>
 *  @return the current active context 
 */
// ============================================================================
const LoKi::Context* LoKi::Hybrid::HltEngineActor::context () const
{
  if ( m_stack.empty() ) 
  {
    LoKi::Report::Error ( "LoKi::Hybrid::HltEngineActor::context: empty stack" ) ;
    return nullptr ;
  }
  const Entry& last = m_stack.top() ;
  return &last.second ;
}
// ============================================================================
// propagate the function to the tool 
// ============================================================================
StatusCode LoKi::Hybrid::HltEngineActor::process
( const std::string&          name , 
  const LoKi::Types::L0_Cuts& func ) const 
{ return _add ( name , func ) ; }
// ============================================================================
// propagate the function to the tool 
// ============================================================================
StatusCode LoKi::Hybrid::HltEngineActor::process
( const std::string&          name , 
  const LoKi::Types::L0_Func& func ) const 
{ return _add ( name , func ) ; }
// ============================================================================
// propagate the function to the tool 
// ============================================================================
StatusCode LoKi::Hybrid::HltEngineActor::process
( const std::string&          name , 
  const LoKi::Types::ODIN_Cuts& func ) const 
{ return _add ( name , func ) ; }
// ============================================================================
// propagate the function to the tool 
// ============================================================================
StatusCode LoKi::Hybrid::HltEngineActor::process
( const std::string&          name , 
  const LoKi::Types::ODIN_Func& func ) const 
{ return _add ( name , func ) ; }
// ============================================================================
// propagate the function to the tool 
// ============================================================================
StatusCode LoKi::Hybrid::HltEngineActor::process
( const std::string&           name , 
  const LoKi::Types::HLT_Cuts& func ) const 
{ return _add ( name , func ) ; }
// ============================================================================
// propagate the function to the tool 
// ============================================================================
StatusCode LoKi::Hybrid::HltEngineActor::process
( const std::string&           name , 
  const LoKi::Types::HLT_Func& func ) const 
{ return _add ( name , func ) ; }
// ============================================================================
// The END 
// ============================================================================
