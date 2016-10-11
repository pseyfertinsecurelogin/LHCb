// ============================================================================
// Include files 
// ============================================================================
// DetDesc 
// ============================================================================
#include "DetDesc/HistoCondition.h"
// ============================================================================
/** @file 
 *  Set the unique Class IDs for the Histogram conditions 
 *  @see CLID 
 *  @see DetDesc::Histo1DCond 
 *  @see DetDesc::Histo2DCond 
 *  @author Vanya BELYAEV Ivan.Bwlyaev@nikhef.nl
 *  @date   2009-10-22
 */
// ============================================================================
// The unique Class ID 
// ============================================================================
const CLID& DetDesc::Histo1DCond::classID () 
{
  static const CLID s_CLID = 101 ;
  return s_CLID ;
}
// ============================================================================
// The unique Class ID 
// ============================================================================
const CLID& DetDesc::Histo2DCond::classID () 
{
  static const CLID s_CLID = 102 ;
  return s_CLID ;
}
// ============================================================================
// The END 
// ============================================================================
