
//-----------------------------------------------------------------------------
/** @file RichZeroSuppData_V1.h
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
#include "RichDAQKernel/RichZSHitTriplet.h"

// Kernel
#include "Kernel/MemPoolAlloc.h"

// =================================================================================

namespace Rich::DAQ
{

  /** @namespace Rich::DAQ::RichZeroSuppDataV1
   *
   *  Namespace for version 1 of the RichZeroSuppData object.
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   2004-12-17
   */
  namespace RichZeroSuppDataV1
  {

    /// Import HPD sepcific parameters
    using namespace Rich::DAQ::HPD;

    /** @class RichZeroSuppData RichZeroSuppData_V1.h
     *
     *  The RICH HPD zero suppressed data format.
     *  Second iteration of the format. Identical to version 0
     *  apart from a new header word format.
     *
     *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
     *  @date   2003-11-07
     */
    template < class Version, class Header, class Footer >
    class RichZeroSuppData
      : public PDDataBankImp< Version, Header, Footer >,
        public LHCb::MemPoolAlloc< RichZeroSuppDataV1::RichZeroSuppData< Version, Header, Footer > >
    {

    public:

      /// Default constructor
      RichZeroSuppData() : PDDataBankImp< Version, Header, Footer >( MaxDataSize ) {}

      /** Constructor from a RichSmartID HPD identifier and a vector of RichSmartIDs
       *
       *  @param l0ID   Level0 board Hardware identifier
       *  @param digits Vector of RichSmartIDs listing the active channels in this HPD
       */
      explicit RichZeroSuppData( const Level0ID l0ID, const LHCb::RichSmartID::Vector &digits )
        : PDDataBankImp< Version, Header, Footer >( Header( true, l0ID, digits.size() ),
                                                    Footer(),
                                                    0,
                                                    MaxDataSize )
      {
        buildData( digits );
      }

      /** Constructor from a block of raw data
       *
       *  @param data     Pointer to the start of the data block
       *  @param dataSize The size of the data block (excluding header HPD word)
       */
      explicit RichZeroSuppData( const LongType *data, const ShortType dataSize )
        : PDDataBankImp< Version, Header, Footer >( data,        // start of data
                                                    MaxDataSize, // max data block size
                                                    dataSize )
      {}

      /// Destructor
      virtual ~RichZeroSuppData() = default;

      // Fill a vector with RichSmartIDs for hit pixels
      ShortType fillRichSmartIDs( LHCb::RichSmartID::Vector &ids,
                                  const LHCb::RichSmartID    hpdID ) const override final;

    private: // methods

      /// Build data array from vector of RichSmartIDs
      void buildData( const LHCb::RichSmartID::Vector &pdHits );
    };

  } // namespace RichZeroSuppDataV1

} // namespace Rich::DAQ

// =================================================================================
