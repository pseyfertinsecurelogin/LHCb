
//-----------------------------------------------------------------------------
/** @file RichZeroSuppALICEData_V1.h
 *
 *  Header file for RICH DAQ utility class : RichZeroSuppALICEData
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2004-12-17
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <algorithm>

// local
#include "RichDAQKernel/RichPDDataBank.h"
#include "RichDAQKernel/RichZSPacked_V2.h"

// Kernel
#include "Kernel/MemPoolAlloc.h"

// Event Model
#include "Event/ODIN.h"

namespace Rich::DAQ
{

  /** @namespace Rich::DAQ::RichZeroSuppALICEDataV1
   *
   *  Namespace for version 4 of the RichZeroSuppALICEData object.
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   2004-12-17
   */
  namespace RichZeroSuppALICEDataV1
  {

    /// Import HPD sepcific parameters
    using namespace Rich::DAQ::HPD;

    /** @class RichZeroSuppALICEData RichZeroSuppALICEData.h
     *
     *  The RICH HPD zero suppressed data format.
     *  Third iteration of the format using a new packing scheme.
     *
     *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
     *  @date   2003-11-07
     */
    template < class Version, class Header, class Footer >
    class RichZeroSuppALICEData
      : public PDDataBankImp< Version, Header, Footer >,
        public LHCb::MemPoolAlloc<
          RichZeroSuppALICEDataV1::RichZeroSuppALICEData< Version, Header, Footer > >
    {

    public:
      /// Default constructor
      RichZeroSuppALICEData() : PDDataBankImp< Version, Header, Footer >( MaxDataSize ) {}

      /** Constructor from a RichSmartID HPD identifier and a vector of RichSmartIDs
       *
       *  @param l0ID   Level0 board hardware identifier
       *  @param digits Vector of RichSmartIDs listing the active channels in this HPD
       */
      explicit RichZeroSuppALICEData( const Level0ID                   l0ID,
                                      const LHCb::RichSmartID::Vector &digits,
                                      const bool                       extendedFormat = false,
                                      const LHCb::ODIN *               odin           = nullptr )
        : PDDataBankImp< Version, Header, Footer >(
            Header( true,                                      // Is ZS
                    true,                                      // ALICE mode
                    extendedFormat,                            // data format
                    false,                                     // No GT inhibit
                    l0ID,                                      // L0 ID
                    EventID( odin ? odin->eventNumber() : 0 ), // Event ID
                    0 // Filled by buildData call below in main body
                    ),
            Footer(),
            0,
            MaxDataSize )
      {
        buildData( digits );
      }

      /** Constructor from a block of raw data
       *
       *  @param data Pointer to the start of the data block
       */
      explicit RichZeroSuppALICEData( const LongType *data )
        : PDDataBankImp< Version, Header, Footer >( data,       // start of data
                                                    MaxDataSize // max data block size
          )
      {}

      /** Reset for a new block of raw data
       *  @param data     Pointer to the start of the data block
       */
      void reset( const LongType *data, const ShortType dataSize = 0 ) override
      {
        m_tooBig = false;
        PDDataBankImp< Version, Header, Footer >::reset( data, dataSize );
      }

      /// Destructor
      virtual ~RichZeroSuppALICEData() = default;

      // Fill a vector with RichSmartIDs for hit pixels
      ShortType fillRichSmartIDs( LHCb::RichSmartID::Vector &ids,
                                  const LHCb::RichSmartID    hpdID ) const override final;

      // Test if this bank would be too big ( i.e. greater than 32 words )
      inline bool tooBig() const noexcept { return m_tooBig; }

    private: // methods
      /// Build data array from vector of RichSmartIDs
      void buildData( const LHCb::RichSmartID::Vector &pdHits );

      /// update the address offset
      inline void updateOffset( const int  address,
                                const bool firstWord,
                                int &      last_address,
                                int &      last_offset ) const
      {
        if ( !firstWord )
        {
          if ( 0 == address ) { ++last_offset; }
          else if ( 1 == address )
          {
            if ( 0 == last_address % 2 ) { last_offset = 0; }
            else
            {
              ++last_offset;
            }
          }
        }
        last_address = address;
      }

    private: // data
      /// Too big flag
      bool m_tooBig = false;
    };

  } // namespace RichZeroSuppALICEDataV1

} // namespace Rich::DAQ
