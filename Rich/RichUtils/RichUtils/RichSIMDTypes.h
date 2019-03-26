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

#pragma once

// LHCb SIMD Type
#include "LHCbMath/SIMDTypes.h"

// Kernel
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichSide.h"

namespace Rich {

  /** @namespace Rich::SIMD
   *
   *  Namespace for RICH SIMD types and utilities
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   17/10/2017
   */
  namespace SIMD {

    // import from LHCbMath
    using namespace LHCb::SIMD;

    //------------------------------------------------------------------------------------------------
    // SIMD Representations of various RICH enums
    //------------------------------------------------------------------------------------------------

    /// Container for detector sides
    using Sides = Int32;

  } // namespace SIMD
} // namespace Rich
