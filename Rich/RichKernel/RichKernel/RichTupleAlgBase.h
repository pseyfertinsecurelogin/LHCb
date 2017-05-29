
//-----------------------------------------------------------------------------
/** @file RichTupleAlgBase.h
 *
 *  Header file for algorithm base class : RichTupleAlgBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   05/04/2002
 */
//-----------------------------------------------------------------------------

#ifndef RICHKERNEL_RICHTUPLEALGBASE_H
#define RICHKERNEL_RICHTUPLEALGBASE_H 1

#include <functional>
#include <string>

#include "GaudiAlg/GaudiCommonImp.h"
// GaudiAlg
#include "GaudiAlg/GaudiTupleAlg.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
// local
#include "RichKernel/RichHistoBase.h"
// Rich Utils
#include "RichUtils/RichHistoID.h"

class ISvcLocator;

namespace Rich
{

  //-----------------------------------------------------------------------------
  /** @class TupleAlgBase RichTupleAlgBase.h RichKernel/RichTupleAlgBase.h
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

  class TupleAlgBase : public Rich::HistoBase<GaudiTupleAlg>
  {

  public:

    /// Standard constructor
    TupleAlgBase( const std::string& name,
                  ISvcLocator* pSvcLocator )
      : Rich::HistoBase<GaudiTupleAlg> ( name, pSvcLocator )
    { }

  };

}

#endif // RICHKERNEL_RICHTUPLEALGBASE_H
