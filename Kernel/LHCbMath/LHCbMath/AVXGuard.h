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

#if defined( __AVX__ ) || defined( __AVX2__ )
#  include <x86intrin.h>
#endif

namespace LHCb {

  /** @namespace AVX
   *
   *  Namespace for AVX handling utilities
   *
   *  @author Chris Jones
   *  @date   18/08/2017
   */
  namespace AVX {

    /** @struct Guard AVXGuard.h LHCbMath/AVXGuard.h
     *
     *  Create a guard which when goes out of scope automatically
     *  calls_mm256_zeroupper().
     *
     *  See Agner Fog's optimization guide, 12.1 about mixing AVX and non-AVX code,
     *  (http://www.agner.org/optimize/optimizing_cpp.pdf)
     *  and preserving the YMM register state.
     *  Invoking __mm256_zeroupper seems to reduce the overhead when switching.
     *
     *  @author Chris Jones
     *  @date   18/08/2017
     */
    struct Guard {
#if defined( __AVX__ ) || defined( __AVX2__ )
      ~Guard() { _mm256_zeroupper(); }
#else
      ~Guard() {}
#endif
    };

  } // namespace AVX

} // namespace LHCb
