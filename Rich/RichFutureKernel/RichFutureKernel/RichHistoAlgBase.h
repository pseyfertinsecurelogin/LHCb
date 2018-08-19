
//-----------------------------------------------------------------------------
/** @file RichHistoAlgBase.h
 *
 *  Header file for algorithm base class : RichHistoAlgBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   05/04/2002
 */
//-----------------------------------------------------------------------------

#pragma once

// GaudiAlg
#include "GaudiAlg/GaudiHistoAlg.h"

// local
#include "RichFutureKernel/RichHistoBase.h"

// RichUtils
#include "RichUtils/RichHistoID.h"

namespace Rich::Future
{

  //-----------------------------------------------------------------------------
  /** @class HistoAlgBase RichHistoAlgBase.h RichFutureKernel/RichHistoAlgBase.h
   *
   *  Abstract base class for RICH algorithms providing some basic functionality.
   *
   *  In addition, uses the histogramming
   *  functionality from the base class GaudiHistoAlg.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   05/04/2002
   */
  //-----------------------------------------------------------------------------

  class HistoAlgBase : public HistoBase< GaudiHistoAlg >
  {

  public:
    /// Standard constructor
    HistoAlgBase( const std::string &name, ISvcLocator *pSvcLocator )
      : HistoBase< GaudiHistoAlg >( name, pSvcLocator )
    {}
  };

} // namespace Rich::Future
