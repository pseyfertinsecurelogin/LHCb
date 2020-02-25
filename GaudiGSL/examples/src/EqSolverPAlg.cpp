/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files

// from Gaudi
#include "GaudiGSL/IEqSolver.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiMath/Adapters.h"
#include "GaudiMath/GaudiMath.h"
// local
#include "EqSolverPAlg.h"

//-----------------------------------------------------------------------------
/** @file Implementation file for class : EqSolverPAlg
 *  @see EqSolverPAlg.h
 *  @author Kirill Miklyaev kirillm@iris1.itep.ru
 *  @date 2003-07-07
 */
//-----------------------------------------------------------------------------

using namespace Genfun;
using namespace GaudiGSL::Examples;

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( EqSolverPAlg )

//=============================================================================
typedef Genfun::AbsFunction GenFunc;

// Our functions
double function1( const std::vector<double>& x ) { return x[0] * x[0] + x[1] * x[1] - 1; }

double function2( const std::vector<double>& x ) { return x[0] + x[1]; }

double function3( const std::vector<double>& x ) { return x[2] * x[2] * x[2] * x[1] - 120; }

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode EqSolverPAlg::initialize() {

  MsgStream log( msgSvc(), name() );
  log << MSG::INFO << "==> Initialise" << endmsg;

  StatusCode sc;
  sc = toolSvc()->retrieveTool( "EqSolver", m_publicTool );
  if ( sc.isFailure() ) { log << MSG::ERROR << "Error retrieving the public tool" << endmsg; }
  sc = toolSvc()->retrieveTool( "EqSolver", m_privateTool, this );
  if ( sc.isFailure() ) { log << MSG::ERROR << "Error retrieving the private tool" << endmsg; }
  log << MSG::INFO << "....initialization done" << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode EqSolverPAlg::execute() {

  MsgStream log( msgSvc(), name() );
  log << MSG::INFO << "==> Execute" << endmsg;

  // the objects of the class AdapterPFunction
  // @see Adapters.h
  const GaudiMath::Function& adap1 = GaudiMath::adapter( 3, &function1 );
  const GaudiMath::Function& adap2 = GaudiMath::adapter( 3, &function2 );
  const GaudiMath::Function& adap3 = GaudiMath::adapter( 3, &function3 );

  std::vector<const GenFunc*> function;

  function.push_back( &adap1 );
  function.push_back( &adap2 );
  function.push_back( &adap3 );

  //=============================================================================

  // Input number and value of the arguments of the function "GenFunc"
  IEqSolver::Arg arg( function.size() );

  arg[0] = 2;
  arg[1] = 3;
  arg[2] = 5;

  // Call of the method
  m_publicTool->solver( function, arg ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  log << endmsg;
  log << "START OF THE METHOD" << endmsg;
  log << "SOLUTION FOUND AT: " << endmsg;

  for ( unsigned int i = 0; i < arg.dimension(); i++ ) {
    log << "Value of argument " << i << " is " << arg[i] << endmsg;
  }
  log << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode EqSolverPAlg::finalize() {

  MsgStream log( msgSvc(), name() );
  log << MSG::INFO << "==> Finalize" << endmsg;

  toolSvc()->releaseTool( m_publicTool ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  toolSvc()->releaseTool( m_privateTool ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );

  return StatusCode::SUCCESS;
}

//=============================================================================
