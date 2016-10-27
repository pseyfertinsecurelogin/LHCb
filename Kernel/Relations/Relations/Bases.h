// ============================================================================
#ifndef RELATIONS_BASES_H 
#define RELATIONS_BASES_H 1
// ============================================================================
// Include files
// ============================================================================
namespace Relations 
{
  // ==========================================================================
  /** @struct BaseRange
   *  helper structure to make easy interactive manipulations
   *  with Reflex dictionaries 
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-06
   */
  struct BaseRange {
      virtual ~BaseRange() = default;
  } ;
  // ==========================================================================  
  /** @struct BaseTable
   *  helper structure to make easy interacyive manipulations
   *  with Reflex dictionaries 
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-06
   */
  struct BaseTable {
      virtual ~BaseTable() = default;
  } ;
  // ==========================================================================  
  /** @struct BaseWeightedTable
   *  helper structure to make easy interacyive manipulations
   *  with Reflex dictionaries 
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-02-06
   */
  struct BaseWeightedTable  : BaseTable {} ;
  // ==========================================================================  
} // end of namespace Relations 
// ============================================================================
// The END 
// ============================================================================
#endif // RELATIONS_BASES_H
// ============================================================================
