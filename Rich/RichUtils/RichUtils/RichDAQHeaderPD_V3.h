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

//=============================================================================================
/** @file RichDAQHeaderPD_V3.h
 *
 *  Header file for RICH DAQ class : RichDAQHeaderV3::RichDAQHeaderPD
 *
 *  @author Chris Jones       Christopher.Rob.Jones@cern.ch
 *  @date   2003-11-06
 */
//=============================================================================================

#pragma once

// Base class
#include "RichUtils/RichDAQHeaderPDBase.h"

// Gaudi
#include "GaudiKernel/GaudiException.h"

// numbering
#include "RichUtils/RichDAQDefinitions.h"

namespace Rich::DAQ
{

  /** @namespace Rich::DAQ::RichDAQHeaderV3
   *
   *  Namespace for version 3 of the RichDAQHeaderPD object.
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   2004-12-17
   */
  namespace RichDAQHeaderV3
  {

    /// Import HPD specific parameters
    using namespace Rich::DAQ::HPD;

    /** @namespace Rich::DAQ::RichDAQHeaderV3::RichDAQHeaderPDCode
     *
     *  Namespace for definitions related to RichDAQHeaderV3::RichDAQHeaderPD
     *
     *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
     *  @date   2003-11-06
     */
    namespace RichDAQHeaderPDCode
    {

      /// Number of words in this header
      static const ShortType nHeaderWords = 3;

      // first header word
      //-----------------------------------------------------------------------------------------

      // Define the number of bits for each field
      static const ShortType BitsNeightBitP1 = 11; ///< Bits for # 8-bit blocks + 1
      static const ShortType BitsZS          = 1;  ///< Bits for the zero suppression flag
      static const ShortType BitsAlice       = 1;  ///< Bits for the ALICE/LHCb mode flag
      static const ShortType BitsMemID       = 3;  ///< Bits for the memory ID
      static const ShortType BitsEventID     = 15; ///< Bits for Event ID

      // Create the shift registers
      static const ShortType ShiftNeightBitP1 = 0;
      static const ShortType ShiftZS          = ShiftNeightBitP1 + BitsNeightBitP1;
      static const ShortType ShiftAlice       = ShiftZS + BitsZS;
      static const ShortType ShiftMemID       = ShiftAlice + BitsAlice;
      static const ShortType ShiftEventID     = ShiftMemID + BitsMemID;

      // Create the Masks
      static const LongType MaskNeightBitP1 = ( LongType )( ( 1 << BitsNeightBitP1 ) - 1 )
                                              << ShiftNeightBitP1;
      static const LongType MaskZS      = ( LongType )( ( 1 << BitsZS ) - 1 ) << ShiftZS;
      static const LongType MaskAlice   = ( LongType )( ( 1 << BitsAlice ) - 1 ) << ShiftAlice;
      static const LongType MaskMemID   = ( LongType )( ( 1 << BitsMemID ) - 1 ) << ShiftMemID;
      static const LongType MaskEventID = ( LongType )( ( 1 << BitsEventID ) - 1 ) << ShiftEventID;

      //-----------------------------------------------------------------------------------------

      // second header word
      //-----------------------------------------------------------------------------------------
      // This word is ignored for the moment
      //-----------------------------------------------------------------------------------------

      // third header word
      //-----------------------------------------------------------------------------------------

      // Define the number of bits for each field
      static const ShortType BitsL0ID = 11; ///< Bits for L0 identifier
      // rest is ignored

      // Create the shift registers
      static const ShortType ShiftL0ID = 0;

      // Create the Masks
      static const LongType MaskL0ID = ( ( 1 << BitsL0ID ) - 1 ) << ShiftL0ID;

      //-----------------------------------------------------------------------------------------

      // Create the max values that can be stored in each field
      static const ShortType MaxNeightBitP1 = ( 1 << BitsNeightBitP1 ) - 1; ///< Max number of hits
      static const ShortType MaxL0ID        = ( 1 << BitsL0ID ) - 1;        ///< Max L0 ID

    } // namespace RichDAQHeaderPDCode

