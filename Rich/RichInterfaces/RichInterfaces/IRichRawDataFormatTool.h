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
/** @file IRichRawDataFormatTool.h
 *
 *  Header file for tool interface : Rich::DAQ::IRawDataFormatTool
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2004-12-18
 */
//-----------------------------------------------------------------------------

#pragma once

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Kernel
#include "Kernel/RichSmartID.h"

// RichKernel
#include "RichUtils/RichDecodedData.h"

// forward declarations
namespace LHCb {
  class RawBank;
}

namespace Rich::DAQ {

  /** @class IRawDataFormatTool IRichRawDataFormatTool.h RichKernel/IRichRawDataFormatTool.h
   *
   *  Interface to tool to encode and decode the Raw Buffer for the RICH
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   2004-12-18
   */

  class IRawDataFormatTool : virtual public IAlgTool {

  public:
    /// Type for Input location(s) for RawEvent in TES
    using RawEventLocations = std::vector<std::string>;

  public:
    /// Interface ID
    DeclareInterfaceID( IRawDataFormatTool, 1, 0 );

    /** Fills the RICH RawEvent from a vector of RichSmartIDs
     *
     *  @param smartIDs  Vector of RichSmartIDs
     *  @param version The RICH DAQ data bank version
     */
    virtual void fillRawEvent( const LHCb::RichSmartID::Vector& smartIDs,
                               const Rich::DAQ::BankVersion     version = Rich::DAQ::LHCb2 ) const = 0;

    /** Decode all RICH RawBanks in the given events into the RICH decoded data structure
     *
     *  @param[in] taeLocations Vector of TAE locations to decode
     *  @param decodedData The RICH data structure to fill
     */
    virtual void decodeToSmartIDs( const RawEventLocations& taeLocations, Rich::DAQ::L1Map& decodedData ) const = 0;

    /** Decode all RICH RawBanks in the main event into the RICH decoded data structure
     *
     *  @param decodedData The RICH data structure to fill
     */
    inline void decodeToSmartIDs( Rich::DAQ::L1Map& decodedData ) const {
      static const RawEventLocations loc( 1, "" );
      return decodeToSmartIDs( loc, decodedData );
    }
  };

} // namespace Rich::DAQ
