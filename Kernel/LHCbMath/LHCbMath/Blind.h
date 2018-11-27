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
#ifndef LHCBMATH_BLIND_H
#define LHCBMATH_BLIND_H
// ===========================================================================
// Include files
// ===========================================================================
// STD & STL
// ===========================================================================
#include <string>
// ===========================================================================
// GaudiKernel
// ===========================================================================
#include "GaudiKernel/Kernel.h"
// ===========================================================================
namespace Gaudi
{
  // =========================================================================
  namespace Math
  {
    // =======================================================================
    /** get the 'blind'-number form the string
     *  @param name the name to be used for construction of the coefficient
     *  @param minv the minimal value
     *  @param minv the maximal value
     *  @return the value that can be used for blinding
     *  @author Vanya Belyaev Ivan.Belyaev@nikhef.nl
     *  @date 2010-10-10
     */
    GAUDI_API double blind
    ( const std::string& name ,
      const double       minv ,
      const double       maxv ) ;
    // ========================================================================
  } //                                             end of namespace Gaudi::Math
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
// The END
// ============================================================================
#endif // LHCBMATH_BLIND_H
// ============================================================================
