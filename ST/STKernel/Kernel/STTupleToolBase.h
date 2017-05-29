
//-----------------------------------------------------------------------------
/** @file STHistoBase.h
 *
 *  Header file for algorithm base class : ST::AlgBase
 *
 *  CVS Log :-
 *  $Id: STTupleToolBase.h,v 1.2 2009-03-09 15:14:26 akeune Exp $
 *
 *  @author Matthew Needham
 *  @date   11/10/2008
 */
//-----------------------------------------------------------------------------

#ifndef STKERNEL_STTUPLETOOLBASE_H
#define STKERNEL_STTUPLETOOLBASE_H 1

#include <functional>
#include <string>

#include "/tmp/foobarpath/LHCb/ST/STKernel/src/STCommonBase.icpp"
#include "GaudiAlg/GaudiCommonImp.h"
// GaudiAlg
#include "GaudiAlg/GaudiTupleTool.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
// local
#include "Kernel/STCommonBase.h"

class IInterface;

namespace ST
{

  //-----------------------------------------------------------------------------
  /** @class AlgBase STTupleToolBase.h STKernel/STTupleToolBase.h
   *
   *  Abstract base class for ST algorithms providing
   *  some basic functionality.
   *
   *  @author Matthew Needham
   *  @date   11/10/2008
   */
  //-----------------------------------------------------------------------------

  class TupleToolBase : public ST::CommonBase<GaudiTupleTool>
  {

  public:

    /// Standard constructor
    TupleToolBase(const std::string& type,
                  const std::string& name,
                  const IInterface* parent )
      : ST::CommonBase<GaudiTupleTool> ( type, name, parent ) { }

  };

}


#endif // STKERNEL_STHISTOTOOLBASE_H
