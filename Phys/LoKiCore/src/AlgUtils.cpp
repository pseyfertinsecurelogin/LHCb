/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
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
#include "GaudiKernel/IAlgorithm.h"
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GetAlgs.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/AuxFunBase.h"
#include "LoKi/ILoKiSvc.h"
// ============================================================================
// local
// ============================================================================
#include "LoKi/AlgUtils.h"
// ============================================================================
/** @file  
 *
 *  Access to algorithms 
 * 
 *  This file is part of LoKi project: 
 *   ``C++ ToolKit for Smart and Friendly Physics Analysis''
 * 
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
 *  contributions and advices from G.Raven, J.van Tilburg, 
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
 *  @date 2010-12-06
 */
// ============================================================================
// get gaudi algorithm from the context 
// ============================================================================
GaudiAlgorithm* 
LoKi::AlgUtils::getGaudiAlg ( const LoKi::AuxFunBase& base  , 
                              const bool              force ) 
{
  LoKi::ILoKiSvc* svc = base.lokiSvc() ;
  if ( force  ) { base.Assert( !(!svc) , "LoKi Service is not available!"   ) ; }
  else if ( !svc ) { return nullptr ; }
  //
  SmartIF<IAlgContextSvc> cntx ( svc ) ;
  if ( force  ) { base.Assert( !(!cntx) , "IAlgContextSvc* points to NULL!" ) ; }
  else if ( !cntx     ) { return nullptr ; }
  //
  return Gaudi::Utils::getGaudiAlg ( cntx ) ;
}
// ============================================================================
// get the algorithm from context service 
// ============================================================================
IAlgorithm*   LoKi::AlgUtils::getAlg      
( const LoKi::AuxFunBase& base  , 
  const bool              force ) 
{
  //
  LoKi::ILoKiSvc* svc = base.lokiSvc() ;
  if ( force ) { base.Assert( !(!svc)   , "LoKi Service is not available!" ) ; }
  else if ( !svc ) { return nullptr ; }
  //
  SmartIF<IAlgContextSvc> cntx ( svc ) ;
  if ( force ) { base.Assert( !(!cntx)  , "IAlgContextSvc* points to NULL!") ; }
  else if ( !cntx    ) { return nullptr ; }
  IAlgorithm* ialg = cntx->currentAlg() ;
  if ( force ) { base.Assert( nullptr != ialg , "IAlgorithm* points to NULL!"    ) ; }
  //
  return ialg ;
}
// ==========================================================================
// The END 
// ==========================================================================
