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
/** @file RawBufferToRichDigitsAlg.h
 *
 *  Header file for RICH DAQ algorithm : Rich::DAQ::RawBufferToRichDigitsAlg
 *
 *  @author Chris Jones       Christopher.Rob.Jones@cern.ch
 *  @date   2003-11-06
 */
//-----------------------------------------------------------------------------

#pragma once

// base class
#include "RichKernel/RichAlgBase.h"

// Event
#include "Event/RichDigit.h"

// Interfaces
#include "RichInterfaces/IRichRawBufferToSmartIDsTool.h"

namespace Rich::DAQ {

  //-----------------------------------------------------------------------------
  /** @class RawBufferToRichDigitsAlg RawBufferToRichDigitsAlg.h
   *
   *  Algorithm to create RichDigits from RawEvent object
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2003-11-06
   */
  //-----------------------------------------------------------------------------

  class RawBufferToRichDigitsAlg final : public AlgBase {

  public:
    /// Standard constructor
    RawBufferToRichDigitsAlg( const std::string& name, ISvcLocator* pSvcLocator );

    virtual ~RawBufferToRichDigitsAlg(); ///< Destructor

    StatusCode initialize() override; // Algorithm initialization
    StatusCode execute() override;    // Algorithm execution

  private: // data
    /// Output location for RichDigits
    std::string m_richDigitsLoc;

    /// Raw Buffer Decoding tool
    const IRawBufferToSmartIDsTool* m_decoder = nullptr;

    /// Flag to turn off RichDigit creation
    bool m_decodeOnly;
  };

} // namespace Rich::DAQ
