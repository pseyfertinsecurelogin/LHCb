
//-----------------------------------------------------------------------------
/** @file RichNonZeroSuppALICEData_V2.h
 *
 *  Header file for RICH DAQ utility class : RichNonZeroSuppALICEData
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2003-11-07
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <algorithm>

// local
#include "RichDAQKernel/RichHPDDataBank.h"
#include "RichDAQKernel/RichZSPacked_V2.h"

#include "Event/ODIN.h"
// Kernel
#include "Kernel/MemPoolAlloc.h"

//===================================================================================

namespace Rich
{
  namespace DAQ
  {

    /** @namespace Rich::DAQ::RichNonZeroSuppALICEDataV2
     *
     *  Namespace for version 2 of the RichNonZeroSuppALICEData object.
     *
     *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
     *  @date   2004-12-17
     */
    namespace RichNonZeroSuppALICEDataV2
    {

      /// Import HPD sepcific parameters
      using namespace Rich::DAQ::HPD;

      /** @class RichNonZeroSuppALICEData RichNonZeroSuppALICEData_V2.h
       *
       *  The RICH HPD non zero suppressed data format for ALICE mode.
       *
       *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
       *  @date   2003-11-07
       */
      template< class Version, class Header, class Footer >
      class RichNonZeroSuppALICEData : public HPDDataBankImp<Version,Header,Footer>,
                                       public LHCb::MemPoolAlloc< RichNonZeroSuppALICEDataV2::RichNonZeroSuppALICEData<Version,Header,Footer> >
      {

      public:

        /// Default constructor
        RichNonZeroSuppALICEData() :
          HPDDataBankImp<Version,Header,Footer> ( MaxDataSizeALICE )
        { }

        /** Constructor from a RichSmartID HPD identifier and a vector of RichSmartIDs
         *
         *  @param l0ID   L0 board hardware identifier
         *  @param digits Vector of RichSmartIDs listing the active channels in this HPD
         */
        explicit RichNonZeroSuppALICEData( const Level0ID l0ID,
                                           const LHCb::RichSmartID::Vector & digits,
                                           const bool extendedFormat = false,
                                           const LHCb::ODIN * odin = NULL )
          : HPDDataBankImp<Version,Header,Footer> ( Header ( false, // Non-ZS
                                                             true,  // Is ALICE mode
                                                             extendedFormat, // data format
                                                             false, // No GT inhibit
                                                             l0ID,  // The L0 ID
                                                             EventID( odin ? odin->eventNumber() : 0 ), // Event ID
                                                             0 // filled by buildData call
                                                             ),
                                                    Footer(),
                                                    0, MaxDataSizeALICE )
        {
          buildData( digits );
        }

        /** Constructor from a block of raw data
         *
         *  @param data Pointer to the start of the data block
         */
        explicit RichNonZeroSuppALICEData( const LongType * data )
          : HPDDataBankImp<Version,Header,Footer> ( data, // start of data
                                                    MaxDataSizeALICE,
                                                    MaxDataSizeALICE )
        { }

        /** Reset for a new block of raw data
         *  @param data     Pointer to the start of the data block
         */
        void reset( const LongType * data,
                    const ShortType  dataSize = 0 ) override
        {
          HPDDataBankImp<Version,Header,Footer>::reset( data, std::max(dataSize,MaxDataSizeALICE) );
        }

        /// Destructor
        ~RichNonZeroSuppALICEData() = default;

        // Fill a vector with RichSmartIDs for hit pixels
        ShortType fillRichSmartIDs( LHCb::RichSmartID::Vector & ids,
                                    const LHCb::RichSmartID hpdID ) const override final;

      private: // methods

        /// Build data array from vector of RichSmartIDs
        void buildData( const LHCb::RichSmartID::Vector & digits );

        /// Calculates number of 8-bit words in the data
        ShortType calcEightBitword( const LHCb::RichSmartID::Vector & digits ) const;

        /// Set a pixel as active
        inline void setPixelActive( const ShortType row,
                                    const ShortType col ) noexcept
        {
          this->setBit( this->data()[this->maxDataSize()-(row+1)], col );
        }

        /// Is a given pixel active ?
        inline bool isPixelActive( const ShortType row,
                                   const ShortType col ) const noexcept
        {
          return this->isBitOn( this->data()[this->maxDataSize()-(row+1)], col );
        }

      };

    } // RichNonZeroSuppALICEDataV2 namespace

  }
}
