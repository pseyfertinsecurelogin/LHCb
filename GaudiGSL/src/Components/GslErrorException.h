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
#ifndef GAUDIGSL_GSLERRORException_H
#define GAUDIGSL_GSLERRORException_H 1
// Include files
// from STL
#include <string>
// from GaudiKernel
#include "GaudiKernel/AlgTool.h"
// from GaudiGSL
#include "GaudiGSL/GslError.h"
#include "GaudiGSL/IGslErrorHandler.h"
// forward declaration

/** @class GslErrorException GslErrorException.h
 *
 *  Concrete GSL eror handler
 *  It is just thrown the exception
 *
 *  @attention The error handling could be "turned off"
 *  for selected error codes (e.g. GSL_SUCCESS or GSL_CONTINUE )
 *  using "IgnoreCodes" property
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   30/04/2002
 */

class GslErrorException : public extends<AlgTool, IGslErrorHandler> {
public:
  /** handle the GSL error
   *  @see IGslErrorHandler
   *  @param error  error to be handled
   *  @see GslError
   *  @return status code
   */
  StatusCode handle( const GslError& error ) const override;

  /// Inherited constructor
  using extends::extends;

private:
  Gaudi::Property<std::vector<int>> m_ignore{this, "IgnoreCodes", {}, "codes to be ignored"};
};

// ============================================================================
// The END
// ============================================================================
#endif // GAUDIGSL_GSLERRORException_H
