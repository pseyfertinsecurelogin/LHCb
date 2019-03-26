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
#ifndef LHCBMATH_CHOOSE_H
#define LHCBMATH_CHOOSE_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
// ============================================================================
namespace Gaudi {
  // ==========================================================================
  namespace Math {
    // ========================================================================
    /** calculate the binomial coefficient C(n,k) = n!/((n-k)!*k!)
     *  the result is exact for all n,k<=67
     *  @warning In case of overflow std::numeric_limits<unsigned long long>::max is returned
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-03-08
     */
    GAUDI_API
    unsigned long long choose( const unsigned short n, const unsigned short k );
    // ========================================================================
    /** calculate the logarithm of binomial coefficient
     *  \f$ \log C^n_k \f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-03-08
     */
    GAUDI_API
    double log_choose( const unsigned short n, const unsigned short k );
    // ========================================================================
    /** calculate the binomial coefficient C(k,n) = n!/((n-k)!*k!)
     *  @author Vanya BELYAEV Ivan.Belyaev@irep.ru
     *  @date 2015-03-08
     */
    GAUDI_API
    double choose_double( const unsigned short n, const unsigned short k );
    // ========================================================================
    /** calculate the generalized binomial coefficient C(a,k)
     *  \f$C(\alpha,k) = \frac{\alpha}{k}\frac{\alpha-1}{k-1}...\f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-03-08
     */
    GAUDI_API
    double gen_choose( const double a, const unsigned short k );
    // ========================================================================
    /** calculate the generalized binomial coefficient C(n/2,k)
     *  \f$C(n,k) = \frac{n/2}{k}\frac{n/2-1}{k-1}...\f$
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-03-08
     */
    GAUDI_API
    double choose_half( const int n, const unsigned short k );
    // ========================================================================
  } // namespace Math
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LHCBMATH_CHOOSE_H
