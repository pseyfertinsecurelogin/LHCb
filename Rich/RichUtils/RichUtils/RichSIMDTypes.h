
#pragma once

// LHCb SIMD Type
#include "LHCbMath/SIMDTypes.h"

// Kernel
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichSide.h"

namespace Rich
{

  /** @namespace Rich::SIMD
   *
   *  Namespace for RICH SIMD types and utilities
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   17/10/2017
   */
  namespace SIMD
  {

    // import from LHCbMath
    using namespace LHCb::SIMD;

    //------------------------------------------------------------------------------------------------
    // SIMD Representations of various RICH enums
    //------------------------------------------------------------------------------------------------

    /// Container for detector sides
    using Sides = Int32;

  } // namespace SIMD
} // namespace Rich
