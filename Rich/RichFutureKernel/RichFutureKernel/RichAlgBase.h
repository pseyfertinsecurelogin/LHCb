/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

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

// Gaudi
#include "GaudiAlg/FixTESPath.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/Algorithm.h"

// local
#include "RichFutureKernel/RichCommonBase.h"

namespace Rich::Future {

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

  template <typename PBASE = FixTESPath<Gaudi::Algorithm>>
  class AlgBase : public CommonBase<PBASE> {
  public:
    // inherit constructors from base
    using CommonBase<PBASE>::CommonBase;
  };

} // namespace Rich::Future
