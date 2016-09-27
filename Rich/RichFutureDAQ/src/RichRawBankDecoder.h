#ifndef RICHFUTUREDAQ_RICHRAWBANKDECODER_H 
#define RICHFUTUREDAQ_RICHRAWBANKDECODER_H 1

// STD
#include <sstream>
#include <string>
#include <set>
#include <memory>

// Gaudi Array properties ( must be first ...)
#include "GaudiKernel/ParsersFactory.h"
#include "GaudiKernel/StdArrayAsProperty.h"

// Rich (Future) Kernel
#include "RichFutureKernel/RichAlgBase.h"

// Gaudi Functional
#include "GaudiAlg/FunctionalUtilities.h"
#include "GaudiAlg/Transformer.h"

// Rich Utils
#include "RichUtils/RichDecodedData.h"
#include "RichUtils/RichMap.h"
#include "RichUtils/RichHashMap.h"

// RICH DAQ Kernel
#include "RichDAQKernel/RichDAQVersions.h"

// Event model
#include "Event/RawEvent.h"
#include "Event/ODIN.h"

// RichDet
#include "RichDet/DeRichSystem.h"

// Boost
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"

// Old 32 bit SmartID
#include "Kernel/RichSmartID32.h"

namespace Rich
{
  namespace Future
  {

    // Use the functional framework
    using namespace Gaudi::Functional;

