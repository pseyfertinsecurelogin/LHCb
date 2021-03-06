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
#include "GaudiGSL/IFuncMinimum.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiMath/Adapters.h"
#include "GaudiMath/GaudiMath.h"
// from CLHEP
#include "CLHEP/Matrix/SymMatrix.h"
// local
#include "FuncMinimumPAlg.h"

// Handle CLHEP 2.0.x move to CLHEP namespace
namespace CLHEP {}
using namespace CLHEP;

using namespace GaudiGSL::Examples;

//-----------------------------------------------------------------------------
/** @file Implementation file for class : FuncMinimumPAlg
 *  @see FuncMinimumPAlg.h
 *  @author Kirill Miklyaev kirillm@iris1.itep.ru
 *  @date 2002-09-14
 */
//-----------------------------------------------------------------------------

//=============================================================================
// Our function
double function( const std::vector<double>& x ) { return 10 * x[0] * x[0] + 20 * x[1] * x[1] + 40; }

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode FuncMinimumPAlg::initialize() {

  MsgStream log( msgSvc(), name() );
  log << MSG::INFO << "==> Initialise" << endmsg;

  StatusCode sc;
  sc = toolSvc()->retrieveTool( "FuncMinimum", m_publicTool );
  if ( sc.isFailure() ) { log << MSG::ERROR << "Error retrieving the public tool" << endmsg; }
  sc = toolSvc()->retrieveTool( "FuncMinimum", m_privateTool, this );
  if ( sc.isFailure() ) { log << MSG::ERROR << "Error retrieving the private tool" << endmsg; }
  log << MSG::INFO << "....initialization done" << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode FuncMinimumPAlg::execute() {

  MsgStream log( msgSvc(), name() );
  log << MSG::INFO << "==> Execute" << endmsg;

  // the object of the class AdapterPFunction
  // @see Adapter.h
  const GaudiMath::Function& func = GaudiMath::adapter( 2, &function );

  //=============================================================================
  // Input number and value of the arguments of the function "GenFunc"
  IFuncMinimum::Arg arg( func.dimensionality() );

  arg[0] = 2;
  arg[1] = 4;

  // Matrix of error
  IFuncMinimum::Covariance matrix_error( arg.dimension(), 0 );

  // Call of the method
  m_publicTool->minimum( func, arg );
  log << endmsg;
  log << "START OF THR METHOD" << endmsg;
  log << "MINIMUM FOUND AT: " << endmsg;

  for ( unsigned int i = 0; i < arg.dimension(); i++ ) {

    log << "Value of argument " << i << " is " << arg[i] << endmsg;
  }

  log << endmsg;
  //=============================================================================
  // With Covariance matrix (matrix of error)
  arg[0] = 2;
  arg[1] = 4;

  // Call of the method(with covariance matrix (matrix of error))
  m_publicTool->minimum( func, arg, matrix_error );
  log << endmsg;
  log << "THE METHOD WITH MATRIX OF ERROR" << endmsg;
  log << "MINIMUM FOUND AT: " << endmsg;

  for ( unsigned int i = 0; i < arg.dimension(); i++ ) {

    log << "Value of argument " << i << " is " << arg[i] << endmsg;
  }

  log << endmsg;
  log << "MATRIX OF ERROR";

  for ( unsigned int i = 0; i < arg.dimension(); i++ ) {
    log << endmsg;

    for ( unsigned int j = 0; j < arg.dimension(); j++ ) { log << matrix_error( i + 1, j + 1 ) << " "; }
  }
  log << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode FuncMinimumPAlg::finalize() {

  MsgStream log( msgSvc(), name() );
  log << MSG::INFO << "==> Finalize" << endmsg;

  toolSvc()->releaseTool( m_publicTool );
  toolSvc()->releaseTool( m_privateTool );

  return StatusCode::SUCCESS;
}

//=============================================================================
// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FuncMinimumPAlg )
