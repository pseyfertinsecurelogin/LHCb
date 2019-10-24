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
#ifndef KERNEL_IACCEPT_H
#define KERNEL_IACCEPT_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/Kernel.h"
// ============================================================================
/** @class IAccept Kernel/IAccept.h
 *
 *  Generic 'accept'-interface
 *
 *  @author Vanya Belyaev
 *  @date   2010-08-02
 */
struct GAUDI_API IAccept : extend_interfaces<IAlgTool> {
  // ==========================================================================
  /// InterfaceID
  DeclareInterfaceID( IAccept, 4, 0 );
  // ==========================================================================
  /// make a generic selection
  [[nodiscard]] virtual bool accept() const = 0;
  // ==========================================================================
};
// ============================================================================
// The END
// ============================================================================
#endif // KERNEL_ISELECTOR_H
