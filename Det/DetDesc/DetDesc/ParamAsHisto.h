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
#ifndef DETDESC_PARAMASHISTO_H
#define DETDESC_PARAMASHISTO_H 1
// Include files
// ============================================================================
// DetDesc
// ============================================================================
#include "DetDesc/Condition.h"
#include "DetDesc/HistoParam.h"
// ============================================================================
/** @file
 *  set of helper function for easy extraction of histograms
 *  (especially in python) from condition objects
 */
// ============================================================================
namespace DetDesc {
  // ==========================================================================
  namespace Params {
    // ========================================================================
    /** Get a read accessor to 1d histogram parameter of the Condition
     *
     *  @param cond pointer to the Condition object
     *  @param name name of the parameter
     *  @return pointer to DetDesc::Histo1D on success, NULL if cond == NULL
     *
     *  @author Dmitry Golubkov
     *  @date   2009-12-01
     */
    GAUDI_API
    const Histo1D* paramAsHisto1D( const Condition* cond, const std::string& name );
    // ========================================================================
    /** Get a read accessor to 2d histogram parameter of the Condition
     *
     *  @param cond pointer to the Condition object
     *  @param name name of the parameter
     *  @return pointer to DetDesc::Histo2D on success, NULL if cond == NULL
     *
     *  @author Dmitry Golubkov
     *  @date   2009-12-01
     */
    GAUDI_API
    const Histo2D* paramAsHisto2D( const Condition* cond, const std::string& name );
    // ========================================================================
  } // namespace Params
  // ==========================================================================
} //                                                   end of namespace DetDesc
// ============================================================================
// THE END
// ============================================================================
#endif // DETDESC_PARAMASHISTO_H
