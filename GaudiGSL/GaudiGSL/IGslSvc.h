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
#ifndef GAUDIGSL_IGSLSVC_H
#define GAUDIGSL_IGSLSVC_H 1
// Include files
#include "GaudiKernel/IService.h"
//  forward declaration
class GslError;
/** @class IGslSvc IGslSvc.h GaudiGSL/IGslSvc.h
 *
 *  The abstract interface to "deal" with GNU Scientific Library (GLS)
 *  The main task is the error handling.
 *  Could be extended in future to cover other aspects as well.
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   29/04/2002
 */
struct GAUDI_API IGslSvc : extend_interfaces<IService> {
  /// InterfaceID
  DeclareInterfaceID( IGslSvc, 3, 0 );

  /** type definition of "standard" GSL error handler functions
   *  @param reason error reason (message)
   *  @param file   file  name
   *  @param line   line  number
   *  @param code   error code
   */
  typedef void ( *GslErrorHandler )( const char* /* reason */, const char* /* file   */, int /* line   */,
                                     int /* code   */ );

  /** handle the GSL error
   *  @param error  error to be handled
   *  @see GslError
   *  @return status code
   */
  virtual StatusCode handle( const GslError& error ) const = 0;

  /** retrieve the  current GSL error handler
   *  @return current GSL error handler
   */
  virtual GslErrorHandler handler() const = 0;

  /** set new GSL error handler
   *  @param  handler   new GSL error handler
   *  @return GSL error handler
   */
  virtual GslErrorHandler setHandler( GslErrorHandler handler ) const = 0;

  /** transform GSL error code to Gaudi status code
   *  @param  error GLS error code
   *  @return status code
   */
  virtual StatusCode status( const int error ) const = 0;
};

// ============================================================================
// The END
// ============================================================================
#endif // GAUDIGSL_IGSLSVC_H
