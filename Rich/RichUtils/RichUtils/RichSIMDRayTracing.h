
#pragma once

// RichUtils
#include "RichUtils/RichSIMDTypes.h"

// LHCbKernel
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichTraceMode.h"
#include "Kernel/RichSmartID.h"

// Vc
#include <Vc/common/alignedbase.h>

// Forward declarations
class DeRichSphMirror;
class DeRichPD;

namespace Rich::RayTracingUtils
{

  /// Return type for the vectorised raytracing
  class SIMDResult : public Vc::AlignedBase<Vc::VectorAlignment>
  {

  public:

    /// Type for SIMD array of result codes
    using Results  = Rich::SIMD::UInt32;
    /// Type for SmartIDs container.
    using SmartIDs = Rich::SIMD::STDArray<LHCb::RichSmartID>;
    /// Type for mirror pointers
    using Mirrors  = Rich::SIMD::STDArray<const DeRichSphMirror *>;
    /// Type for PDs
    using PDs      = Rich::SIMD::STDArray<const DeRichPD *>;
    /// Type for Point
    using Point    = Rich::SIMD::Point<Rich::SIMD::DefaultScalarFP>;
    /// Type for validity mask
    using Mask     = Rich::SIMD::FP<Rich::SIMD::DefaultScalarFP>::MaskType;

  public:

    /// Default constructor
    SIMDResult() = default;

  public:

    /// Ray tracing status code
    Results result = Results::Zero(); // 0 means RayTraceFailed

    /// Detection point
    Point detectionPoint;

    /// Channel ID for detection point
    SmartIDs smartID = {{}};

    /// Pointer to the associated primary mirror detector element
    Mirrors primaryMirror = {{}};

    /// Pointer to the associated secondary mirror detector element
    Mirrors secondaryMirror = {{}};

    /// Pointer to the associated DeRichPD object (if available)
    PDs photonDetector = {{}};

    /// Validity mask
    Mask valid;

  public:

    /// Container of results
    using Vector = Rich::SIMD::STDVector<SIMDResult>;

  };

}
