#ifndef LOKI_IHDRFILTERTOOL_H
#define LOKI_IHDRFILTERTOOL_H 1

#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/ClassID.h"

/** @class IHDRFilterTool IHDRFilterTool.h
 *  Interface class for filters on HltDecReports
 *
 *  @author Rosen Matev
 *  @date   2017-03-24
 */

namespace LoKi {

struct GAUDI_API IHDRFilterTool: extend_interfaces<IAlgTool> {
  //class GAUDI_API IHDRFilterTool: virtual public IAlgTool {
  public:
    /// access to interface id
    DeclareInterfaceID(IHDRFilterTool, 1, 0);

    /// the predicate method
    virtual bool predicate(const LHCb::HltDecReports& hdr) const = 0;

  };
}

#endif // LOKI_IHDRFILTERTOOL_H
