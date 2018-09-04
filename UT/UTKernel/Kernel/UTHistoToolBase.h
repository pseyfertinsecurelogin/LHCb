
//-----------------------------------------------------------------------------
/** @file UTHistoBase.h
 *
 *  Header file for algorithm base class : UT::AlgBase
 *
 *  CVS Log :-
 *  $Id: UTHistoToolBase.h,v 1.2 2008-10-16 13:08:02 mneedham Exp $
 *
 *  @author Matthew Needham
 *  @date   11/10/2008
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
   *  @author Matthew Needham
   *  @date   11/10/2008
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
