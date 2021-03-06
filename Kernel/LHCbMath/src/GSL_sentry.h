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
#ifndef GSL_SENTRY_H
#define GSL_SENTRY_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
// ============================================================================
// GSL
// ============================================================================
#include "gsl/gsl_errno.h"
// ============================================================================
namespace Gaudi {
  // ==========================================================================
  namespace Math {
    // ========================================================================
    namespace GSL {
      // ======================================================================
      /** @class GSL_Error_Handler
       *  helper class to manipulate with GSL error handlers
       *  @author Vanya BELAYEV Ivan.Belyaev@itep.ru
       */
      class GAUDI_API GSL_Error_Handler {
      public:
        // ====================================================================
        /// constructor: loc
        GSL_Error_Handler();
        /// destructor/ unlock
        ~GSL_Error_Handler();
        // ====================================================================
      private:
        // ====================================================================
        /// the "old" error handler
        gsl_error_handler_t* m_old; // the "old" error handler
        // ====================================================================
      };
      // ======================================================================
    } // namespace GSL
    // ========================================================================
  } // namespace Math
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // GSL_SENTRY_H
