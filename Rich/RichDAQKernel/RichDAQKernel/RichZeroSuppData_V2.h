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
/** @file RichZeroSuppData_V2.h
 *
 *  Header file for RICH DAQ utility class : RichZeroSuppData
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2004-12-17
 */
//-----------------------------------------------------------------------------

#pragma once

// local
#include "RichDAQKernel/RichPDDataBank.h"
#include "RichDAQKernel/RichZSPacked_V1.h"

// Kernel
#include "Kernel/MemPoolAlloc.h"

namespace Rich::DAQ {

  /** @namespace Rich::DAQ::RichZeroSuppDataV2
   *
   *  Namespace for version 2 of the RichZeroSuppData object.
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   2004-12-17
   */
  namespace RichZeroSuppDataV2 {

    /// Import HPD sepcific parameters
    using namespace Rich::DAQ::HPD;

    /** @class RichZeroSuppData RichZeroSuppData.h
     *
     *  The RICH HPD zero suppressed data format.
     *  Third iteration of the format using a new packing scheme.
     *
     *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
     *  @date   2003-11-07
     */
    template <class Version, class Header, class Footer>
    class RichZeroSuppData : public PDDataBankImp<Version, Header, Footer>,
                             public LHCb::MemPoolAlloc<RichZeroSuppDataV2::RichZeroSuppData<Version, Header, Footer>> {

    public:
      /// Default constructor
      RichZeroSuppData() : PDDataBankImp<Version, Header, Footer>( MaxDataSize ) {}

      /** Constructor from a RichSmartID HPD identifier and a vector of RichSmartIDs
       *
       *  @param l0ID   Level0 board Rich::DAQ::hardware identifier
       *  @param digits Vector of RichSmartIDs listing the active channels in this HPD
       */
      explicit RichZeroSuppData( const Level0ID l0ID, const LHCb::RichSmartID::Vector& digits )
          : PDDataBankImp<Version, Header, Footer>( Header( true, l0ID, digits.size() ), Footer(), 0, MaxDataSize ) {
        buildData( digits );
      }

      /** Constructor from a block of raw data
       *
       *  @param data     Pointer to the start of the data block
       *  @param dataSize The size of the data block (excluding header HPD word)
       */
      explicit RichZeroSuppData( const LongType* data, const ShortType dataSize )
          : PDDataBankImp<Version, Header, Footer>( data,        // start of data
                                                    MaxDataSize, // max data block size
                                                    dataSize ) {}

      /** Reset for a new block of raw data
       *  @param data     Pointer to the start of the data block
       *  @param dataSize The size of the data block (excluding header HPD word)
       */
      void reset( const LongType* data, const ShortType dataSize ) override {
        m_tooBig = false;
        PDDataBankImp<Version, Header, Footer>::reset( data, dataSize );
      }

      /// Destructor
      virtual ~RichZeroSuppData() = default;

      // Fill a vector with RichSmartIDs for hit pixels
      ShortType fillRichSmartIDs( LHCb::RichSmartID::Vector& ids, const LHCb::RichSmartID hpdID ) const override final;

      // Test if this bank would be too big ( i.e. greater than 32 words )
      inline bool tooBig() const noexcept { return m_tooBig; }

    private: // methods
      /// Build data array from vector of RichSmartIDs
      void buildData( const LHCb::RichSmartID::Vector& pdHits );

    private: // data
      /// Too big flag
      bool m_tooBig = false;
    };

  } // namespace RichZeroSuppDataV2

} // namespace Rich::DAQ
