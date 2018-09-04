
//-----------------------------------------------------------------------------
/** @file UTHistoBase.h
 *
 *  Header file for algorithm base class : UT::AlgBase
 *
 *  CVS Log :-
 *  $Id: UTTupleToolBase.h,v 1.2 2009-03-09 15:14:26 akeune Exp $
 *
 *  @author Matthew Needham
 *  @date   11/10/2008
 */
//-----------------------------------------------------------------------------

#ifndef UTKERNEL_UTTUPLETOOLBASE_H
#define UTKERNEL_UTTUPLETOOLBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiTupleTool.h"

// local
#include "Kernel/UTCommonBase.h"

namespace UT
{

  //-----------------------------------------------------------------------------
  /** @class AlgBase UTTupleToolBase.h UTKernel/UTTupleToolBase.h
   *
   *  Abstract base class for UT algorithms providing
   *  some basic functionality.
   *
   *  @author Matthew Needham
   *  @date   11/10/2008
   */
  //-----------------------------------------------------------------------------

  class TupleToolBase : public UT::CommonBase<GaudiTupleTool>
  {

  public:

    /// Standard constructor
    TupleToolBase(const std::string& type,
                  const std::string& name,
                  const IInterface* parent )
      : UT::CommonBase<GaudiTupleTool> ( type, name, parent ) { }

  };

}


#endif // UTKERNEL_UTHIUTOTOOLBASE_H
