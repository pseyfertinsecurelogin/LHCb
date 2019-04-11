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
// ============================================================================
// Include files
// GaudiKernel
#include "GaudiKernel/GaudiException.h"
// GaudiGSL
#include "GaudiGSL/GslError.h"
#include "GaudiGSL/IGslSvc.h"
// local
#include "GaudiGSL/GaudiGSL.h"
#include "GaudiGSL/GslErrorHandlers.h"

#include <sstream>

/** @file
 *
 *  Implementation file for functionns form GslErrorHandlers namespace
 *  @see GslErrorHandlers
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   29/04/2002
 */

void GslErrorHandlers::ignoreTheError( const char* /* reason */, const char* /* file   */, int /* line   */,
                                       int /* code   */ ) {}

void GslErrorHandlers::handleTheError( const char* reason, const char* file, int line, int code ) {
  // het the GSL service
  const IGslSvc* svc = GaudiGSL::gslSvc();
  // handle the error if service is valid
  if ( nullptr != svc ) { svc->handle( GslError( reason, file, line, code ) ); }
}

void GslErrorHandlers::throwException( const char* reason, const char* file, int line, int code ) {
  std::ostringstream error;
  error << " GSL ErrorCode=" << code << ": '" << reason << "' in the file '" << file << "' at the line " << line;
  throw GaudiException( error.str(), "*GLS Error*", StatusCode::FAILURE );
}

// ============================================================================
// The END
// ============================================================================
