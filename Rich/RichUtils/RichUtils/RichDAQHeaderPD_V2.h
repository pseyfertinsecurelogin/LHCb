
//=============================================================================================
/** @file RichDAQHeaderPD_V2.h
 *
 *  Header file for RICH DAQ class : RichDAQHeaderV2::RichDAQHeaderPD
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

  /** @namespace Rich::DAQ::RichDAQHeaderV2
   *
   *  Namespace for version 2 of the RichDAQHeaderPD object.
   *
   *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
   *  @date   2004-12-17
   */
  namespace RichDAQHeaderV2
  {

    /// Import HPD specific parameters
    using namespace Rich::DAQ::HPD;

    /** @namespace Rich::DAQ::RichDAQHeaderV2::RichDAQHeaderPDCode
     *
     *  Namespace for definitions related to RichDAQHeaderV2::RichDAQHeaderPD
     *
     *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
     *  @date   2003-11-06
     */
    namespace RichDAQHeaderPDCode
    {

      /// Number of words in this header
      static const ShortType nHeaderWords = 1;

      // Define the number of bits for each field
      static const ShortType BitsNeightBitP1 = 10; ///< Bits for # 8-bit blocks + 1
      static const ShortType BitsL0ID        = 12; ///< Bits for L0 identifier
      static const ShortType BitsUnUsed1     = 7;  ///< Unused bits
      static const ShortType BitsAlice       = 1;  ///< Bits for the ALICE/LHCb mode flag
      static const ShortType BitsZS          = 1;  ///< Bits for the zero suppression flag

      // Create the shift registers
      static const ShortType ShiftNeightBitP1 = 0;
      static const ShortType ShiftL0ID        = ShiftNeightBitP1 + BitsNeightBitP1;
      static const ShortType ShiftAlice       = ShiftL0ID + BitsL0ID + BitsUnUsed1;
      static const ShortType ShiftZS          = ShiftAlice + BitsAlice;

      // Create the Masks
      static const LongType MaskNeightBitP1 = ( ( 1 << BitsNeightBitP1 ) - 1 ) << ShiftNeightBitP1;
      static const LongType MaskL0ID        = ( ( 1 << BitsL0ID ) - 1 ) << ShiftL0ID;
      static const LongType MaskAlice       = ( ( 1 << BitsAlice ) - 1 ) << ShiftAlice;
      static const LongType MaskZS          = ( ( 1 << BitsZS ) - 1 ) << ShiftZS;

      // Create the max values that can be stored in each field
      static const ShortType MaxNeightBitP1 = ( 1 << BitsNeightBitP1 ) - 1; ///< Max number of hits
      static const ShortType MaxL0ID        = ( 1 << BitsL0ID ) - 1;        ///< Max L0 ID

    } // namespace RichDAQHeaderPDCode

    /** @class RichDAQHeaderPD RichDAQHeaderPD_V2.h
     *
     *  Utility class representing the header word for a Level0/HPD data block.
     *
     *  Version 2 : Final single word version. Under development
     *
     *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
     *  @date   2003-11-06
     */
    class RichDAQHeaderPD final : public HeaderPDBase, public LHCb::MemPoolAlloc< RichDAQHeaderPD >
    {

    public: // methods
      /// Default Constructor
      RichDAQHeaderPD() = default;

      /// Constructor from a pointer to a data stream
      explicit RichDAQHeaderPD( const LongType *data )
        : HeaderPDBase( WordType( *( data++ ) ) ) // header has one word only
      {}

      /// Constructor from all data
      RichDAQHeaderPD( const bool      zSupp, ///< Flag indicating if the block is zero suppressed
                       const bool      aMode, ///< Flag indicating if data is ALICE mode
                       const Level0ID  l0ID,  ///< The Level 0 hardware identifier
                       const ShortType dSize  ///< The data size word
      )
      {
        if ( !setZeroSuppressed( zSupp ) || !setAliceMode( aMode ) || !setL0ID( l0ID ) ||
             !setNEightBitBlocksPlusOne( dSize ) )
        {
          throw GaudiException( "Data out of range", "*RichDAQHeaderPDV2*", StatusCode::FAILURE );
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
      inline void reset() { setPrimaryHeaderWord( WordType( 0 ) ); }

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
        return Level0ID( ( primaryHeaderWord().data() & RichDAQHeaderPDCode::MaskL0ID ) >>
                         RichDAQHeaderPDCode::ShiftL0ID );
      }

      /// Set the Level0 ID
      inline bool setL0ID( const Level0ID l0id )
      {
        return (
          dataInRange( l0id.data(), RichDAQHeaderPDCode::MaxL0ID ) ?
            set( l0id.data(), RichDAQHeaderPDCode::ShiftL0ID, RichDAQHeaderPDCode::MaskL0ID ) :
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
                        RichDAQHeaderPDCode::MaskNeightBitP1 ) :
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
        return set( i, RichDAQHeaderPDCode::ShiftZS, RichDAQHeaderPDCode::MaskZS );
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
        return set( i, RichDAQHeaderPDCode::ShiftAlice, RichDAQHeaderPDCode::MaskAlice );
      }

      /** Returns the number of words in the data block associated to this header
       *  taking into account the bank type (zero-supressed or not)
       */
      inline unsigned int nDataWords() const
      {
        return ( zeroSuppressed() ? nEightBitBlocksPlusOne() / 2 :
                                    ( aliceMode() ? MaxDataSizeALICE : MaxDataSize ) );
      }

    public: // Static methods to test specific flags in external data blocks
      /// Test if this data block is for an ALICE mode HPD
      inline static bool aliceMode( const LongType *word )
      {
        return ( 0 != ( ( word[ 0 ] & RichDAQHeaderPDCode::MaskAlice ) >>
                        RichDAQHeaderPDCode::ShiftAlice ) );
      }

      /// Test if this data block is for a zero suppressed HPD
      inline static bool zeroSuppressed( const LongType *word )
      {
        return ( 0 !=
                 ( ( word[ 0 ] & RichDAQHeaderPDCode::MaskZS ) >> RichDAQHeaderPDCode::ShiftZS ) );
      }

    public: // methods not properly implemented, but included for compatibility
      /// Returns if this header (and the associated footer) are in extended mode or not (compact)
      inline bool extendedFormat() const { return false; }

      /// Retrieve the event ID word
      inline EventID eventID() const { return EventID( 0, 0 ); }

      /// Is the data from this HPD suppressed by the L1
      inline bool l1Suppressed() const { return true; }
    };

  } // namespace RichDAQHeaderV2

  /// overloaded output to std::ostream
  inline std::ostream &operator<<( std::ostream &                          os,
                                   const RichDAQHeaderV2::RichDAQHeaderPD &header )
  {
    os << "HPD header V2 : nHeadW = " << header.nHeaderWords() << " L0ID = " << header.l0ID()
       << " ZS = " << header.zeroSuppressed() << " AliceMode = " << header.aliceMode()
       << " # 8-bit blocks + 1 = " << header.nEightBitBlocksPlusOne();
    return os;
  }

} // namespace Rich::DAQ
