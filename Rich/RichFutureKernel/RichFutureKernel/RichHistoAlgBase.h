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
/** @file RichHistoAlgBase.h
 *
 *  Header file for algorithm base class : RichHistoAlgBase
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   05/04/2002
 */
//-----------------------------------------------------------------------------

#pragma once

// Gaudi
#include "Gaudi/Algorithm.h"
#include "GaudiAlg/FixTESPath.h"
#include "GaudiAlg/GaudiHistoAlg.h"

// local
#include "RichFutureKernel/RichHistoBase.h"

// RichUtils
#include "RichUtils/RichHistoID.h"

namespace Rich::Future {

  namespace Details {
    /// Basic algorithm class with minimum functionality from legacy GaudiAlg needed by GaudiHistos
    struct GAUDI_API BaseAlg : FixTESPath<Gaudi::Algorithm> {
      using FixTESPath<Gaudi::Algorithm>::FixTESPath;
      StatusCode Print( const std::string& msg,                       //
                        const StatusCode   st  = StatusCode::SUCCESS, //
                        const MSG::Level   lev = MSG::INFO ) const {
        msgStream( lev ) << msg;
        return st;
      }
      StatusCode Warning( const std::string& msg, //
                          const StatusCode   st = StatusCode::FAILURE ) const {
        warning() << msg;
        return st;
      }
      StatusCode Error( const std::string& msg, //
                        const StatusCode   st = StatusCode::FAILURE ) const {
        error() << msg;
        return st;
      }
    };
    /// Histogramming base a la GaudiAlg, but based on Gaudi::Algorithm
    struct GAUDI_API HistoBaseAlg : GaudiHistos<BaseAlg> {
      using GaudiHistos<BaseAlg>::GaudiHistos;
      using GaudiHistos<BaseAlg>::initialize;
      using GaudiHistos<BaseAlg>::finalize;
    };
  } // namespace Details

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

  class HistoAlgBase : public HistoBase<Details::HistoBaseAlg> {

  public:
    /// Standard constructor
    HistoAlgBase( const std::string& name, ISvcLocator* pSvcLocator )
        : HistoBase<Details::HistoBaseAlg>( name, pSvcLocator ) {
      this->initRichHistoConstructor();
    }
  };

} // namespace Rich::Future
