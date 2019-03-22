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
#ifndef GAUDIMATH_INDEFINITEINTEGRAL_H
#define GAUDIMATH_INDEFINITEINTEGRAL_H 1
// ============================================================================
// Include files
// ============================================================================
#include "GaudiKernel/Kernel.h"

/** @class IndefiniteIntegral IndefiniteIntegral.h GaudiMath/IndefiniteIntegral.h
 *
 *  @author Ivan BELYAEV
 *  @date   2003-08-31
 */
class GAUDI_API IndefiniteIntegral {
public:
  /// Standard constructor
  IndefiniteIntegral();

  virtual ~IndefiniteIntegral(); ///< Destructor
};
#endif // GAUDIMATH_INDEFINITEINTEGRAL_H
