
#pragma once

/** @namespace FPE
 *
 *  Namespace for AVX handling utilities
 *
 *  @author Chris Jones
 *  @date   18/08/2017
 */
namespace AVX
{
  
  /** @struct Guard AVXGuard.h LHCbKernel/AVXGuard.h
   *
   *  Create a guard which when goes out of scope automatically 
   *  calls_mm256_zeroupper().
   *
   *  see Agner Fog's optimization guide, 12.1 about mixing AVX and non-AVX code,
   *  (http://www.agner.org/optimize/optimizing_cpp.pdf)
   *  and preserving the YMM register state.
   *  Invoking __mm256_zeroupper seems to reduce the overhead when switching.
   *
   *  @author Chris Jones
   *  @date   18/08/2017
   */
  struct Guard
  {
#if defined(__AVX__) || defined(__AVX2__)
  ~avx_guard() { _mm256_zeroupper(); }
#else
  ~avx_guard() { }
#endif
  };

}
