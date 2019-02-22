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
// Include files
// ============================================================================
// STD&STL
// ============================================================================
#include <cassert>
#include <limits>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/StatEntity.h"
// ============================================================================
// LHcbMath
// ============================================================================
#include "LHCbMath/NStatEntity.h"
// ===========================================================================
/** @file
 *  Implementation file for class Gaudi::Math::NStatEntity
 *  @see Gaudi::Math::NStatEntity
 *  @see Gaudi::Math::WStatEntity
 *  @see StatEntity
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   2015-04-03
 *
 *                    $Revision$
 *  Last modification $Date$
 *                 by $Author$
 */
// ===========================================================================
namespace {
  // =========================================================================
  static_assert( std::numeric_limits<unsigned long>::is_specialized,
                 "numeric_limits<unsigned long> is not specialized!" );
  // =========================================================================
  /// define the maximum half-width of a window
  const unsigned long s_max = std::numeric_limits<unsigned long>::max() << 3;
  // =========================================================================
} // namespace
// ===========================================================================
// constructor with N-parameter
// ===========================================================================
Gaudi::Math::NStatEntity::NStatEntity( const unsigned long N )
    : m_cnt1(), m_cnt2(), m_N( std::min( std::max( 1UL, N ), s_max ) ) {}
// ===========================================================================
// The END
// ===========================================================================
