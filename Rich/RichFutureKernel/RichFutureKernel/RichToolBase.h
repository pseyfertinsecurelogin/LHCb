
//-----------------------------------------------------------------------------
/** @file RichToolBase.h
 *
 *  Header file for tool base class : Rich::ToolBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   05/04/2002
 */
//-----------------------------------------------------------------------------

#ifndef RICHFUTUREKERNEL_RICHTOOLBASE_H
#define RICHFUTUREKERNEL_RICHTOOLBASE_H 1

#include <functional>
#include <string>

#include "GaudiAlg/GaudiCommonImp.h"
// GaudiAlg
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
// local
#include "RichFutureKernel/RichCommonBase.h"

class IInterface;

namespace Rich
{
  namespace Future
  {

    //-----------------------------------------------------------------------------
    /** @class ToolBase RichToolBase.h RichFutureKernel/RichToolBase.h
     *
     *  Abstract base class for RICH tools providing
     *  some basic functionality.
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   05/04/2002
     */
    //-----------------------------------------------------------------------------

    class ToolBase : public CommonBase<GaudiTool>
    {

    public:

      /// Standard constructor
      ToolBase( const std::string& type,
                const std::string& name,
                const IInterface* parent )
        : CommonBase<GaudiTool> ( type, name, parent ) { }

    };

  }
}

#endif // RICHFUTUREKERNEL_RICHTOOLBASE_H
