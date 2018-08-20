
//-----------------------------------------------------------------------------
/** @file RichAlgBase.h
 *
 *  Header file for algorithm base class : Rich::AlgBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   05/04/2002
 */
//-----------------------------------------------------------------------------

#pragma once

// GaudiAlg
#include "GaudiAlg/GaudiAlgorithm.h"

// local
#include "RichFutureKernel/RichCommonBase.h"

namespace Rich::Future
{

  //-----------------------------------------------------------------------------
  /** @class AlgBase RichAlgBase.h RichFutureKernel/RichAlgBase.h
   *
   *  Abstract base class for RICH algorithms providing
   *  some basic functionality.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   05/04/2002
   */
  //-----------------------------------------------------------------------------

  class AlgBase : public CommonBase< GaudiAlgorithm >
  {

  public:
    /// Standard constructor
    AlgBase( const std::string &name, ISvcLocator *pSvcLocator )
      : CommonBase< GaudiAlgorithm >( name, pSvcLocator )
    {}
  };

} // namespace Rich::Future
