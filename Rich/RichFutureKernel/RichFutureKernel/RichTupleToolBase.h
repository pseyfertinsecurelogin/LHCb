
//-----------------------------------------------------------------------------
/** @file RichTupleToolBase.h
 *
 *  Header file for tool base class : RichTupleToolBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   05/04/2002
 */
//-----------------------------------------------------------------------------

#ifndef RICHFUTUREKERNEL_RICHTUPLETOOLBASE_H
#define RICHFUTUREKERNEL_RICHTUPLETOOLBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiTupleTool.h"

// local
#include "RichFutureKernel/RichHistoBase.h"

// Rich Utils
#include "RichUtils/RichHistoID.h"

namespace Rich
{
  namespace Future
  {

    //-----------------------------------------------------------------------------
    /** @class TupleToolBase RichTupleToolBase.h RichFutureKernel/RichTupleToolBase.h
     *
     *  Abstract base class for RICH tools providing some basic functionality.
     *
     *  In addition, uses the histogramming and ntupling
     *  functionality from the base class GaudiTupleTool.
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   05/04/2002
     */
    //-----------------------------------------------------------------------------
    
    class TupleToolBase : public HistoBase<GaudiTupleTool>
    {
      
    public:
      
      /// Standard constructor
      TupleToolBase( const std::string& type,
                     const std::string& name,
                     const IInterface* parent )
        : HistoBase<GaudiTupleTool> ( type, name, parent )
      { }
      
    };
    
  }
}

#endif // RICHFUTUREKERNEL_RICHTUPLETOOLBASE_H
