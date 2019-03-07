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
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IAlgContextSvc.h"
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/IAlgorithm.h"
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/SmartIF.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GetAlg.h"
// ============================================================================
// DaVinciInterfaces
// ============================================================================
#include "Kernel/GetIDVAlgorithm.h"
#include "Kernel/IDVAlgorithm.h"
// ============================================================================
/** @file
 *  Implementation file for function Gaudi::Utils::GetIDVAlgorithm
 *  @see IDVAlgorithm
 *  @see IAlgContextSvc
 *  @see GaudiAlg/GetAlg.h
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2010-05-14
 */
// ============================================================================
namespace {
  // =========================================================================
  /** @class IDVSelector
   *  Helper class for selection of IDVAlgorithm
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2010-05-14
   */
  class IDVSelector : public Gaudi::Utils::AlgSelector {
  public:
    // ========================================================================
    /// the only one essential method:
    bool operator()( const IAlgorithm* alg ) const override {
      if ( !alg ) { return false; }
      IAlgorithm*           _alg = const_cast<IAlgorithm*>( alg );
      SmartIF<IDVAlgorithm> idv( _alg );
      return idv.isValid();
    }
    virtual ~IDVSelector() {}
    // ========================================================================
  };
  // ==========================================================================
} // namespace
// ============================================================================
IDVAlgorithm* Gaudi::Utils::getIDVAlgorithm( const IAlgContextSvc* svc ) {
  if ( !svc ) { return NULL; } // RETURN
  // use context service:
  IAlgorithm* alg = Gaudi::Utils::getAlgorithm( svc, IDVSelector() );
  if ( !alg ) { return NULL; } // RETURN
  return SmartIF<IDVAlgorithm>( alg );
}
// ============================================================================
IDVAlgorithm* Gaudi::Utils::getIDVAlgorithm( const IAlgContextSvc* svc, const IAlgTool* tool ) {
  if ( !tool ) { return NULL; }
  // Recurse down the ownership tree, to see with we ever end up at the ToolSvc
  bool         ownedByToolSvc = false;
  unsigned int sanityCheck( 0 );
  while ( tool && ++sanityCheck < 9999 ) {
    ownedByToolSvc = ( NULL != dynamic_cast<const IToolSvc*>( tool->parent() ) );
    if ( ownedByToolSvc ) { break; }
    // if parent is also a tool, try again
    tool = dynamic_cast<const IAlgTool*>( tool->parent() );
  }
  if ( ownedByToolSvc ) {
    throw GaudiException( "Cannot safely be used with public tools", "Gaudi::Utils::getIDVAlgorithm",
                          StatusCode::FAILURE );
  }
  // No, so OK to return the IDVAlgorithm interface
  return getIDVAlgorithm( svc );
}
// ============================================================================
// The END
// ============================================================================
