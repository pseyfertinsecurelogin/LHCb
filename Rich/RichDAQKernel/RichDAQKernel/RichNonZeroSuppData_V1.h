
//-----------------------------------------------------------------------------
/** @file RichNonZeroSuppData_V1.h
 *
 *  Header file for RICH DAQ utility class : RichNonZeroSuppData
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2003-11-07
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <algorithm>

// local
#include "RichDAQKernel/RichPDDataBank.h"

// Kernel
#include "Kernel/MemPoolAlloc.h"

//===================================================================================

namespace Rich::DAQ
{

  /** @namespace Rich::DAQ::RichNonZeroSuppDataV1
   *
   *  Namespace for version 1 of the RichNonZeroSuppData object.
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   2004-12-17
   */
  namespace RichNonZeroSuppDataV1
  {

    /// Import HPD specific parameters
    using namespace Rich::DAQ::HPD;

    /** @class RichNonZeroSuppData RichNonZeroSuppData_V1.h
     *
     *  The RICH HPD non zero suppressed data format.Z
     *  Second iteration of the format. Identical to version 0
     *  apart from a new header word format.
     *
     *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
     *  @date   2003-11-07
     */
    template < class Version, class Header, class Footer >
    class RichNonZeroSuppData
      : public PDDataBankImp< Version, Header, Footer >,
        public LHCb::MemPoolAlloc<
          RichNonZeroSuppDataV1::RichNonZeroSuppData< Version, Header, Footer > >
    {

    public:

      /// Default constructor
      RichNonZeroSuppData() : PDDataBankImp< Version, Header, Footer >( MaxDataSize ) {}

      /** Constructor from a RichSmartID HPD identifier and a vector of RichSmartIDs
       *
       *  @param l0ID   L0 board hardware identifier
       *  @param digits Vector of RichSmartIDs listing the active channels in this HPD
       */
      explicit RichNonZeroSuppData( const Level0ID l0ID, const LHCb::RichSmartID::Vector &digits )
        : PDDataBankImp< Version, Header, Footer >( Header( false, l0ID, digits.size() ),
                                                    Footer(),
                                                    0,
                                                    MaxDataSize,
                                                    MaxDataSize )
      {
        for ( const auto &dig : digits ) { setPixelActive( dig.pixelRow(), dig.pixelCol() ); }
      }

      /** Constructor from a block of raw data
       *
       *  @param data Pointer to the start of the data block
       */
      explicit RichNonZeroSuppData( const LongType *data )
        : PDDataBankImp< Version, Header, Footer >( data,        // start of data
                                                    MaxDataSize, // max data block size
                                                    MaxDataSize )
      {}

      /** Reset for a new block of raw data
       *  @param data     Pointer to the start of the data block
       */
      void reset( const LongType *data, const ShortType dataSize = 0 ) override
      {

        PDDataBankImp< Version, Header, Footer >::reset( data, std::max( MaxDataSize, dataSize ) );
      }

      /// Destructor
      ~RichNonZeroSuppData() = default;

      // Fill a vector with RichSmartIDs for hit pixels
      ShortType fillRichSmartIDs( LHCb::RichSmartID::Vector &ids,
                                  const LHCb::RichSmartID    hpdID ) const override final;

    private: // methods

      /// Set a pixel as active
      inline void setPixelActive( const ShortType row, const ShortType col ) noexcept
      {
        PDDataBankImp< Version, Header, Footer >::setBit( this->data()[row], col );
      }

      /// Is a given pixel active ?
      inline bool isPixelActive( const ShortType row, const ShortType col ) const noexcept
      {
        return this->isBitOn( this->data()[row], col );
      }
    };

  } // namespace RichNonZeroSuppDataV1

} // namespace Rich::DAQ
