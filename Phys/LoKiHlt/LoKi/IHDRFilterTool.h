// ============================================================================
#ifndef LOKI_IHDRFILTERTOOL_H
#define LOKI_IHDRFILTERTOOL_H 1
// ============================================================================
// Include files 
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/ClassID.h"
// ============================================================================
// forward declarations 
// ============================================================================
namespace LHCb { class HltDecReport ; }
// ============================================================================
namespace LoKi 
{
  // ==========================================================================
  /** @class IHDRFilterTool IHDRFilterTool.h
   *  Interface class for filters on HltDecReports
   *
   *  @author Rosen Matev
   *  @date   2017-03-24
   */
  struct GAUDI_API IHDRFilterTool: extend_interfaces<IAlgTool> 
  {
  public:
    // ========================================================================
    /// access to interface id
    DeclareInterfaceID(IHDRFilterTool, 1, 0);    
    /// the predicate method
    virtual bool predicate(const LHCb::HltDecReports& hdr) const = 0;    
    // ========================================================================
  };
  // ==========================================================================
}                                                  // The end of namespace LoKi 
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_IHDRFILTERTOOL_H
// ============================================================================
