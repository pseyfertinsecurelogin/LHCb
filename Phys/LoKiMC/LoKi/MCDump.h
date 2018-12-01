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
#ifndef LOKI_MCDUMP_H
#define LOKI_MCDUMP_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Filters.h"
#include "LoKi/MCTypes.h"
#include "LoKi/Dumper.h"
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-06-03
 *
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace Functors
  {
    // ========================================================================
    /** template specialization of ``dumper''
     *  @see LoKi::Fuctors::Dump_
     *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
     *  @date   2011-06-03
     */
    template <>
    std::vector<const LHCb::MCParticle*>
    Dump_<const LHCb::MCParticle*>::operator()
      ( const std::vector<const LHCb::MCParticle*>& a ) const ;
    // ========================================================================
    /** template specialization of ``dumper''
     *  @see LoKi::Fuctors::Dump1_
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2012-01-28
     */
    template <>
    bool
    Dump1_<const LHCb::MCParticle*,bool>::operator()
      ( const LHCb::MCParticle* a ) const ;
    // ========================================================================
    /** template specialization of ``dumper''
     *  @see LoKi::Fuctors::Dump1_
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2012-01-28
     */
    template <>
    double
    Dump1_<const LHCb::MCParticle*,double>::operator()
      ( const LHCb::MCParticle* a ) const ;
    // ========================================================================
  } //                                          end of namespace LoKi::Functors
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_MCDUMP_H
// ============================================================================
