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
#ifndef CALOFUTUREINTERFACES_ICALOFUTUREHYPOTOOL_H
#define CALOFUTUREINTERFACES_ICALOFUTUREHYPOTOOL_H 1
// Include files
// STD & STL
#include <functional>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
namespace LHCb {
  class CaloHypo;
}
/** @class ICaloFutureHypoTool ICaloFutureHypoTool.h CaloFutureInterfaces/ICaloFutureHypoTool.h
 *
 *  The generic interface for "CalorimeterFuture tools" , which deals with
 *  CaloHypo objects, the potential candidates are:
 *
 *    \li hypothesis processing
 *    \li dispatching
 *    \li subcomponent of CaloFutureParticle processing
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   30/10/2001
 */

struct ICaloFutureHypoTool : extend_interfaces<IAlgTool> {

  /** static interface identification
   *  @see IInterface
   *  @return unique interface identifier
   */
  DeclareInterfaceID( ICaloFutureHypoTool, 3, 0 );

  /** The main processing method
   *  @param  hypo  pointer to CaloHypo object to be processed
   *  @return status code
   */
  virtual StatusCode process( LHCb::CaloHypo& hypo ) const = 0;

  /** The main processing method (functor interface)
   *  @param  hypo  pointer to CaloHypo object to be processed
   *  @return status code
   */
  virtual StatusCode operator()( LHCb::CaloHypo& hypo ) const = 0;
};

// ============================================================================
#endif // CALOFUTUREINTERFACES_ICALOFUTURECLUSTERTOOL_H