    /** @class RichDAQHeaderPD RichDAQHeaderPD_V3.h
     *
     *  Utility class representing the header word for a Level0/HPD data block.
     *
     *  Version 3 : Multi word version used in September 2006 testbeam
     *
     *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
     *  @date   2003-11-06
     */
    class RichDAQHeaderPD final : public HeaderPDBase, public LHCb::MemPoolAlloc< RichDAQHeaderPD >
    {

    public: // methods

      /// Default Constructor
      explicit RichDAQHeaderPD() : HeaderPDBase( RichDAQHeaderPDCode::nHeaderWords, WordType( 0 ) )
      {}

      /// Constructor from a pointer to a data stream
      explicit RichDAQHeaderPD( const LongType *data )
        : HeaderPDBase( RichDAQHeaderPDCode::nHeaderWords,
                        WordType( 0 ) ) // header has three words
      {
        setPrimaryHeaderWord( WordType( *( data++ ) ) );
        extendedHeaderWords()[0] = WordType( *( data++ ) );
        extendedHeaderWords()[1] = WordType( *( data++ ) );
      }

      /// Constructor from all data
      RichDAQHeaderPD( const bool      zSupp, ///< Flag indicating if the block is zero suppressed
                       const bool      aMode, ///< Flag indicating if data is ALICE mode
                       const Level0ID  l0ID,  ///< The Level 0 hardware identifier
                       const ShortType dSize  ///< The data size word
                       )
        : HeaderPDBase( RichDAQHeaderPDCode::nHeaderWords,
                        WordType( 0 ) ) // header has three words
      {
        if ( !setZeroSuppressed( zSupp ) || !setAliceMode( aMode ) || !setL0ID( l0ID ) ||
             !setNEightBitBlocksPlusOne( dSize ) )
        {
          throw GaudiException( "Data out of range", "*RichDAQHeaderPDV3*", StatusCode::FAILURE );
        }
      }

    public:

      /// Read correct number of data words from given stream
      /// Note, after this call data pointer is incremented to the next word after the header
      inline void readFromDataStream( const LongType *&data )
      {
        for ( auto i = 0u; i < nHeaderWords(); ++i ) { setHeaderWord( i, *( data++ ) ); }
      }

    public:

      /// reset for new data stream
      inline void reset()
      {
        setPrimaryHeaderWord( WordType( 0 ) );
        extendedHeaderWords()[0] = WordType( 0 );
        extendedHeaderWords()[1] = WordType( 0 );
      }

      /// reset for a new data stream
      inline void reset( const LongType *data )
      {
        reset();
        readFromDataStream( data );
      }

    public:

      /// Retrieve the Level0 ID
      inline Level0ID l0ID() const
      {
        return Level0ID( ( extendedHeaderWords()[1].data() & RichDAQHeaderPDCode::MaskL0ID ) >>
                         RichDAQHeaderPDCode::ShiftL0ID );
      }

      /// Set the Level0 ID
      inline bool setL0ID( const Level0ID l0id )
      {
        return (
          dataInRange( l0id.data(), RichDAQHeaderPDCode::MaxL0ID ) ?
            set( l0id.data(), RichDAQHeaderPDCode::ShiftL0ID, RichDAQHeaderPDCode::MaskL0ID, 2 ) :
            false );
      }

      /// Retrieve the number of "8-bit data blocks plus one" with at least one hit
      inline ShortType nEightBitBlocksPlusOne() const
      {
        return ( ( primaryHeaderWord().data() & RichDAQHeaderPDCode::MaskNeightBitP1 ) >>
                 RichDAQHeaderPDCode::ShiftNeightBitP1 );
      }

      /// Set the number of "8-bit data blocks plus one" with at least one hit
      inline bool setNEightBitBlocksPlusOne( const ShortType value )
      {
        return ( dataInRange( value, RichDAQHeaderPDCode::MaxNeightBitP1 ) ?
                   set( value,
                        RichDAQHeaderPDCode::ShiftNeightBitP1,
                        RichDAQHeaderPDCode::MaskNeightBitP1,
                        0 ) :
                   false );
      }

