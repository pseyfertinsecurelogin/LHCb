/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

#pragma once

// STD
#include <limits>
#include <memory>
#include <set>
#include <sstream>
#include <string>

// Gaudi Array properties ( must be first ...)
#include "GaudiKernel/ParsersFactory.h"
#include "GaudiKernel/StdArrayAsProperty.h"

// Rich (Future) Kernel
#include "RichFutureKernel/RichAlgBase.h"

// Gaudi Functional
#include "GaudiAlg/Transformer.h"

// Rich Utils
#include "RichFutureUtils/RichDecodedData.h"
#include "RichUtils/RichHashMap.h"
#include "RichUtils/RichMap.h"

// RICH DAQ Kernel
#include "RichDAQKernel/RichDAQVersions.h"

// Event model
#include "Event/ODIN.h"
#include "Event/RawEvent.h"

// RichDet
#include "RichDet/DeRichSystem.h"

// Boost
#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"

// Old 32 bit SmartID
#include "Kernel/RichSmartID32.h"

namespace Rich::Future
{
  // namespace DAQ
  //{

  // Use the functional framework
  using namespace Gaudi::Functional;

  /** @class RawBankDecoder RichRawBankDecoder.h
   *
   *  RICH Raw bank decoder.
   *
   *  @author Chris Jones
   *  @date   2016-09-21
   */
  class RawBankDecoder final
    : public Transformer< Rich::Future::DAQ::L1Map( const LHCb::RawEvent &, const LHCb::ODIN & ),
                          Traits::BaseClass_t< AlgBase > >
  {

  public:

    /// Standard constructor
    RawBankDecoder( const std::string &name, ISvcLocator *pSvcLocator );

    /// Initialize
    StatusCode initialize() override;

    /// Algorithm execution via transform
    Rich::Future::DAQ::L1Map operator()( const LHCb::RawEvent &rawEvent,
                                         const LHCb::ODIN &    odin ) const override;

  private:

    /// Returns the RawBank version enum for the given bank
    inline Rich::DAQ::BankVersion bankVersion( const LHCb::RawBank &bank ) const
    {
      return static_cast< Rich::DAQ::BankVersion >( bank.version() );
    }

    /// Simple class to cache the PDDataBank objects
    class PDBanks
    {
    public: // the cached PD decoders

      /// Non-zero suppressed decoder (LHCb mode)
      std::unique_ptr< Rich::DAQ::PDDataBank > lhcb_nonZS;
      ///     zero suppressed decoder (LHCb mode)
      std::unique_ptr< Rich::DAQ::PDDataBank > lhcb_ZS;
      /// Non-zero suppressed decoder (ALICE mode)
      std::unique_ptr< Rich::DAQ::PDDataBank > alice_nonZS;
      ///     zero suppressed decoder (ALICE mode)
      std::unique_ptr< Rich::DAQ::PDDataBank > alice_ZS;

    public:

      /// The bank type the current decoders are for
      Rich::DAQ::BankVersion version { Rich::DAQ::UndefinedBankVersion };
    };

  private:

    /** Creates a bank data from the given raw block of data
     *
     *  @param dataStart Pointer to the start of the raw data
     *  @param dataSize  The length of the data block (excluding header HPD word)
     *  @param version   The RICH DAQ data bank version
     */
    const Rich::DAQ::PDDataBank *createDataBank( const Rich::DAQ::LongType *  dataStart,
                                                 const Rich::DAQ::BankVersion version,
                                                 PDBanks &                    banks ) const;

  private:

    /// Decode a RawBank into RichSmartID identifiers
    void decodeToSmartIDs( const LHCb::RawBank &     bank,
                           const LHCb::ODIN &        odin,
                           Rich::Future::DAQ::L1Map &decodedData,
                           PDBanks &                 banks ) const;

    /// Decode a RawBank into RichSmartID identifiers
    /// Version compatible with first 2007 "final" L1 firmware
    void decodeToSmartIDs_2007( const LHCb::RawBank &     bank,
                                const LHCb::ODIN &        odin,
                                Rich::Future::DAQ::L1Map &decodedData,
                                PDBanks &                 banks ) const;

    /// Decode a RawBank into RichSmartID identifiers
    /// MaPMT0 version
    void decodeToSmartIDs_MaPMT0( const LHCb::RawBank &     bank,
                                  Rich::Future::DAQ::L1Map &decodedData ) const;

    /// Check if a given L1 ID should be decoded
    inline bool okToDecode( const Rich::DAQ::Level1HardwareID L1ID ) const
    {
      // First check if we are decoding each RICH.
      // If so, no need to lookup RICH type from ID
      bool ok = m_richIsActive[Rich::Rich1] && m_richIsActive[Rich::Rich2];
      if ( UNLIKELY( !ok ) )
      {
        // Now lookup the RICH type
        const auto rich = m_richSys->richDetector( L1ID );
        if ( UNLIKELY( rich == Rich::InvalidDetector ) )
        {
          std::ostringstream mess;
          mess << "L1 bank " << L1ID << " has an invalid RICH detector mapping -> Bank skipped";
          Warning( mess.str() ).ignore();
          ok = false;
        }
        else
        {
          ok = m_richIsActive[rich];
        }
      }
      return ok;
    }

  private:

    /// Suppress hot pixels
    void suppressHotPixels( const LHCb::RichSmartID &  hpdID,
                            LHCb::RichSmartID::Vector &newids ) const;

  private:

    /** Print the given RawBank as a simple hex dump
     *  @param bank The RawBank to dump out
     *  @param os   The Message Stream to print to
     */
    void dumpRawBank( const LHCb::RawBank &bank, MsgStream &os ) const;

    /// Print the given data word as Hex and as bits, to the given precision
    template < class TYPE >
    inline void
    rawDump( MsgStream &os, const TYPE word, const Rich::DAQ::ShortType nBits = 32 ) const
    {
      std::ostringstream hexW;
      hexW << std::hex << word;
      std::string tmpW = hexW.str();
      if ( tmpW.size() < 8 ) { tmpW = std::string( 8 - tmpW.size(), '0' ) + tmpW; }
      os << tmpW << "  |";
      for ( int iCol = nBits - 1; iCol >= 0; --iCol ) { os << "  " << isBitOn( word, iCol ); }
    }

    /// Test if a given bit in a word is set on
    template < class TYPE >
    inline bool isBitOn( const TYPE data, const Rich::DAQ::ShortType pos ) const noexcept
    {
      return ( 0 != ( data & ( 1 << pos ) ) );
    }

  private:

    /// RICH System detector element
    const DeRichSystem *m_richSys = nullptr;

    /// Flag to turn on/off decoding of each RICH detector (default is both on)
    Gaudi::Property< Rich::DetectorArray< bool > > m_richIsActive { this,
                                                                    "Detectors",
                                                                    { true, true } };

    /// Flag to turn on/off the use of the ODIN data bank during decoding for integrity checks
    Gaudi::Property< bool > m_decodeUseOdin { this, "DecodeUsingODIN", false };

    /// Turn on/off Event ID integrity checks between the RICH ingress headers and the ODIN
    Gaudi::Property< bool > m_checkODINEventsIDs { this, "CheckODINEventIDs", false };

    /// Turn on/off internal RICH Event ID integrity checks between the ingress and HPD header words
    Gaudi::Property< bool > m_checkRICHEventsIDs { this, "CheckRICHEventIDs", true };

    /// Perform check of HPD L1 hardward ID to that expected by the DB
    Gaudi::Property< bool > m_hpdL1check { this, "CheckHPDL1IDs", false };

    /** Flag to turn on/off the purging off data from HPDs that fail the data
     *  integrity checks (default is on) */
    Gaudi::Property< bool > m_purgeHPDsFailIntegrity { this, "PurgeHPDsFailIntegrityTest", true };

    /// Flag to activate the raw printout of each Rawbank
    Gaudi::Property< bool > m_dumpBanks { this, "DumpRawBanks", false };

    /// Turn on/off BX ID integrity checks
    Gaudi::Property< bool > m_checkBxIDs { this, "CheckBXIDs", true };

    /// Turn on/off data integrity checks
    Gaudi::Property< bool > m_checkDataIntegrity { this, "CheckDataIntegrity", true };

    /// Boolean to indicate if there are any pixels that need suppressing
    bool m_pixelsToSuppress = false;

    /// Max HPD Occupancy Cut
    Gaudi::Property< Rich::DAQ::ShortType > m_maxHPDOc {
      this,
      "MaxHPDOccupancy",
      std::numeric_limits< Rich::DAQ::ShortType >::max()
    };

    /** Turn on/off detailed error messages.
     *
     *  VERY verbose in case of frequent errors
     */
    Gaudi::Property< bool > m_verboseErrors { this, "VerboseErrors", false };

  private: // pixel masking

    /// Type for hot pixel list
    using HotPixelListType = std::vector< LHCb::RichSmartID::KeyType >;

    /** Software suppression of hot channels. List of RichSmartIDs (as unsigned ints)
     *  to suppress in the data. */
    Gaudi::Property< HotPixelListType > m_hotChannels { this, "HotPixelsToMask" };

    /// Type for Storage of pixels to mask for each HPD
    typedef Rich::HashMap< LHCb::RichSmartID, std::set< LHCb::RichSmartID > > HPDHotPixels;

    /// Storage of pixels to mask for each HPD
    HPDHotPixels m_hotPixels;

  private:

    /** @brief Turns on the use of a fake HPD RichSmartID for each HPD data data block.
     *
     *  Useful for deep debugging cases when the HPD L0 ID is missing in the database.
     *
     *  @attention If set to true, decoded data is not complete (RICH,HPD panel and HPD info is
     * missing). Consequently, this option should only be used to test the data decoding and not if
     * the RichSmartIDs are needed for analysis downstream.
     */
    Gaudi::Property< bool > m_useFakeHPDID { this, "UseFakeHPDID", false };
  };

  //}
} // namespace Rich::Future
