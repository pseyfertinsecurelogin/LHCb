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
#ifndef LHCBMATH_BINOMIAL_H
#define LHCBMATH_BINOMIAL_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <utility>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
// ============================================================================
/** @file
 *  Collection of functions to estimate the confidence intervals for
 *  binomial proportion/efficiency
 *  @see http://en.wikipedia.org.wiki/Binomial_proportion_connfidence_interval
 *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
 *  @date 2015-09-17
 */
// ============================================================================
namespace Gaudi {
  // ==========================================================================
  namespace Math {
    // ========================================================================
    /** normal approximation interval for binomial proportion/efficiency
     *  ( "Wald test")
     *  @param  accepted  number of accepted events
     *  @param  rejected  number of rejected events
     *  @param  conflevel the confidence level:    0<=CL<=1
     *  @return the confidence interval
     *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
     *  @date 2015-09-17
     *  @see http://en.wikipedia.org.wiki/Binomial_proportion_connfidence_interval
     */
    GAUDI_API
    std::pair<double, double> wald_interval( const unsigned long accepted, const unsigned long rejected,
                                             const double conflevel );
    // ========================================================================
    /** Wilson score interval for binomial proportion/efficiency
     *  @param  accepted  number of accepted events
     *  @param  rejected  number of rejected events
     *  @param  conflevel the confidence level:    0<=CL<=1
     *  @return the confidence interval
     *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
     *  @date 2015-09-17
     *  @see http://en.wikipedia.org.wiki/Binomial_proportion_connfidence_interval
     */
    GAUDI_API
    std::pair<double, double> wilson_score_interval( const unsigned long accepted, const unsigned long rejected,
                                                     const double conflevel );
    // ========================================================================
    /** Wilson score interval with continuity correction for binomial proportion/efficiency
     *  @param  accepted  number of accepted events
     *  @param  rejected  number of rejected events
     *  @param  conflevel the confidence level:    0<=CL<=1
     *  @return the confidence interval
     *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
     *  @date 2015-09-17
     *  @see http://en.wikipedia.org.wiki/Binomial_proportion_connfidence_interval
     */
    GAUDI_API
    std::pair<double, double> wilson_score_continuity_interval( const unsigned long accepted,
                                                                const unsigned long rejected, const double conflevel );
    // ========================================================================
    /** ArcSin interval with continuity correction for binomial proportion/efficiency
     *  @param  accepted  number of accepted events
     *  @param  rejected  number of rejected events
     *  @param  conflevel the confidence level:    0<=CL<=1
     *  @return the confidence interval
     *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
     *  @date 2015-09-17
     *  @see http://en.wikipedia.org.wiki/Binomial_proportion_connfidence_interval
     */
    GAUDI_API
    std::pair<double, double> arcsin_interval( const unsigned long accepted, const unsigned long rejected,
                                               const double conflevel );
    // ========================================================================
    /** Agresti-Coull interval with continuity correction for binomial proportion/efficiency
     *  @param  accepted  number of accepted events
     *  @param  rejected  number of rejected events
     *  @param  conflevel the confidence level:    0<=CL<=1
     *  @return the confidence interval
     *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
     *  @date 2015-09-17
     *  @see http://en.wikipedia.org.wiki/Binomial_proportion_connfidence_interval
     */
    GAUDI_API
    std::pair<double, double> agresti_coull_interval( const unsigned long accepted, const unsigned long rejected,
                                                      const double conflevel );
    // ========================================================================
    /** Jeffreys interval for binomial proportion/efficiency
     *  @param  accepted  number of accepted events
     *  @param  rejected  number of rejected events
     *  @param  conflevel the confidence level:    0<=CL<=1
     *  @return the confidence interval
     *  @see http://en.wikipedia.org.wiki/Binomial_proportion_connfidence_interval
     *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
     *  @date 2015-09-17
     *  @see http://en.wikipedia.org.wiki/Binomial_proportion_connfidence_interval
     */
    GAUDI_API
    std::pair<double, double> jeffreys_interval( const unsigned long accepted, const unsigned long rejected,
                                                 const double conflevel );
    // ========================================================================
    /** Clopper-Pearson interval for binomial proportion/efficiency
     *  @param  accepted  number of accepted events
     *  @param  rejected  number of rejected events
     *  @param  conflevel the confidence level:    0<=CL<=1
     *  @return the confidence interval
     *  @see http://en.wikipedia.org.wiki/Binomial_proportion_connfidence_interval
     *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
     *  @date 2015-09-17
     *  @see http://en.wikipedia.org.wiki/Binomial_proportion_connfidence_interval
     */
    GAUDI_API
    std::pair<double, double> clopper_pearson_interval( const unsigned long accepted, const unsigned long rejected,
                                                        const double conflevel );
    // ========================================================================
  } // namespace Math
  // ==========================================================================
} //                                                     end of namespace Gaudi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LHCBMATH_BINOMIAL_H
