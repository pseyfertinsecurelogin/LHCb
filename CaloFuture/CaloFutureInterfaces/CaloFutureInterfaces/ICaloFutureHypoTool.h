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
#include "Kernel/STLExtensions.h"
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
namespace LHCb::Calo::Interfaces {
  struct IProcessHypos : extend_interfaces<IAlgTool> {

    /** static interface identification
     *  @see IInterface
     *  @return unique interface identifier
     */
    DeclareInterfaceID( IProcessHypos, 1, 0 );

    /** The main processing method
     *  @param  hypos  range of pointers to CaloHypo objects to be processed
     *  @return status code
     */
    // FIXME: the use of `CaloHypo*` is temporary, waiting for a migration
    //       from KeyedContainer to a plain vector. At that point this should
    //       become span<CaloHypo> hypos...
    virtual StatusCode process( span<CaloHypo* const> hypos ) const = 0;

    /** The main processing method -- backwards compatibility for callers
     *  @param  hypo  reference to CaloHypo object to be processed
     *  @return status code
     */
    StatusCode process( CaloHypo& hypo ) const { return process( range::single{&hypo} ); }
  };
} // namespace LHCb::Calo::Interfaces

// ============================================================================
#endif // CALOFUTUREINTERFACES_ICALOFUTURECLUSTERTOOL_H
