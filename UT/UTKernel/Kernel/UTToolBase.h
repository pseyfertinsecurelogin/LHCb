
//-----------------------------------------------------------------------------
/** @file UTHistoBase.h
 *
 *  Header file for algorithm base class : UT::AlgBase
 *
 *  @author Andy Beiter
 *  @date   2018-09-04
 */
//-----------------------------------------------------------------------------

#ifndef UTKERNEL_UTTOOLBASE_H
#define UTKERNEL_UTTOOLBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiTool.h"

// local
#include "Kernel/UTCommonBase.h"

namespace UT
{

  //-----------------------------------------------------------------------------
  /** @class AlgBase UTToolBase.h UTKernel/UTToolBase.h
   *
   *  Abstract base class for UT algorithms providing
   *  some basic functionality.
   *
   *  @author Andy Beiter (based on code by Matthew Needham)
   *  @date   2018-09-04
   */
  //-----------------------------------------------------------------------------

  class ToolBase : public UT::CommonBase<GaudiTool>
  {

  public:

    /// Standard constructor
    ToolBase(const std::string& type, 
             const std::string& name,
             const IInterface* parent )
      : UT::CommonBase<GaudiTool> ( type, name, parent ) { }

  };

}


#endif // UTKERNEL_UTALGBASE_H
