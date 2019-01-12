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
#ifndef LOKI_IHDRFILTERTOOL_H
#define LOKI_IHDRFILTERTOOL_H 1

#include "Event/HltDecReports.h"

#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/ClassID.h"

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
