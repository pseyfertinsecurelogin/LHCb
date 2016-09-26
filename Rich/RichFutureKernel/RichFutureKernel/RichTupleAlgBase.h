
//-----------------------------------------------------------------------------
/** @file RichTupleAlgBase.h
 *
 *  Header file for algorithm base class : RichTupleAlgBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   05/04/2002
 */
//-----------------------------------------------------------------------------

#ifndef RICHFUTUREKERNEL_RICHTUPLEALGBASE_H
#define RICHFUTUREKERNEL_RICHTUPLEALGBASE_H 1

// GaudiAlg
#include "GaudiAlg/GaudiTupleAlg.h"

// local
#include "RichFutureKernel/RichHistoBase.h"

// Rich Utils
#include "RichUtils/RichHistoID.h"

namespace Rich
{
  namespace Future
  {

    //-----------------------------------------------------------------------------
    /** @class TupleAlgBase RichTupleAlgBase.h RichFutureKernel/RichTupleAlgBase.h
     *
     *  Abstract base class for RICH algorithms providing some basic functionality.
     *
     *  In addition, uses the histogramming and ntupling
     *  functionality from the base class GaudiTupleAlg.
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   05/04/2002
     */
    //-----------------------------------------------------------------------------

    class TupleAlgBase : public HistoBase<GaudiTupleAlg>
    {

    public:

      /// Standard constructor
      TupleAlgBase( const std::string& name,
                    ISvcLocator* pSvcLocator )
        : HistoBase<GaudiTupleAlg> ( name, pSvcLocator )
      { }

    };

  }
}

#endif // RICHFUTUREKERNEL_RICHTUPLEALGBASE_H
