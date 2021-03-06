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
/** @file RichTupleAlgBase.h
 *
 *  Header file for algorithm base class : RichTupleAlgBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   05/04/2002
 */
//-----------------------------------------------------------------------------

#pragma once

// GaudiAlg
#include "GaudiAlg/GaudiTupleAlg.h"

// local
#include "RichFutureKernel/RichHistoBase.h"

// Rich Utils
#include "RichUtils/RichHistoID.h"

namespace Rich::Future {

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

  class TupleAlgBase : public HistoBase<GaudiTupleAlg> {

  public:
    /// Standard constructor
    TupleAlgBase( const std::string& name, ISvcLocator* pSvcLocator ) : HistoBase<GaudiTupleAlg>( name, pSvcLocator ) {
      this->initRichHistoConstructor();
      this->initRichTupleConstructor();
    }
  };

} // namespace Rich::Future