      /// Retrieve the zero suppressed information
      inline bool zeroSuppressed() const
      {
        return ( 0 != ( ( primaryHeaderWord().data() & RichDAQHeaderPDCode::MaskZS ) >>
                        RichDAQHeaderPDCode::ShiftZS ) );
      }

      /// Set the zero suppression info
      inline bool setZeroSuppressed( const bool zSupp )
      {
        const ShortType i = ( zSupp ? 1 : 0 );
        return set( i, RichDAQHeaderPDCode::ShiftZS, RichDAQHeaderPDCode::MaskZS, 0 );
      }

      /// Retrieve the flag to say if the data is in ALICE mode
      inline bool aliceMode() const
      {
        return ( 0 != ( ( primaryHeaderWord().data() & RichDAQHeaderPDCode::MaskAlice ) >>
                        RichDAQHeaderPDCode::ShiftAlice ) );
      }

      /// Set the flag to say if the data is in ALICE mode
      inline bool setAliceMode( const bool aMode )
      {
        const ShortType i = ( aMode ? 1 : 0 );
        return set( i, RichDAQHeaderPDCode::ShiftAlice, RichDAQHeaderPDCode::MaskAlice, 0 );
      }

      /** Returns the number of words in the data block associated to this header
       *  taking into account the bank type (zero-supressed or not)
       */
      inline unsigned int nDataWords() const
      {
        return ( zeroSuppressed() ? nEightBitBlocksPlusOne() / 2 :
                                    ( aliceMode() ? MaxDataSizeALICE : MaxDataSize ) );
      }

      /// Retrieve the "memID" word
      inline unsigned int memID() const
      {
        return ( ( primaryHeaderWord().data() & RichDAQHeaderPDCode::MaskMemID ) >>
                 RichDAQHeaderPDCode::ShiftMemID );
      }

      /// Retrieve the "memID" word
      inline EventID eventID() const
      {
        return EventID( ( ( primaryHeaderWord().data() & RichDAQHeaderPDCode::MaskEventID ) >>
                          RichDAQHeaderPDCode::ShiftEventID ),
                        RichDAQHeaderPDCode::BitsEventID );
      }

    public: // Static methods to test specific flags in external data blocks

      /// Test if this data block is for an ALICE mode HPD
      inline static bool aliceMode( const LongType *word )
      {
        return ( 0 != ( ( word[0] & RichDAQHeaderPDCode::MaskAlice ) >>
                        RichDAQHeaderPDCode::ShiftAlice ) );
      }

      /// Test if this data block is for a zero suppressed HPD
      inline static bool zeroSuppressed( const LongType *word )
      {
        return ( 0 !=
                 ( ( word[0] & RichDAQHeaderPDCode::MaskZS ) >> RichDAQHeaderPDCode::ShiftZS ) );
      }

    public: // methods not properly implemented, but included for compatibility

      /// Returns if this header (and the associated footer) are in extended mode or not (compact)
      inline bool extendedFormat() const noexcept { return true; }

      /// Is the data from this HPD suppressed by the L1
      inline bool l1Suppressed() const noexcept { return true; }
    };

  } // namespace RichDAQHeaderV3

  /// overloaded output to std::ostream
  inline std::ostream &operator<<( std::ostream &                          os,
                                   const RichDAQHeaderV3::RichDAQHeaderPD &header )
  {
    os << "HPD header V3 : nHeadW = " << header.nHeaderWords() << " L0ID=" << header.l0ID()
       << " MemID=" << header.memID() << " EventID=" << header.eventID()
       << " ZS=" << header.zeroSuppressed() << " AliceMode=" << header.aliceMode()
       << " # 8-bit blocks+1=" << header.nEightBitBlocksPlusOne();
    return os;
  }

} // namespace Rich::DAQ
