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
#ifndef GAUDIGSL_IGSLERRORHANDLER_H
#define GAUDIGSL_IGSLERRORHANDLER_H 1
// Include files
#include "GaudiKernel/IAlgTool.h"
// forward declarations
class GslError;

/** @class IGslErrorHandler IGslErrorHandler.h GaudiGSL/IGslErrorHandler.h
 *
 *  The abstract interface for arbitrary GSL error handler
 *
 *  @author Vanya Ivan.Belyaev@itep.ru
 *  @date   30/04/2002
 */
struct GAUDI_API IGslErrorHandler : extend_interfaces<IAlgTool> {
  /// InterfaceID
  DeclareInterfaceID( IGslErrorHandler, 3, 0 );

  /** handle the GSL error
   *  @param error  error to be handled
   *  @see GslError
   *  @return status code
   */
  virtual StatusCode handle( const GslError& error ) const = 0;
};

// ============================================================================
// The END
// ============================================================================
#endif // GAUDIGSL_IGSLERRORHANDLER_H
