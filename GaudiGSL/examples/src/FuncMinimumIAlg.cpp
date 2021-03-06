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
// from CLHEP
#include "CLHEP/GenericFunctions/GenericFunctions.hh"
#include "CLHEP/Matrix/SymMatrix.h"
// from AIDA
#include "AIDA/IFunction.h"
// local
#include "FuncMinimumIAlg.h"

// disable warning about nullptr dereferencing (icc and clang)
//   It's a real problem, but here it is used for test purposes
#ifdef __ICC
#  pragma warning( disable : 327 )
#elif __clang__
#  pragma clang diagnostic ignored "-Wnull-dereference"
#endif

// Handle CLHEP 2.0.x move to CLHEP namespace
namespace CLHEP {}
using namespace CLHEP;

using namespace GaudiGSL::Examples;

//-----------------------------------------------------------------------------
/** @file Implementation file for class : FuncMinimumIAlg
 *  @see FuncMinimumIAlg.h
 *  @author Kirill Miklyaev kirillm@iris1.itep.ru
 *  @date 2002-09-14
 */
//-----------------------------------------------------------------------------

//=============================================================================
// Class for the function "IFunction"
// @see AIDA/IFunction.h
class Function : virtual public AIDA::IFunction

{
public:
  typedef std::vector<double> argument;

public:
  Function() { m_gradient.resize( 2 ); }
  double      value( const std::vector<double>& x ) const override { return 10 * x[0] * x[0] + 20 * x[1] * x[1]; }
  int         dimension() const override { return 2; }
  bool        setTitle( const std::string& ) override { return false; }
  std::string title( void ) const override { return ""; }
  bool        isEqual( const AIDA::IFunction& ) const override { return false; }
  const std::vector<double>& gradient( const std::vector<double>& x ) const override {
    m_gradient[0] = 20 * x[0];
    m_gradient[1] = 40 * x[1];
    return m_gradient;
  }
  bool                       providesGradient( void ) const override { return true; }
  std::string                variableName( int ) const override { return ""; }
  std::vector<std::string>   variableNames( void ) const override { return m_names; }
  bool                       setParameters( const std::vector<double>& ) override { return false; }
  const std::vector<double>& parameters( void ) const override { return m_values; }
  int                        numberOfParameters( void ) const override { return 0; }
  std::vector<std::string>   parameterNames() const override { return m_names; }
  bool                       setParameter( const std::string&, double ) override { return false; }
  double                     parameter( const std::string& ) const override { return 0.0; }
  int                        indexOfParameter( const std::string& ) const override { return 0; }
  const AIDA::IAnnotation&   annotation( void ) const override { return *(AIDA::IAnnotation*)0; }
  AIDA::IAnnotation&         annotation( void ) override { return *(AIDA::IAnnotation*)0; }
  std::string                codeletString( void ) const override { return ""; }
  void*                      cast( const std::string& ) const override { return 0; }

private:
  std::string                 m_version;
  std::string                 m_label;
  std::vector<std::string>    m_names;
  std::vector<double>         m_values;
  mutable std::vector<double> m_gradient;
};

//=============================================================================

//=============================================================================
// Initialization. Check parameters
//=============================================================================
StatusCode FuncMinimumIAlg::initialize() {

  MsgStream log( msgSvc(), name() );
  log << MSG::INFO << "==> Initialize" << endmsg;

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
StatusCode FuncMinimumIAlg::execute() {

  MsgStream log( msgSvc(), name() );
  log << MSG::INFO << "==> Execute" << endmsg;

  // the object of the class Function
  const Function* fun = new Function();
  // the object of the class AdapterIFunction
  // @see Adapters.h
  const GaudiMath::AIDAFunction func = GaudiMath::adapter( *fun );

  //=============================================================================
  // Input number and value of the arguments of the function "GenFunc"
  IFuncMinimum::Arg arg( func.dimensionality() );

  arg[0] = 5;
  arg[1] = 10;

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
  arg[0] = 5;
  arg[1] = 10;

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
StatusCode FuncMinimumIAlg::finalize() {

  MsgStream log( msgSvc(), name() );
  log << MSG::INFO << "==> Finalize" << endmsg;

  toolSvc()->releaseTool( m_publicTool );
  toolSvc()->releaseTool( m_privateTool );

  return StatusCode::SUCCESS;
}

//=============================================================================
// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FuncMinimumIAlg )
