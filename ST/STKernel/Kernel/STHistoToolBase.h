
//-----------------------------------------------------------------------------
/** @file STHistoBase.h
 *
 *  Header file for algorithm base class : ST::AlgBase
 *
 *  CVS Log :-
 *  $Id: STHistoToolBase.h,v 1.2 2008-10-16 13:08:02 mneedham Exp $
 *
 *  @author Matthew Needham
 *  @date   11/10/2008
 */
//-----------------------------------------------------------------------------

#ifndef STKERNEL_STHISTOTOOLBASE_H
#define STKERNEL_STHISTOTOOLBASE_H 1

#include <functional>
#include <string>

#include "/tmp/foobarpath/LHCb/ST/STKernel/src/STCommonBase.icpp"
#include "GaudiAlg/GaudiCommonImp.h"
// GaudiAlg
#include "GaudiAlg/GaudiHistoTool.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
// local
#include "Kernel/STCommonBase.h"

class IInterface;

namespace ST
{

  //-----------------------------------------------------------------------------
  /** @class AlgBase STHistoToolBase.h STKernel/STHistoToolBase.h
   *
   *  Abstract base class for ST algorithms providing
   *  some basic functionality.
   *
   *  @author Matthew Needham
   *  @date   11/10/2008
   */
  //-----------------------------------------------------------------------------

  class HistoToolBase : public ST::CommonBase<GaudiHistoTool>
  {

  public:

    /// Standard constructor
    HistoToolBase(const std::string& type, 
             const std::string& name,
             const IInterface* parent )
      : ST::CommonBase<GaudiHistoTool> ( type, name, parent ) { }

  };

}


#endif // STKERNEL_STHISTOTOOLBASE_H
