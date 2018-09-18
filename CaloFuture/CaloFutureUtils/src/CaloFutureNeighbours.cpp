// ============================================================================
// Include files 
// ============================================================================
// STD & STL 
// ============================================================================
#include<set>
#include<vector>
// ============================================================================
// CaloDet
// ============================================================================
#include "CaloDet/DeCalorimeter.h"
// ============================================================================
// local
// ============================================================================
#include "CaloFutureUtils/CaloFutureNeighbours.h"
// ============================================================================
namespace 
{
  // ==========================================================================
  inline 
  bool _neighbours 
  ( LHCb::CaloCellID::Set&      cells    , 
    const unsigned int          level    , 
    const DeCalorimeter*        detector ) 
  {
    if ( 0 == detector ) { return false ; }                           // RETURN   
    if ( 0 >= level    ) { return true  ; }                           // RETURN 
    if ( cells.empty() ) { return true  ; }                           // RETURN 
    // local copy:
    std::set<LHCb::CaloCellID> local ( cells ) ;
    for ( auto iloc = local.begin() ; local.end() != iloc ; ++iloc ) 
    {
      const LHCb::CaloCellID::Vector& nei 
        = detector->neighborCells ( *iloc ) ;
      cells.insert ( nei.begin() , nei.end() ) ;
    }
    return _neighbours ( cells , level - 1 , detector ) ;         // RECURSION!
  }
  // ==========================================================================
} //                                                 end of anonymous namespace 
// ============================================================================
/*  find all neighbours for the given set of cells for the givel level 
 *  @param cells    (UPDATE) list of cells 
 *  @param level    (INPUT)  level
 *  @param detector (INPUT) the detector
 *  @return true if neighbours are added 
 */
// ============================================================================
bool LHCb::CaloFutureFunctors::neighbours 
( LHCb::CaloCellID::Set&      cells    , 
  const unsigned int          level    , 
  const DeCalorimeter*        detector ) 
{
  return _neighbours ( cells , level , detector ) ;
}
// ============================================================================
/*  find all neighbours for the given set of cells for the givel level 
 *  @param cells    (UPDATE) list of cells 
 *  @param level    (INPUT)  level
 *  @param detector (INPUT)  the detector
 *  @return true if neighbours are added 
 */
// ============================================================================
bool LHCb::CaloFutureFunctors::neighbours 
( LHCb::CaloCellID::Vector&   cells    , 
  const unsigned int          level    , 
  const DeCalorimeter*        detector ) 
{
  if ( 0 == detector ) { return false ; }                             // RETURN 
  if ( 0 == level    ) { return true  ; }                             // RETURN 
  if ( cells.empty() ) { return true  ; }                             // RETURN 
  // local copy:
  LHCb::CaloCellID::Set local ;
  local.insert ( cells.begin() , cells.end()  ) ;
  const bool result = _neighbours ( local , level , detector ) ;
  if ( !result       ) { return false ; }                             // RETURN
  //
  cells.clear() ;
  cells.insert ( cells.begin() , local.begin() , local.end() ) ;
  return true ;                                                       // RETURN 
}
// ============================================================================
// The END 
// ============================================================================
