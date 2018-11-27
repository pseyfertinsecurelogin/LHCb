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
#ifndef OTDAQ_OTChannelHit_H
#define OTDAQ_OTChannelHit_H 1

#include <vector>
#include "GaudiKernel/Range.h"

namespace OTDAQ
{

  // Forward declarations

  /** @class RawHit RawHit.h
   *
   * The Data Word is a word of 32 bits that containes the data of 2 hits of the
 * OT for the OTDAQ data format
   *
   * @author Jacopo Nardulli, Wouter Hulsbergen
   * created Wed Nov 14 17:03:07 2007
   *
   */

  class RawHit final
  {
  public:

    /// Default Constructor
    RawHit() = default;

    /// constructor with data word in raw bank buffer
    RawHit(unsigned short data) : m_data(data) {}

    /// constructor with word, channel in OTIS (0 -127), tdctime
    RawHit( unsigned int word,
            unsigned int channel,
            unsigned int tdcTime )
    : RawHit( ( word << wordBits ) + ( channel << ChannelBits ) + ( tdcTime << TimeBits ) )
    {}

    /// Retrieve tdc time
    unsigned int time() const;

    /// Retrieve channel number (0-127)
    unsigned int channel() const;

    /// Retrieve channel number in otis (0-31)
    unsigned int channelInOtis() const;

    /// Retrieve otis number (0-3)
    unsigned int otis() const;

    /// Retrieve another bit that I do not know the meaning of
    bool word() const;

    /// Retrieve the contents
    unsigned short data() const { return m_data ; }

  public:

    /// Offsets of bitfield dataWord
    enum dataWordBits{TimeBits     = 0,
                      ChannelBits  = 8,
                      OtisBits     = 13,
                      wordBits     = 15
    };

    /// Bitmasks for bitfield dataWord
    enum dataWordMasks{TimeMask     = 0xFFL,
                       ChannelInOtisMask  = 0x1F00L,
                       OtisMask     = 0x6000L,
		       ChannelMask  = ChannelInOtisMask | OtisMask,
                       wordMask     = 0x8000L
    };

  private:
    unsigned short m_data = 0; ///< Data Word ID

  }; // class RawHit

  /// Type for a container of RawHit  objects
  typedef std::vector<RawHit> RawHitContainer ;

  /// Range object for RawHitContainer
  typedef Gaudi::Range_<RawHitContainer> RawHitRange ;

  // -----------------------------------------------------------------------------
  // end of class
  // -----------------------------------------------------------------------------

  inline unsigned int RawHit::time() const
  {
    return (m_data & TimeMask) >> TimeBits;
  }

  inline unsigned int RawHit::channel() const
  {
    return (m_data & ChannelMask) >> ChannelBits;
  }

  inline unsigned int RawHit::channelInOtis() const
  {
    return (m_data & ChannelInOtisMask) >> ChannelBits;
  }

  inline unsigned int RawHit::otis() const
  {
    return (m_data & OtisMask) >> OtisBits;
  }

  inline bool RawHit::word() const
  {
    return 0 != ((m_data & wordMask) >> wordBits);
  }
}


#endif ///OTDAQ_RawHit_H
