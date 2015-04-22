// $Id:$ 
// ============================================================================
// Include files 
// ============================================================================
// STD&STL 
// ============================================================================
#include <limits>
#include <cassert>
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
 *                    $Revision:$
 *  Last modification $Date:$
 *                 by $Author:$
 */
// ===========================================================================
namespace 
{
  // =========================================================================
  static_assert ( std::numeric_limits<unsigned long>::is_specialized   , 
                  "numeric_limits<unsigned long> is not specialized!" ) ;
  // =========================================================================
  /// define the maximum half-width of a window 
  const unsigned long s_max = std::numeric_limits<unsigned long>::max() << 3 ;
  // =========================================================================
}
// ===========================================================================
// constructor with N-parameter  
// ===========================================================================
Gaudi::Math::NStatEntity::NStatEntity ( const unsigned long N ) 
  : m_cnt1 () 
  , m_cnt2 () 
  , m_N    ( std::min ( std::max ( 1UL , N ) , s_max ) )
{}
// ===========================================================================
// The END 
// ===========================================================================
