
//-----------------------------------------------------------------------------
/** @file RichHistoToolBase.h
 *
 *  Header file for tool base class : RichHistoToolBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   05/04/2002
 */
//-----------------------------------------------------------------------------

#ifndef RICHFUTUREKERNEL_RICHHISTOTOOLBASE_H
#define RICHFUTUREKERNEL_RICHHISTOTOOLBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiHistoTool.h"

// local
#include "RichFutureKernel/RichHistoBase.h"

// RichUtils
#include "RichUtils/RichHistoID.h"

namespace Rich
{
  namespace Future
  {

    //-----------------------------------------------------------------------------
    /** @class HistoToolBase RichHistoToolBase.h RichFutureKernel/RichHistoToolBase.h
     *
     *  Abstract base class for RICH tools providing some basic functionality.
     *
     *  In addition, uses the histogramming
     *  functionality from the base class GaudiTupleAlg.
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   05/04/2002
     */
    //-----------------------------------------------------------------------------

    class HistoToolBase : public HistoBase<GaudiHistoTool>
    {

    public:

      /// Standard constructor
      HistoToolBase( const std::string& type,
                     const std::string& name,
                     const IInterface* parent )
        : HistoBase<GaudiHistoTool> ( type, name, parent )
      { }

    };

  }
}

#endif // RICHFUTUREKERNEL_RICHHISTOTOOLBASE_H