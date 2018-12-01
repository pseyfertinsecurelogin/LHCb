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
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgContextSvc.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/FilterAlg.h"
// ============================================================================
/** @file
 *  Implementation file for class LoKi::FilterAlg
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @see LoKi::FilterAlg
 *  @date 2008-09-23
 *  @author Vanya  BELYAEV Ivan.Belyaev@nikhef.nl
 */
// ============================================================================
/*  standard constructor
 *  @see GaudiAlgorithm
 *  @see      Algorithm
 *  @see      AlgFactory
 *  @see     IAlgFactory
 *  @param name the algorithm instance name
 *  @param pSvc pointer to Service Locator
 */
// ============================================================================
LoKi::FilterAlg::FilterAlg
( const std::string& name ,                     // the algorithm instance name
  ISvcLocator*       pSvc )                  // pointer to the service locator
  : GaudiAlgorithm ( name , pSvc )
{
  // the factory
  declareProperty
    ( "Factory" ,
      m_factory ,
      "The type/name of LoKiBender \"hybrid\" factory" )
    -> declareUpdateHandler ( &LoKi::FilterAlg::updateFactory  , this ) ;
  // the code
  declareProperty
    ( "Code"    ,
      m_code    ,
      "The Bender/Python code to be used" )
    -> declareUpdateHandler ( &LoKi::FilterAlg::updateCode     , this ) ;
  // the code
  declareProperty
    ( "Preambulo"  ,
      m_preambulo_ ,
      "The preambulo to be used for the temporary python script" )
    -> declareUpdateHandler ( &LoKi::FilterAlg::updatePreambulo , this ) ;
  //
  Assert( setProperty ( "RegisterForContextService" , true ).isSuccess() ,
          "Unable to enforce the registration for Algorithm Context Service") ;
}
// ============================================================================
// add to preambulo
// ============================================================================
void LoKi::FilterAlg::addToPreambulo( const std::string& item )
{
  m_preambulo_.push_back ( item ) ;
  m_preambulo_updated = true ;
}
// ============================================================================
// set preambulo
// ============================================================================
void LoKi::FilterAlg::setPreambulo( const std::vector<std::string>& items )
{
  m_preambulo_        = items ;
  m_preambulo_updated = true  ;
}
// ============================================================================
// update the factory
// ============================================================================
void LoKi::FilterAlg::updateFactory ( Property& ) // update the factory
{
  // mark as "to-be-updated"
  m_factory_updated = true ;
  // no action if not yet initialized
  if ( Gaudi::StateMachine::INITIALIZED > targetFSMState() ) { return ; }
  //
  // postpone the action
  if ( !m_code_updated || !m_preambulo_updated ) { return ; }
  //
  // perform the actual immediate decoding
  //
  StatusCode sc = decode () ;
  Assert ( sc.isSuccess () , "Error from LoKi::FilterAlg::decode()" , sc ) ;

  m_code_updated      = false ;
  m_factory_updated   = false ;
  m_preambulo_updated = false ;
}
// ============================================================================
// update the code
// ============================================================================
void LoKi::FilterAlg::updateCode ( Property& /* p */ )    // update the factory
{
  // no action if not yet initialized
  if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) { return ; }
  /// mark as "to-be-updated"
  m_code_updated = true ;
  //
  // postpone the action
  if ( !m_factory_updated || !m_preambulo_updated ) { return ; }

  // perform the actual immediate decoding

  StatusCode sc = decode  () ;
  Assert ( sc.isSuccess () , "Error from LoKi::FilterAlg::decode()" , sc ) ;

  m_code_updated      = false ;
  m_factory_updated   = false ;
  m_preambulo_updated = false ;
}
// ============================================================================
// update the preambulo
// ============================================================================
void LoKi::FilterAlg::updatePreambulo ( Property& /* p */ )  // update preambulo
{
  // decode the preambulo:
  m_preambulo.clear() ;
  auto iline = m_preambulo_.cbegin() ;
  if    ( iline != m_preambulo_.cend() ) { m_preambulo += *iline++ ; }
  while ( iline != m_preambulo_.cend() )
  {
    m_preambulo += "\n" ;
    m_preambulo += *iline++ ;
  }
  // no further action if not yet initialized
  if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) { return ; }
  /// mark as "to-be-updated"
  m_preambulo_updated = true ;
  // postpone the action
  if ( !m_factory_updated || !m_code_updated ) { return ; }

  // perform the actual immediate decoding

  StatusCode sc = decode  () ;
  Assert ( sc.isSuccess () , "Error from LoKi::FilterAlg::decode()" , sc ) ;

  m_code_updated      = false ;
  m_factory_updated   = false ;
  m_preambulo_updated = false ;
}
// ============================================================================
// the initialization of the algorithm
// ============================================================================
StatusCode LoKi::FilterAlg::initialize ()
{
  // look the context
  Gaudi::Utils::AlgContext lock  ( this , contextSvc() ) ;
  /// initialize the base
  StatusCode sc = GaudiAlgorithm::initialize () ;
  if ( sc.isFailure() ) { return sc ; }
  /// lock the context
  Gaudi::Utils::AlgContext lock2 ( this , contextSvc() ) ;
  // force LoKi service
  svc<IService>( "LoKiSvc" , true ) ;
  // decode the functor
  return decode () ;
}
// ============================================================================
// The END
// ============================================================================
