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
#ifndef GAUDIGSL_GAUDIGSL_H
#define GAUDIGSL_GAUDIGSL_H 1
// Include files
#include "GaudiKernel/Kernel.h"

struct IGslSvc; // from GaudiGSL

/** @class GaudiGSL GaudiGSL.h GaudiGSL/GaudiGSL.h
 *
 *  Helper class to get (static) access to Gaudi  GSL Service
 *  General users are not supposed to use it.
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   29/04/2002
 */
struct GAUDI_API GaudiGSL {

  /** static accessor to Gaudi GSL Service
   *  @return (const) pointer to Gaudi GSL Service
   */
  static const IGslSvc* gslSvc();

  /**set new value for static Gaudi GSL Service
   *  @return (const) pointer to Gaudi GSL Service
   */
  static const IGslSvc* setGslSvc( const IGslSvc* value );
};

// ============================================================================
// The END
// ============================================================================
#endif // GAUDIGSL_GAUDIGSL_H
