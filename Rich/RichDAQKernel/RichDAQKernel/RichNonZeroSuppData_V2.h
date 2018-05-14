
//-----------------------------------------------------------------------------
/** @file RichNonZeroSuppData_V2.h
 *
 *  Header file for RICH DAQ utility class : RichNonZeroSuppData
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2003-11-07
 */
//-----------------------------------------------------------------------------

#pragma once

// local
#include "RichDAQKernel/RichPDDataBank.h"
#include "RichDAQKernel/RichZSPacked_V2.h"

// Kernel
#include "Kernel/MemPoolAlloc.h"

//===================================================================================

namespace Rich
{
  namespace DAQ
  {

    /** @namespace Rich::DAQ::RichNonZeroSuppDataV2
     *
     *  Namespace for version 2 of the RichNonZeroSuppData object.
     *
     *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
     *  @date   2004-12-17
     */
    namespace RichNonZeroSuppDataV2
    {

      /// Import HPD specific parameters
      using namespace Rich::DAQ::HPD;

      /** @class RichNonZeroSuppData RichNonZeroSuppData_V2.h
       *
       *  The RICH HPD non zero suppressed data format.
       *  Second iteration of the format. Identical to version 1
       *  apart from reversing the order the rows are encoded/decoded.
       *
       *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
       *  @date   2003-11-07
       *
       *  @todo Update 8 bit words + 1 value
       */
      template< class Version, class Header, class Footer >
      class RichNonZeroSuppData : public PDDataBankImp<Version,Header,Footer>,
                                  public LHCb::MemPoolAlloc<RichNonZeroSuppDataV2::RichNonZeroSuppData<Version,Header,Footer> >
      {

      public:

        /// Default constructor
        RichNonZeroSuppData()
          : PDDataBankImp<Version,Header,Footer>( MaxDataSize )
        { }

        /** Constructor from a RichSmartID HPD identifier and a vector of RichSmartIDs
         *
         *  @param l0ID   L0 board hardware identifier
         *  @param digits Vector of RichSmartIDs listing the active channels in this HPD
         */
        explicit RichNonZeroSuppData( const Level0ID l0ID,
                                      const LHCb::RichSmartID::Vector & digits )
          : PDDataBankImp<Version,Header,Footer> ( Header( false, // Not ZS
                                                           false, // Not ALICE mode
                                                           l0ID,
                                                           0 // filled by buildData call
                                                     ),
                                                   Footer(),
                                                   0, MaxDataSize, MaxDataSize )
        {
          buildData( digits );
        }

        /** Constructor from a block of raw data
         *
         *  @param data Pointer to the start of the data block
         */
        explicit RichNonZeroSuppData( const LongType * data )
          : PDDataBankImp<Version,Header,Footer> ( data, // start of data
                                                   MaxDataSize // max data bloxk size
                                                 )
        { }

        /// Destructor
        ~RichNonZeroSuppData() = default;

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
          this -> setBit( this->data()[this->maxDataSize()-(row+1)], col );
        }

        /// Is a given pixel active ?
        inline bool isPixelActive( const ShortType row,
                                   const ShortType col ) const noexcept
        {
          return this -> isBitOn( this->data()[this->maxDataSize()-(row+1)], col );
        }

      };

    } // RichNonZeroSuppDataV2 namespace

  }
}
