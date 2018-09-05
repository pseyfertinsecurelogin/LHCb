
//-----------------------------------------------------------------------------
/** @file UTHistoBase.h
 *
 *  Header file for algorithm base class : UT::AlgBase
 *
 *
 *  @author Andy Beiter (based on code by Matthew Needham)
 *  @date   2018-09-04
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
   *  @author Andy Beiter (based on code by Matthew Needham)
   *  @date   2018-09-04
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
