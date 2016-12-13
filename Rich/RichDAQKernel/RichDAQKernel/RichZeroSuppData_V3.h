
//-----------------------------------------------------------------------------
/** @file RichZeroSuppData_V3.h
 *
 *  Header file for RICH DAQ utility class : RichZeroSuppData
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2004-12-17
 */
//-----------------------------------------------------------------------------

#ifndef RICHDAQ_RICHZEROSUPPDATA_V3_H
#define RICHDAQ_RICHZEROSUPPDATA_V3_H 1

// local
#include "RichDAQKernel/RichHPDDataBank.h"
#include "RichDAQKernel/RichZSPacked_V2.h"

// Kernel
#include "Kernel/MemPoolAlloc.h"

namespace Rich
{
  namespace DAQ
  {

    /** @namespace Rich::DAQ::RichZeroSuppDataV3
     *
     *  Namespace for version 3 of the RichZeroSuppData object.
     *
     *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
     *  @date   2004-12-17
     */
    namespace RichZeroSuppDataV3
    {

      /** @class RichZeroSuppData RichZeroSuppData.h
       *
       *  The RICH HPD zero suppressed data format.
       *  Third iteration of the format using a new packing scheme.
       *
       *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
       *  @date   2003-11-07
       */
      template< class Version, class Header, class Footer >
      class RichZeroSuppData : public HPDDataBankImp<Version,Header,Footer>,
                               public LHCb::MemPoolAlloc<RichZeroSuppDataV3::RichZeroSuppData<Version,Header,Footer> >
      {

      public:

        /// Default constructor
        RichZeroSuppData()
          : HPDDataBankImp<Version,Header,Footer>( MaxDataSize )
        { }

        /** Constructor from a RichSmartID HPD identifier and a vector of RichSmartIDs
         *
         *  @param l0ID   Level0 board hardware identifier
         *  @param digits Vector of RichSmartIDs listing the active channels in this HPD
         */
        explicit RichZeroSuppData( const Level0ID l0ID,
                                   const LHCb::RichSmartID::Vector & digits )
          : HPDDataBankImp<Version,Header,Footer> ( Header ( true,  // Is ZS
                                                             false, // Not ALICE mode
                                                             l0ID,  // L0 ID
                                                             0      // Filled by buildData call below in main body
                                                             ),
                                                    Footer ( ),
                                                    0, MaxDataSize )
        {
          buildData( digits );
        }

        /** Constructor from a block of raw data
         *
         *  @param data     Pointer to the start of the data block
         */
        explicit RichZeroSuppData( const LongType * data )
          : HPDDataBankImp<Version,Header,Footer> ( data,          // start of data
                                                    MaxDataSize  // max data block size
                                                    )
        { }

        /** Reset for a new block of raw data
         *  @param data     Pointer to the start of the data block
         */
        inline void reset( const LongType * data )
        {
          m_tooBig = false;
          HPDDataBankImp<Version,Header,Footer>::reset( data );
        }

        /// Destructor
        virtual ~RichZeroSuppData() = default;

        // Fill a vector with RichSmartIDs for hit pixels
        ShortType fillRichSmartIDs( LHCb::RichSmartID::Vector & ids,
                                    const LHCb::RichSmartID hpdID ) const override final;

        // Test if this bank would be too big ( i.e. greater than 32 words )
        inline bool tooBig() const noexcept { return m_tooBig; }

      private: // methods

        /// Build data array from vector of RichSmartIDs
        void buildData( const LHCb::RichSmartID::Vector & pdHits );

      private: // data

        /// Too big flag
        bool m_tooBig = false;

      };

    } // end V3 namespace

  }
}

#endif // RICHDAQ_RICHZEROSUPPDATA_V3_H
