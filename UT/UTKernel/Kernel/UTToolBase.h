
//-----------------------------------------------------------------------------
/** @file UTHistoBase.h
 *
 *  Header file for algorithm base class : UT::AlgBase
 *
 *  CVS Log :-
 *  $Id: UTToolBase.h,v 1.2 2008-10-16 13:08:02 mneedham Exp $
 *
 *  @author Matthew Needham
 *  @date   11/10/2008
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
   *  @author Matthew Needham
   *  @date   11/10/2008
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