    /** @class RichRawBankDecoder RichRawBankDecoder.h
     *
     *  RICH Raw bank decoder.
     *
     *  @author Chris Jones
     *  @date   2016-09-21
     */
    class RawBankDecoder final : public Transformer< Rich::DAQ::L1Map( const LHCb::RawEvent&, 
                                                                       const LHCb::ODIN& ),
                                                     Traits::BaseClass_t<AlgBase> >
    {

    public:

      /// Standard constructor
      RawBankDecoder( const std::string& name, ISvcLocator* pSvcLocator );

      /// Initialize
      StatusCode initialize() final;
      
      /// Algorithm execution via transform
      Rich::DAQ::L1Map operator()( const LHCb::RawEvent& rawEvent,
                                   const LHCb::ODIN& odin ) const final;

    private:

      /// Returns the RawBank version enum for the given bank
      inline Rich::DAQ::BankVersion bankVersion( const LHCb::RawBank & bank ) const
      {
        return static_cast< Rich::DAQ::BankVersion > ( bank.version() );
      }

      /** Creates a bank data from the given raw block of data
       *
       *  Uses static objects to avoid recreating.
       *
       *  @param dataStart Pointer to the start of the raw data
       *  @param dataSize  The length of the data block (excluding header HPD word)
       *  @param version   The RICH DAQ data bank version
       */
      std::unique_ptr<const Rich::DAQ::HPDDataBank> 
      createDataBank( const Rich::DAQ::LongType * dataStart,
                      const unsigned int dataSize,
                      const Rich::DAQ::BankVersion version ) const;
      
      /// Decode a RawBank into RichSmartID identifiers
      void decodeToSmartIDs( const LHCb::RawBank & bank,
                             const LHCb::ODIN& odin,
                             Rich::DAQ::L1Map & decodedData ) const;

      /// Decode a RawBank into RichSmartID identifiers
      /// Version with DC06 and DC04 compatibility
      void decodeToSmartIDs_DC0406( const LHCb::RawBank & bank,
                                    Rich::DAQ::L1Map & decodedData ) const;

      /// Decode a RawBank into RichSmartID identifiers
      /// Version compatible with 2006 testbeam
      void decodeToSmartIDs_2006TB( const LHCb::RawBank & bank,
                                    Rich::DAQ::L1Map & decodedData ) const;

      /// Decode a RawBank into RichSmartID identifiers
      /// Version compatible with first 2007 "final" L1 firmware
      void decodeToSmartIDs_2007( const LHCb::RawBank & bank,
                                  const LHCb::ODIN& odin,
                                  Rich::DAQ::L1Map & decodedData ) const;

      /// Decode a RawBank into RichSmartID identifiers
      /// MaPMT0 version
      void decodeToSmartIDs_MaPMT0( const LHCb::RawBank & bank,
                                    Rich::DAQ::L1Map & decodedData ) const;

    private:

      /// Suppress hot pixels
      void suppressHotPixels( const LHCb::RichSmartID& hpdID,
                              LHCb::RichSmartID::Vector & newids ) const;

    private:
      
      /** Print the given RawBank as a simple hex dump
       *  @param bank The RawBank to dump out
       *  @param os   The Message Stream to print to
       */
      void dumpRawBank( const LHCb::RawBank & bank,
                        MsgStream & os ) const;

      /// Print the given data word as Hex and as bits, to the given precision
      template < class TYPE >
      inline void rawDump( MsgStream & os,
                           const TYPE word,
                           const Rich::DAQ::ShortType nBits = 32 ) const
      {
        std::ostringstream hexW;
        hexW << std::hex << word;
        std::string tmpW = hexW.str();
        if ( tmpW.size() < 8 ) { tmpW = std::string(8-tmpW.size(),'0')+tmpW; }
        os << tmpW << "  |";
        for ( int iCol = nBits-1; iCol >= 0; --iCol )
        {
          os << "  " << isBitOn( word, iCol );
        }
      }

      /// Test if a given bit in a word is set on
      template < class TYPE >
      inline bool isBitOn( const TYPE data, const Rich::DAQ::ShortType pos ) const noexcept
      {
        return ( 0 != (data & (1<<pos)) );
      }
      
    private:

      /// RICH System detector element
      const DeRichSystem * m_richSys = nullptr;

      /// Flag to turn on/off decoding of each RICH detector (default is both on)
      Rich::DetectorArray<bool> m_richIsActive;

      /// Flag to turn on/off the use of the ODIN data bank during decoding for integrity checks
      bool m_decodeUseOdin;

      /// Turn on/off Event ID integrity checks between the RICH ingress headers and the ODIN
      bool m_checkODINEventsIDs;

      /// Turn on/off internal RICH Event ID integrity checks between the ingress and HPD header words
      bool m_checkRICHEventsIDs;

      /// Perform check of HPD L1 hardward ID to that expected by the DB
      bool m_hpdL1check;

      /** Flag to turn on/off the purging off data from HPDs that fail the data
       *  integrity checks (default is on) */
      bool m_purgeHPDsFailIntegrity;

      /// Flag to activate the raw printout of each Rawbank
      bool m_dumpBanks;

      /// Turn on/off BX ID integrity checks
      bool m_checkBxIDs;

      /// Turn on/off data integrity checks
      bool m_checkDataIntegrity;

      /// Boolean to indicate if there are any pixels that need suppressing
      bool m_pixelsToSuppress = false;

      /// Max HPD Occupancy Cut
      unsigned int m_maxHPDOc;

      /** Turn on/off detailed error messages.
       *
       *  VERY verbose in case of frequent errors
       */
      bool m_verboseErrors;

    private: // pixel masking

     typedef std::vector<LHCb::RichSmartID::KeyType> HotPixelListType;

      /** Software suppression of hot channels. List of RichSmartIDs (as unsigned ints)
       *  to suppress in the data. */
      HotPixelListType m_hotChannels;

      /// Type for Storage of pixels to mask for each HPD
      typedef Rich::HashMap< LHCb::RichSmartID, std::set<LHCb::RichSmartID> > HPDHotPixels;

      /// Storage of pixels to mask for each HPD
      HPDHotPixels m_hotPixels;

    private:

      /** @brief Turns on the use of a fake HPD RichSmartID for each HPD data data block.
       *
       *  Useful for deep debugging cases when the HPD L0 ID is missing in the database.
       *
       *  @attention If set to true, decoded data is not complete (RICH,HPD panel and HPD info is missing).
       *             Consequently, this option should only be used to test the data decoding and not if the
       *             RichSmartIDs are needed for analysis downstream.
       */
      bool m_useFakeHPDID;

    };

  }
}

#endif // RICHFUTUREDAQ_RICHRAWBANKDECODER_H
