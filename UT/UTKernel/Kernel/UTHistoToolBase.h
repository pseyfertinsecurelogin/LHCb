
//-----------------------------------------------------------------------------
/** @file UTHistoBase.h
 *
 *  Header file for algorithm base class : UT::AlgBase
 *
 *  @author Andy Beiter (based on code by Matthew Needham)
 *  @date   2018-09-04
 */
//-----------------------------------------------------------------------------

#ifndef UTKERNEL_UTHIUTOTOOLBASE_H
#define UTKERNEL_UTHIUTOTOOLBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiHistoTool.h"

// local
#include "Kernel/UTCommonBase.h"

namespace UT
{

  //-----------------------------------------------------------------------------
  /** @class AlgBase UTHistoToolBase.h UTKernel/UTHistoToolBase.h
   *
   *  Abstract base class for UT algorithms providing
   *  some basic functionality.
   *
   *  @author Andy Beiter (based on code by Matthew Needham)
   *  @date   2018-09-04
   */
  //-----------------------------------------------------------------------------

  class HistoToolBase : public UT::CommonBase<GaudiHistoTool>
  {

  public:

    /// Standard constructor
    HistoToolBase(const std::string& type, 
             const std::string& name,
             const IInterface* parent )
      : UT::CommonBase<GaudiHistoTool> ( type, name, parent ) { }

  };

}


#endif // UTKERNEL_UTHIUTOTOOLBASE_H
