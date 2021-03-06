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
#ifndef GAUDIGSL_GSLERRORCOUNT_H
#define GAUDIGSL_GSLERRORCOUNT_H 1
// Include files
// from STL
#include <map>
#include <string>
// from GaudiKernel
#include "GaudiKernel/AlgTool.h"
// from GaudiGSL
#include "GaudiGSL/GslError.h"
#include "GaudiGSL/IGslErrorHandler.h"
// forward declaration

/** @class GslErrorCount GslErrorCount.h
 *
 *  Concrete GSL error handler
 *  It is just counts number of GSL errors
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   30/04/2002
 */

class GslErrorCount : public extends<AlgTool, IGslErrorHandler> {
public:
  /** handle the GSL error
   *  @see IGslErrorHandler
   *  @param error  error to be handled
   *  @see GslError
   *  @return status code
   */
  StatusCode handle( const GslError& error ) const override;

  /** standard finalization of Tool
   *  @see  AlgTool
   *  @see IAlgTool
   *  @return status code
   */
  StatusCode finalize() override;

  /** Standard constructor
   */
  using base_class::base_class;

private:
  /// container of error counters
  typedef std::map<GslError, unsigned int> Counters;
  mutable Counters                         m_counters;
};

// ============================================================================
// The END
// ============================================================================
#endif // GAUDIGSL_GSLERRORCOUNT_H
