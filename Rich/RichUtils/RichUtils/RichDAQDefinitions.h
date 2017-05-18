
//---------------------------------------------------------------------------------
/** @file RichDAQDefinitions.h
 *
 *  Header file for RICH DAQ general definitions
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   2003-11-06
 */
//---------------------------------------------------------------------------------

#pragma once

// from STL
#include <sstream>
#include <vector>
#include <cstdint>
#include <map>

// GaudiKernel
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/HashMap.h"

// Kernel
#include "Kernel/RichSmartID.h"
#include "Kernel/RichSmartIDHashFuncs.h"

namespace Rich
{
  namespace DAQ
  {

    //---------------------------------------------------------------------------------

    /// DAQ 64 bit word type
    using LLongType = std::uint64_t;

    /// DAQ long type definition
    using LongType  = std::uint32_t;

    /// DAQ short type definition
    using ShortType = std::uint32_t;

    /// DAQ index type definition
    using IndexType = std::uint32_t;

    //---------------------------------------------------------------------------------

    /// Maximum data block size (LHCb mode)
    static const ShortType MaxDataSize = 32;

    /// Number of Alice pixels per LHCb pixel
    static const ShortType NumAlicePixelsPerLHCbPixel = 8;

    /// Maximum data block size (ALICE mode)
    static const ShortType MaxDataSizeALICE = MaxDataSize * NumAlicePixelsPerLHCbPixel;

    /// Number of bits per data word
    static const ShortType BitsPerDataWord = 32;

    /// Number of Ingress's per L1 board
    static const ShortType NumIngressPerL1 = 4;

    /** @brief Number of L1 inputs per ingress.
     *  Allow for 12 here even though the UKL1 boards only have 9, to allow the decoding
     *  to work for some prototype boards which do have 12 inputs */
    static const ShortType NumL1InputsPerIngress = 12;

    /// Total number of inputs to an L1 board
    static const ShortType MaxL1Inputs = NumIngressPerL1 * NumL1InputsPerIngress;

    /// Maximum ZS address value
    static const ShortType MaxZSAddress = 255;

    /// Number of pixel columns in the HPD chip
    static const ShortType NumPixelColumns = 32;

    /// Number of pixel rows in the HPD chip
    static const ShortType NumPixelRows    = 32;

    /// Size of HPD (LHCb) pixel in mm
    static const double PixelSize = 0.5;

    //---------------------------------------------------------------------------------

    //---------------------------------------------------------------------------------
    /** @class NumericType RichUtils/RichDAQDefinitions.h
     *
     *  Simple class with numeric characteristics but some additional type safety
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   11/11/2005
     */
    //---------------------------------------------------------------------------------
    template < class TYPE >
    class NumericType
    {
    public:
      /// Default Constructor
      NumericType() = default;
      /// Constructor
      explicit NumericType ( const TYPE id ) noexcept : m_id(id) { }
      /// Retrieve the full value
      inline TYPE data() const noexcept { return m_id; }
      /// Operator ==
      inline bool operator== ( const NumericType<TYPE>& id ) const noexcept
      { return id.data() == this->data() ; }
      /// Operator !=
      inline bool operator!= ( const NumericType<TYPE>& id ) const noexcept
      { return id.data() != this->data() ; }
      /// Operator <
      inline bool operator<  ( const NumericType<TYPE>& id ) const noexcept
      { return this->data() < id.data() ; }
      /// Operator >
      inline bool operator>  ( const NumericType<TYPE>& id ) const noexcept
      { return this->data() > id.data() ; }
    public:
      /// Operator std::string
      inline operator std::string() const
      {
        std::ostringstream s;
        s << data();
        return s.str();
      }
      /// Overload output to ostream
      friend inline std::ostream& operator << ( std::ostream& os1, const NumericType<TYPE> & id )
      { return os1 << id.data() ; }
      /// Operator ++   (prefix)
      inline NumericType<TYPE>& operator++()    { ++m_id; return *this; }
      /// Operator ++(int)  (postfix)
      inline NumericType<TYPE>  operator++(int) { NumericType<TYPE> tmp = *this; ++m_id; return tmp; }
      /// Operator --   (prefix)
      inline NumericType<TYPE>& operator--()    { --m_id; return *this; }
      /// Operator --(int)  (postfix)
      inline NumericType<TYPE>  operator--(int) { NumericType<TYPE> tmp = *this; --m_id; return tmp; }
    public:
      /// Print the word in Hex
      inline void hexDump( std::ostream& os ) const
      {
        std::ostringstream hexW;
        hexW << std::hex << m_id;
        std::string tmpW = hexW.str();
        if ( tmpW.size() < 8 ) { tmpW = std::string(8-tmpW.size(),'0')+tmpW; }
        os << tmpW;
      }
      /// Bits dump
      inline void bitsDump( std::ostream& os,
                            const unsigned int nBits = 8*sizeof(TYPE),
                            const std::string & spacer = " " ) const
      {
        for ( int iCol = nBits-1; iCol >= 0; --iCol )
        {
          os << spacer << isBitOn( iCol );
        }
      }
      /// Raw dump of the word
      inline void rawDump( std::ostream& os ) const
      {
        hexDump(os);
        os << " :";
        bitsDump(os);
      }
    protected:
      /// Update the internal data
      inline void setData( const TYPE id ) noexcept { m_id = id; }
      /// test if a given bit is  'on'
      inline bool isBitOn( const Rich::DAQ::IndexType pos ) const noexcept
      {
        return ( 0 != (m_id & (1<<pos)) );
      }
    private:
      TYPE m_id {0}; ///< The data value
    };

    //---------------------------------------------------------------------------------

    /** @class Level0ID RichUtils/RichDAQDefinitions.h
     *
     *  Contains the Level 0 board number plus one bit identifying one of the two HPDs
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   11/11/2005
     */
    class Level0ID final : public NumericType<ShortType>
    {
    public :
      // Define the number of bits for each field
      static const IndexType  BitsHPD =  1;  ///< Number of bits for HPD ID
      static const IndexType  BitsL0  =  10; ///< Number of bits for L0 ID
      // Create the shift registers
      static const IndexType  ShiftHPD = 0;
      static const IndexType  ShiftL0  = ShiftHPD + BitsHPD;
      // Create the Masks
      static const ShortType  MaskHPD = ((1 << BitsHPD)-1) << ShiftHPD;
      static const ShortType  MaskL0  = ((1 << BitsL0)-1) << ShiftL0;
      // Create the max values that can be stored in each field
      static const ShortType  MaxHPD = ( 1 << BitsHPD ) - 1;
      static const ShortType  MaxL0  = ( 1 << BitsL0 ) - 1;
    public :
      /// Default Constructor
      Level0ID() = default;
      /// Constructor from bit packed word
      explicit Level0ID ( const ShortType id ) noexcept
        : NumericType<ShortType>(id) { }
      /** Constructor from L0 and HPD number
       *  @param l0num  L0 board number
       *  @param hpdnum HPD bit number (0 or 1)
       */
      explicit Level0ID ( const ShortType l0num,
                          const ShortType hpdnum ) noexcept
      {
        setHPD ( hpdnum );
        setL0  ( l0num  );
      }
      /// Return the HPD number (0 or 1)
      inline ShortType hpd() const noexcept
      {
        return ( (data() & MaskHPD) >> ShiftHPD );
      }
      /// Set the HPD number (0 or 1)
      inline bool setHPD( const ShortType hpd ) noexcept
      {
        return ( dataInRange(hpd,MaxHPD) ?
                 set( hpd, ShiftHPD, MaskHPD ) : false );
      }
      /// Return the L0 board number
      inline ShortType l0() const noexcept
      {
        return ( (data() & MaskL0) >> ShiftL0 );
      }
      /// Set the L0 board number
      inline bool setL0( const ShortType l0 ) noexcept
      {
        return ( dataInRange(l0,MaxL0) ?
                 set( l0, ShiftL0, MaskL0 ) : false );
      }
    private: // methods
      /// Set the data value for a given mask and shift value
      inline bool set( const ShortType value,
                       const IndexType shift,
                       const ShortType  mask ) noexcept
      {
        setData( ((value << shift) & mask) | (data() & ~mask) );
        return true;
      }
      /// tests whether a given value is in range for a given data field
      inline bool dataInRange( const ShortType value,
                               const ShortType max ) const noexcept
      {
        return ( value <= max );
      }
    };

    //---------------------------------------------------------------------------------

    /** @class EventID RichUtils/RichDAQDefinitions.h
     *
     *  The Event ID.
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   24/01/2007
     */
    class EventID final : public NumericType<LLongType>
    {
    public:
      /// Default constructor
      EventID( ) = default;
      /// Copy Constructor
      EventID( const EventID& ) = default;
      /// Constructor from value and number of bits
      template<class NUMTYPE>
      EventID ( const NUMTYPE   id,
                const ShortType aBits )
        : NumericType<LLongType> ( (LLongType)id ),
          m_nActiveBits          ( aBits         ) { }
      /// Constructor from value
      template<class NUMTYPE>
      explicit EventID ( const NUMTYPE id )
        : NumericType<LLongType> ( (LLongType)id     ),
          m_nActiveBits          ( 8*sizeof(NUMTYPE) ) { }
      /// Return the number of active bits
      inline ShortType activeBits() const noexcept { return m_nActiveBits; }
      /// Set the number of active bits
      inline void setActiveBits(const ShortType bits) noexcept { m_nActiveBits = bits; }
    public:
      /// Operator LLongtype
      inline operator LLongType() const noexcept { return data(); }
    public:
      /// Overloaded output to ostream
      friend inline std::ostream & operator << ( std::ostream & evtID_os, const EventID & id )
      {
        evtID_os << "[ ID=" << id.data();
        evtID_os << " Hex="; id.hexDump(evtID_os);
        evtID_os << " Bits("<< id.activeBits() <<")=";
        id.bitsDump(evtID_os,id.activeBits(),"");
        return evtID_os << " ]";
      }
    public:
      /// Operator == that takes into account the correct number of bits
      inline bool operator== ( const EventID& id ) const noexcept
      {
        // Compute which how many bits the words should in common, so we only compare these
        const auto lowBits = ( this->activeBits() < id.activeBits() ?
                               this->activeBits() : id.activeBits() );
        const auto mask = ((1 << lowBits)-1);
        // compare the bits and return
        return ( (this->data() & mask) == (id.data() & mask) );
      }
      /// Operator != that takes into account the correct number of bits
      inline bool operator!= ( const EventID& id ) const noexcept
      {
        return ! this->operator==(id);
      }
    private:
      /// Number of sensitive bits in this EventID
      ShortType m_nActiveBits { 8*sizeof(LLongType) };
    };

    /** @class BXID RichUtils/RichDAQDefinitions.h
     *
     *  The BX ID.
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   24/01/2007
     */
    class BXID final : public NumericType<LongType>
    {
    public:
      /// Default constructor
      BXID( ) = default;
      /// Copy Constructor
      BXID( const BXID& ) = default;
      /// Constructor from value
      template<class NUMTYPE>
      explicit BXID ( const NUMTYPE id ) noexcept
        : NumericType<LongType> ( (LongType)id      ),
          m_nActiveBits         ( 8*sizeof(NUMTYPE) ) { }
      /// Constructor from value and number of bits
      template<class NUMTYPE>
      BXID ( const NUMTYPE   id,
             const ShortType aBits )
        : NumericType<LongType> ( (LongType)id ),
          m_nActiveBits         ( aBits        ) { }
      /// Return the number of active bits
      inline ShortType activeBits() const noexcept { return m_nActiveBits; }
      /// Set the number of active bits
      inline void setActiveBits(const ShortType bits) noexcept { m_nActiveBits = bits; }
    public:
      /// Operator LongType
      inline operator LongType() const noexcept { return data(); }
    public:
      /// Overloaded output to ostream
      friend inline std::ostream & operator << ( std::ostream & os, const BXID & id )
      {
        os << "[ ID=" << id.data();
        os << " Hex="; id.hexDump(os);
        os << " Bits("<< id.activeBits() <<")=";
        id.bitsDump(os,id.activeBits(),"");
        return os << " ]";
      }
    public:
      /// Operator == that takes into account the correct number of bits
      inline bool operator== ( const BXID& id ) const noexcept
      {
        // Compute which how many bits the words should in common, so we only compare these
        const auto lowBits = ( this->activeBits() < id.activeBits() ?
                               this->activeBits() : id.activeBits() );
        const auto mask = ((1 << lowBits)-1);
        // compare the bits and return
        return ( (this->data() & mask) == (id.data() & mask) );
      }
      /// Operator != that takes into account the correct number of bits
      inline bool operator!= ( const BXID& id ) const noexcept
      {
        return ! this->operator==(id);
      }
    private:
      /// Number of sensitive bits in this BXID
      ShortType m_nActiveBits { 8*sizeof(LongType) };
    };

    /** @class Level1LogicalID RichUtils/RichDAQDefinitions.h
     *
     *  The Level 1 board (logical) ID.
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   11/11/2005
     */
    class Level1LogicalID final : public NumericType<LongType>
    {
    public :
      /// Default Constructor
      Level1LogicalID() = default;
      /// Constructor with value
      explicit Level1LogicalID ( const LongType id ) noexcept
        : NumericType<LongType>(id) { }
    };

    /** @class Level1HardwareID RichUtils/RichDAQDefinitions.h
     *
     *  The Level 1 board (hardware) ID.
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   11/11/2005
     */
    class Level1HardwareID final : public NumericType<ShortType>
    {
    public :
      /// Default Constructor
      Level1HardwareID() = default;
      /// Constructor with value
      explicit Level1HardwareID ( const ShortType id ) noexcept
        : NumericType<ShortType>(id) { }
    };

    /** @class L1IngressID RichUtils/RichDAQDefinitions.h
     *
     *  The L1 ingress number (which of the four groups of inputs to each L1 board)
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   17/01/2007
     */
    class L1IngressID final : public NumericType<ShortType>
    {
    public :
      /// Default Constructor
      L1IngressID() = default;
      /// Constructor with value
      explicit L1IngressID ( const ShortType id ) noexcept
        : NumericType<ShortType>(id) { }
    };

    /** @class L1InputWithinIngress RichUtils/RichDAQDefinitions.h
     *
     *  The input number within a given L1 ingress
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   17/01/2007
     */
    class L1InputWithinIngress final : public NumericType<ShortType>
    {
    public :
      /// Default Constructor
      L1InputWithinIngress() = default;
      /// Constructor with value
      explicit L1InputWithinIngress ( const ShortType id  )
        : NumericType<ShortType>(id) { }
    };

    /** @class Level1Input RichUtils/RichDAQDefinitions.h
     *
     *  The Level 1 board input number.
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   11/11/2005
     */
    class Level1Input final : public NumericType<ShortType>
    {
    public :
      /// Default Constructor
      Level1Input() = default;
      /// Constructor with value
      explicit Level1Input ( const ShortType id ) noexcept
        : NumericType<ShortType>(id) { }
      /// Constructor from an Ingress ID and Ingress input number
      Level1Input( const L1IngressID ingress,       ///< The ingress ID
                   const L1InputWithinIngress input ///< Input number within an ingress
                   )
        : NumericType<ShortType>( ingress.data()*NumL1InputsPerIngress + input.data() )
      { }
      /// Returns the input number within a given ingress (0-8)
      inline L1InputWithinIngress l1InputWithinIngress() const
      {
        return L1InputWithinIngress( data() % NumL1InputsPerIngress );
      }
      /// Returns the L1 ingress number
      inline L1IngressID ingressID() const
      {
        return L1IngressID ( data() / NumL1InputsPerIngress );
      }
    };

    /** @class HPDHardwareID RichUtils/RichDAQDefinitions.h
     *
     *  The (numeric) HPD hardware ID. Unique to each HPD and can be
     *  used to locate its physical properties, such as Q.E. curves.
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   11/11/2005
     */
    class HPDHardwareID final : public NumericType<LongType>
    {
    public :
      /// Default Constructor
      HPDHardwareID() = default;
      /// Constructor with value
      explicit HPDHardwareID ( const LongType id ) noexcept
        : NumericType<LongType>(id) { }
    };

    /** @class HPDL1InputID RichUtils/RichDAQDefinitions.h
     *
     *  The HPD Level1 input ID. The Level1 board number and the input
     *  number on that board for a given HPD bit-packed into a single word.
     *
     *  The most significant 8 bits gives the L1 board number.
     *  The least significant 8 bits gives the L1 board input number.
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   11/11/2005
     */
    class HPDL1InputID final : public NumericType<ShortType>
    {
    public :
      // Define the number of bits for each field
      static const IndexType  BitsIn =  8; ///< Number of bits for input number
      static const IndexType  BitsB  =  8; ///< Number of bits for board number
      // Create the shift registers
      static const IndexType  ShiftIn = 0;
      static const IndexType  ShiftB  = ShiftIn + BitsIn;
      // Create the Masks
      static const ShortType  MaskIn = ((1 << BitsIn)-1) << ShiftIn;
      static const ShortType  MaskB  = ((1 << BitsB) -1) << ShiftB ;
      // Create the max values that can be stored in each field
      static const ShortType  MaxIn = ( 1 << BitsIn ) - 1;
      static const ShortType  MaxB  = ( 1 << BitsB ) - 1;
    public :
      /// Default Constructor
      HPDL1InputID() = default;
      /// Constructor from bit packed word
      explicit HPDL1InputID ( const ShortType id ) noexcept
        : NumericType<ShortType>(id) { }
      /// Constructor from a L1 ID and input number
      HPDL1InputID ( const Level1HardwareID l1ID,    ///< The L1 board hardware ID
                     const Level1Input input ///< L1 input number
                     )
      {
        setBoardNumber(l1ID);
        setInputNumber(input);
      }
      /// Return the Level1 board number
      inline Level1HardwareID boardNumber() const noexcept
      {
        return Level1HardwareID( (data() & MaskB) >> ShiftB );
      }
      /// Set the Level1 board number
      inline bool setBoardNumber( const Level1HardwareID board ) noexcept
      {
        return ( dataInRange(board.data(),MaxB) ?
                 set( board.data(), ShiftB, MaskB ) : false );
      }
      /// Return the input number
      inline Level1Input inputNumber() const noexcept
      {
        return Level1Input( (data() & MaskIn) >> ShiftIn );
      }
      /// Set the input number
      inline bool setInputNumber( const Level1Input input ) noexcept
      {
        return ( dataInRange(input.data(),MaxIn) ?
                 set( input.data(), ShiftIn, MaskIn ) : false );
      }
    private: // methods
      /// Set the data value for a given mask and shift value
      inline bool set( const ShortType value,
                       const IndexType shift,
                       const ShortType  mask ) noexcept
      {
        setData( ((value << shift) & mask) | (data() & ~mask) );
        return true;
      }
      /// tests whether a given value is in range for a given data field
      inline bool dataInRange( const ShortType value,
                               const ShortType max ) const noexcept
      {
        return ( value <= max );
      }
    };

    /** @class PDCopyNumber RichUtils/RichDAQDefinitions.h
     *
     *  The Geant4 copy number equivalent. A different way to locate HPDs.
     *  The number starts at 0 at Rich1,Top,HPDColumn=0,NumInCol=0 and reaches
     *  483 at Rich2,Right,HPDColumn=8,NumInCol=15
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   24/07/2008
     */
    class PDCopyNumber final : public NumericType<ShortType>
    {
    public :
      /// Default Constructor
      PDCopyNumber() = default;
      /// Constructor with value
      explicit PDCopyNumber ( const ShortType id ) noexcept
        : NumericType<ShortType>(id) { }
    };
    /// Temporary alias (to be removed)
    using HPDCopyNumber = PDCopyNumber;

    /** @class PDPanelIndex RichUtils/RichDAQDefinitions.h
     *
     *  PD Index within a panel. Does not neccessary run concurrently.
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   24/07/2008
     */
    class PDPanelIndex final : public NumericType<ShortType>
    {
    public :
      /// Default Constructor
      PDPanelIndex() = default;
      /// Constructor with value
      explicit PDPanelIndex ( const ShortType id ) noexcept
        : NumericType<ShortType>(id) { }
    };

    /** @class  Level1CopyNumber RichUtils/RichDAQDefinitions.h
     *
     *  Copy number for L1 boards. Starts at 0 in RICH1 and increments
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   11/11/2010
     */
    class Level1CopyNumber final : public NumericType<ShortType>
    {
    public :
      /// Default Constructor
      Level1CopyNumber() = default;
      /// Constructor with value
      explicit Level1CopyNumber ( const ShortType id ) noexcept
        : NumericType<ShortType>(id) { }
    };

    //--------------------------------------------------------------------------------------

    /// Local definition of a RawBank constructed from 32bit ints
    //using RAWBank           = std::vector< LongType >;
    typedef std::vector< LongType > RAWBank;

    /// Vector of HPD Hardware IDs
    //using HPDHardwareIDs    = std::vector< HPDHardwareID >;
    typedef std::vector< HPDHardwareID > HPDHardwareIDs;

    /// Vector of Level 0 IDs
    //using Level0IDs         = std::vector< Level0ID >;
    typedef std::vector< Level0ID > Level0IDs;

    /// Vector of Level 1 hardware IDs
    //using Level1HardwareIDs = std::vector< Level1HardwareID >;
    typedef std::vector< Level1HardwareID > Level1HardwareIDs;

    /// Vector of Level 1 logical IDs
    //using Level1LogicalIDs  = std::vector< Level1LogicalID >;
    typedef std::vector< Level1LogicalID > Level1LogicalIDs;

    /// Vector of L1InputWithinIngress
    //using L1IngressInputs   = std::vector< L1InputWithinIngress >;
    typedef std::vector< L1InputWithinIngress > L1IngressInputs;

    //--------------------------------------------------------------------------------------

    /** @enum BankVersion
     *
     *  Enumeration for the RICH DAQ Level1 bank versions
     *
     *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
     *  @date   20/12/2004
     */
    enum BankVersion
      {
        /// Undefined
        UndefinedBankVersion = -1,
        /// Compatible with DC04
        LHCb0  = 0,
        /// Same as LHCb0 with new header + Level1 grouping
        LHCb1  = 1,
        /// Same as LHCb1 with new zero suppression format. Compatible with DC06
        LHCb2  = 2,
        /// Similar to LHCb4, but with "normal" (small) headers. Never used in practise
        LHCb3  = 127,
        /// Version compatible with first L1 firmware, as used in 2006 September testbeam
        LHCb4  = 128,
        /// Version compatible with the second (final?) version of the L1 firmware
        LHCb5  = 129,
        /// HPD 'SmartID' Data blocks format
        FlatList = 4,
        /// Basic MaPMT format
        MaPMT0 = 3
      };

    //---------------------------------------------------------------------------------

  }
}

//---------------------------------------------------------------------------------

// Hash functions
// Needed in order to allow these classes to be used as keys in Hash maps
// CRJ : Are these really needed ?

#ifdef __GNUC__
//namespace __gnu_cxx
namespace std
{

  /// Level0ID hash function
  template <> struct hash<Rich::DAQ::Level0ID>
  { inline size_t operator() ( Rich::DAQ::Level0ID id ) const { return (size_t)id.data(); } } ;
  /// Level0ID hash function
  template <> struct hash<Rich::DAQ::Level0ID&>
  { inline size_t operator() ( Rich::DAQ::Level0ID id ) const { return (size_t)id.data(); } } ;
  /// Level0ID hash function
  template <> struct hash<const Rich::DAQ::Level0ID>
  { inline size_t operator() ( const Rich::DAQ::Level0ID id ) const { return (size_t)id.data(); } } ;
  /// Level0ID hash function
  template <> struct hash<const Rich::DAQ::Level0ID&>
  { inline size_t operator() ( const Rich::DAQ::Level0ID id ) const { return (size_t)id.data(); } } ;

  /// Level1HardwareID hash function
  template <> struct hash<Rich::DAQ::Level1HardwareID>
  { inline size_t operator() ( Rich::DAQ::Level1HardwareID id ) const { return (size_t)id.data(); } } ;
  /// Level1HardwareID hash function
  template <> struct hash<Rich::DAQ::Level1HardwareID&>
  { inline size_t operator() ( Rich::DAQ::Level1HardwareID id ) const { return (size_t)id.data(); } } ;
  /// Level1HardwareID hash function
  template <> struct hash<const Rich::DAQ::Level1HardwareID>
  { inline size_t operator() ( const Rich::DAQ::Level1HardwareID id ) const { return (size_t)id.data(); } } ;
  /// Level1HardwareID hash function
  template <> struct hash<const Rich::DAQ::Level1HardwareID&>
  { inline size_t operator() ( const Rich::DAQ::Level1HardwareID id ) const { return (size_t)id.data(); } } ;

  /// Level1LogicalID hash function
  template <> struct hash<Rich::DAQ::Level1LogicalID>
  { inline size_t operator() ( Rich::DAQ::Level1LogicalID id ) const { return (size_t)id.data(); } } ;
  /// Level1LogicalID hash function
  template <> struct hash<Rich::DAQ::Level1LogicalID&>
  { inline size_t operator() ( Rich::DAQ::Level1LogicalID id ) const { return (size_t)id.data(); } } ;
  /// Level1LogicalID hash function
  template <> struct hash<const Rich::DAQ::Level1LogicalID>
  { inline size_t operator() ( const Rich::DAQ::Level1LogicalID id ) const { return (size_t)id.data(); } } ;
  /// Level1LogicalID hash function
  template <> struct hash<const Rich::DAQ::Level1LogicalID&>
  { inline size_t operator() ( const Rich::DAQ::Level1LogicalID id ) const { return (size_t)id.data(); } } ;

  /// L1IngressID hash function
  template <> struct hash<Rich::DAQ::L1IngressID>
  { inline size_t operator() ( Rich::DAQ::L1IngressID id ) const { return (size_t)id.data(); } } ;
  /// L1IngressID hash function
  template <> struct hash<Rich::DAQ::L1IngressID&>
  { inline size_t operator() ( Rich::DAQ::L1IngressID id ) const { return (size_t)id.data(); } } ;
  /// L1IngressID hash function
  template <> struct hash<const Rich::DAQ::L1IngressID>
  { inline size_t operator() ( const Rich::DAQ::L1IngressID id ) const { return (size_t)id.data(); } } ;
  /// L1IngressID hash function
  template <> struct hash<const Rich::DAQ::L1IngressID&>
  { inline size_t operator() ( const Rich::DAQ::L1IngressID id ) const { return (size_t)id.data(); } } ;

  /// L1InputWithinIngress hash function
  template <> struct hash<Rich::DAQ::L1InputWithinIngress>
  { inline size_t operator() ( Rich::DAQ::L1InputWithinIngress id ) const { return (size_t)id.data(); } } ;
  /// L1InputWithinIngress hash function
  template <> struct hash<Rich::DAQ::L1InputWithinIngress&>
  { inline size_t operator() ( Rich::DAQ::L1InputWithinIngress id ) const { return (size_t)id.data(); } } ;
  /// L1InputWithinIngress hash function
  template <> struct hash<const Rich::DAQ::L1InputWithinIngress>
  { inline size_t operator() ( const Rich::DAQ::L1InputWithinIngress id ) const { return (size_t)id.data(); } } ;
  /// L1InputWithinIngress hash function
  template <> struct hash<const Rich::DAQ::L1InputWithinIngress&>
  { inline size_t operator() ( const Rich::DAQ::L1InputWithinIngress id ) const { return (size_t)id.data(); } } ;

  /// Level1Input hash function
  template <> struct hash<Rich::DAQ::Level1Input>
  { inline size_t operator() ( Rich::DAQ::Level1Input id ) const { return (size_t)id.data(); } } ;
  /// Level1Input hash function
  template <> struct hash<Rich::DAQ::Level1Input&>
  { inline size_t operator() ( Rich::DAQ::Level1Input id ) const { return (size_t)id.data(); } } ;
  /// Level1Input hash function
  template <> struct hash<const Rich::DAQ::Level1Input>
  { inline size_t operator() ( const Rich::DAQ::Level1Input id ) const { return (size_t)id.data(); } } ;
  /// Level1Input hash function
  template <> struct hash<const Rich::DAQ::Level1Input&>
  { inline size_t operator() ( const Rich::DAQ::Level1Input id ) const { return (size_t)id.data(); } } ;

  /// HPDHardwareID hash function
  template <> struct hash<Rich::DAQ::HPDHardwareID>
  { inline size_t operator() ( Rich::DAQ::HPDHardwareID id ) const { return (size_t)id.data(); } } ;
  /// HPDHardwareID hash function
  template <> struct hash<Rich::DAQ::HPDHardwareID&>
  { inline size_t operator() ( Rich::DAQ::HPDHardwareID id ) const { return (size_t)id.data(); } } ;
  /// HPDHardwareID hash function
  template <> struct hash<const Rich::DAQ::HPDHardwareID>
  { inline size_t operator() ( const Rich::DAQ::HPDHardwareID id ) const { return (size_t)id.data(); } } ;
  /// HPDHardwareID hash function
  template <> struct hash<const Rich::DAQ::HPDHardwareID&>
  { inline size_t operator() ( const Rich::DAQ::HPDHardwareID id ) const { return (size_t)id.data(); } } ;

  /// HPDL1InputID hash function
  template <> struct hash<Rich::DAQ::HPDL1InputID>
  { inline size_t operator() ( Rich::DAQ::HPDL1InputID id ) const { return (size_t)id.data(); } } ;
  /// HPDL1InputID hash function
  template <> struct hash<Rich::DAQ::HPDL1InputID&>
  { inline size_t operator() ( Rich::DAQ::HPDL1InputID id ) const { return (size_t)id.data(); } } ;
  /// HPDL1InputID hash function
  template <> struct hash<const Rich::DAQ::HPDL1InputID>
  { inline size_t operator() ( const Rich::DAQ::HPDL1InputID id ) const { return (size_t)id.data(); } } ;
  /// HPDL1InputID hash function
  template <> struct hash<const Rich::DAQ::HPDL1InputID&>
  { inline size_t operator() ( const Rich::DAQ::HPDL1InputID id ) const { return (size_t)id.data(); } } ;

  /// PDCopyNumber hash function
  template <> struct hash<Rich::DAQ::PDCopyNumber>
  { inline size_t operator() ( Rich::DAQ::PDCopyNumber id ) const { return (size_t)id.data(); } } ;
  /// PDCopyNumber hash function
  template <> struct hash<Rich::DAQ::PDCopyNumber&>
  { inline size_t operator() ( Rich::DAQ::PDCopyNumber id ) const { return (size_t)id.data(); } } ;
  /// PDCopyNumber hash function
  template <> struct hash<const Rich::DAQ::PDCopyNumber>
  { inline size_t operator() ( const Rich::DAQ::PDCopyNumber id ) const { return (size_t)id.data(); } } ;
  /// PDCopyNumber hash function
  template <> struct hash<const Rich::DAQ::PDCopyNumber&>
  { inline size_t operator() ( const Rich::DAQ::PDCopyNumber id ) const { return (size_t)id.data(); } } ;

  /// Level1CopyNumber hash function
  template <> struct hash<Rich::DAQ::Level1CopyNumber>
  { inline size_t operator() ( Rich::DAQ::Level1CopyNumber id ) const { return (size_t)id.data(); } } ;
  /// Level1CopyNumber hash function
  template <> struct hash<Rich::DAQ::Level1CopyNumber&>
  { inline size_t operator() ( Rich::DAQ::Level1CopyNumber id ) const { return (size_t)id.data(); } } ;
  /// Level1CopyNumber hash function
  template <> struct hash<const Rich::DAQ::Level1CopyNumber>
  { inline size_t operator() ( const Rich::DAQ::Level1CopyNumber id ) const { return (size_t)id.data(); } } ;
  /// Level1CopyNumber hash function
  template <> struct hash<const Rich::DAQ::Level1CopyNumber&>
  { inline size_t operator() ( const Rich::DAQ::Level1CopyNumber id ) const { return (size_t)id.data(); } } ;

}

namespace GaudiUtils
{

  /// Level0ID Hash function
  template <> struct Hash<Rich::DAQ::Level0ID>
  { inline size_t operator() ( Rich::DAQ::Level0ID id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level0ID Hash function
  template <> struct Hash<Rich::DAQ::Level0ID&>
  { inline size_t operator() ( Rich::DAQ::Level0ID id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level0ID Hash function
  template <> struct Hash<const Rich::DAQ::Level0ID>
  { inline size_t operator() ( const Rich::DAQ::Level0ID id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level0ID Hash function
  template <> struct Hash<const Rich::DAQ::Level0ID&>
  { inline size_t operator() ( const Rich::DAQ::Level0ID id ) const noexcept { return (size_t)id.data(); } } ;

  /// Level1HardwareID Hash function
  template <> struct Hash<Rich::DAQ::Level1HardwareID>
  { inline size_t operator() ( Rich::DAQ::Level1HardwareID id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level1HardwareID Hash function
  template <> struct Hash<Rich::DAQ::Level1HardwareID&>
  { inline size_t operator() ( Rich::DAQ::Level1HardwareID id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level1HardwareID Hash function
  template <> struct Hash<const Rich::DAQ::Level1HardwareID>
  { inline size_t operator() ( const Rich::DAQ::Level1HardwareID id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level1HardwareID Hash function
  template <> struct Hash<const Rich::DAQ::Level1HardwareID&>
  { inline size_t operator() ( const Rich::DAQ::Level1HardwareID id ) const noexcept { return (size_t)id.data(); } } ;

  /// Level1LogicalID Hash function
  template <> struct Hash<Rich::DAQ::Level1LogicalID>
  { inline size_t operator() ( Rich::DAQ::Level1LogicalID id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level1LogicalID Hash function
  template <> struct Hash<Rich::DAQ::Level1LogicalID&>
  { inline size_t operator() ( Rich::DAQ::Level1LogicalID id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level1LogicalID Hash function
  template <> struct Hash<const Rich::DAQ::Level1LogicalID>
  { inline size_t operator() ( const Rich::DAQ::Level1LogicalID id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level1LogicalID Hash function
  template <> struct Hash<const Rich::DAQ::Level1LogicalID&>
  { inline size_t operator() ( const Rich::DAQ::Level1LogicalID id ) const noexcept { return (size_t)id.data(); } } ;

  /// L1IngressID Hash function
  template <> struct Hash<Rich::DAQ::L1IngressID>
  { inline size_t operator() ( Rich::DAQ::L1IngressID id ) const noexcept { return (size_t)id.data(); } } ;
  /// L1IngressID Hash function
  template <> struct Hash<Rich::DAQ::L1IngressID&>
  { inline size_t operator() ( Rich::DAQ::L1IngressID id ) const noexcept { return (size_t)id.data(); } } ;
  /// L1IngressID Hash function
  template <> struct Hash<const Rich::DAQ::L1IngressID>
  { inline size_t operator() ( const Rich::DAQ::L1IngressID id ) const noexcept { return (size_t)id.data(); } } ;
  /// L1IngressID Hash function
  template <> struct Hash<const Rich::DAQ::L1IngressID&>
  { inline size_t operator() ( const Rich::DAQ::L1IngressID id ) const noexcept { return (size_t)id.data(); } } ;

  /// L1InputWithinIngress Hash function
  template <> struct Hash<Rich::DAQ::L1InputWithinIngress>
  { inline size_t operator() ( Rich::DAQ::L1InputWithinIngress id ) const noexcept { return (size_t)id.data(); } } ;
  /// L1InputWithinIngress Hash function
  template <> struct Hash<Rich::DAQ::L1InputWithinIngress&>
  { inline size_t operator() ( Rich::DAQ::L1InputWithinIngress id ) const noexcept { return (size_t)id.data(); } } ;
  /// L1InputWithinIngress Hash function
  template <> struct Hash<const Rich::DAQ::L1InputWithinIngress>
  { inline size_t operator() ( const Rich::DAQ::L1InputWithinIngress id ) const noexcept { return (size_t)id.data(); } } ;
  /// L1InputWithinIngress Hash function
  template <> struct Hash<const Rich::DAQ::L1InputWithinIngress&>
  { inline size_t operator() ( const Rich::DAQ::L1InputWithinIngress id ) const noexcept { return (size_t)id.data(); } } ;

  /// Level1Input Hash function
  template <> struct Hash<Rich::DAQ::Level1Input>
  { inline size_t operator() ( Rich::DAQ::Level1Input id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level1Input Hash function
  template <> struct Hash<Rich::DAQ::Level1Input&>
  { inline size_t operator() ( Rich::DAQ::Level1Input id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level1Input Hash function
  template <> struct Hash<const Rich::DAQ::Level1Input>
  { inline size_t operator() ( const Rich::DAQ::Level1Input id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level1Input Hash function
  template <> struct Hash<const Rich::DAQ::Level1Input&>
  { inline size_t operator() ( const Rich::DAQ::Level1Input id ) const noexcept { return (size_t)id.data(); } } ;

  /// HPDHardwareID Hash function
  template <> struct Hash<Rich::DAQ::HPDHardwareID>
  { inline size_t operator() ( Rich::DAQ::HPDHardwareID id ) const noexcept { return (size_t)id.data(); } } ;
  /// HPDHardwareID Hash function
  template <> struct Hash<Rich::DAQ::HPDHardwareID&>
  { inline size_t operator() ( Rich::DAQ::HPDHardwareID id ) const noexcept { return (size_t)id.data(); } } ;
  /// HPDHardwareID Hash function
  template <> struct Hash<const Rich::DAQ::HPDHardwareID>
  { inline size_t operator() ( const Rich::DAQ::HPDHardwareID id ) const noexcept { return (size_t)id.data(); } } ;
  /// HPDHardwareID Hash function
  template <> struct Hash<const Rich::DAQ::HPDHardwareID&>
  { inline size_t operator() ( const Rich::DAQ::HPDHardwareID id ) const noexcept { return (size_t)id.data(); } } ;

  /// HPDL1InputID Hash function
  template <> struct Hash<Rich::DAQ::HPDL1InputID>
  { inline size_t operator() ( Rich::DAQ::HPDL1InputID id ) const noexcept { return (size_t)id.data(); } } ;
  /// HPDL1InputID Hash function
  template <> struct Hash<Rich::DAQ::HPDL1InputID&>
  { inline size_t operator() ( Rich::DAQ::HPDL1InputID id ) const noexcept { return (size_t)id.data(); } } ;
  /// HPDL1InputID Hash function
  template <> struct Hash<const Rich::DAQ::HPDL1InputID>
  { inline size_t operator() ( const Rich::DAQ::HPDL1InputID id ) const noexcept { return (size_t)id.data(); } } ;
  /// HPDL1InputID Hash function
  template <> struct Hash<const Rich::DAQ::HPDL1InputID&>
  { inline size_t operator() ( const Rich::DAQ::HPDL1InputID id ) const noexcept { return (size_t)id.data(); } } ;

  /// PDCopyNumber Hash function
  template <> struct Hash<Rich::DAQ::PDCopyNumber>
  { inline size_t operator() ( Rich::DAQ::PDCopyNumber id ) const noexcept { return (size_t)id.data(); } } ;
  /// PDCopyNumber Hash function
  template <> struct Hash<Rich::DAQ::PDCopyNumber&>
  { inline size_t operator() ( Rich::DAQ::PDCopyNumber id ) const noexcept { return (size_t)id.data(); } } ;
  /// PDCopyNumber Hash function
  template <> struct Hash<const Rich::DAQ::PDCopyNumber>
  { inline size_t operator() ( const Rich::DAQ::PDCopyNumber id ) const noexcept { return (size_t)id.data(); } } ;
  /// PDCopyNumber Hash function
  template <> struct Hash<const Rich::DAQ::PDCopyNumber&>
  { inline size_t operator() ( const Rich::DAQ::PDCopyNumber id ) const noexcept { return (size_t)id.data(); } } ;

  /// Level1CopyNumber Hash function
  template <> struct Hash<Rich::DAQ::Level1CopyNumber>
  { inline size_t operator() ( Rich::DAQ::Level1CopyNumber id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level1CopyNumber Hash function
  template <> struct Hash<Rich::DAQ::Level1CopyNumber&>
  { inline size_t operator() ( Rich::DAQ::Level1CopyNumber id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level1CopyNumber Hash function
  template <> struct Hash<const Rich::DAQ::Level1CopyNumber>
  { inline size_t operator() ( const Rich::DAQ::Level1CopyNumber id ) const noexcept { return (size_t)id.data(); } } ;
  /// Level1CopyNumber Hash function
  template <> struct Hash<const Rich::DAQ::Level1CopyNumber&>
  { inline size_t operator() ( const Rich::DAQ::Level1CopyNumber id ) const noexcept { return (size_t)id.data(); } } ;

}

#endif

//----------------------------------------------------------------------------------------------------
namespace Rich
{
  namespace DAQ
  {

    /// Map for RICH PD data, sorted by PD number
    using PDMap = std::map< const LHCb::RichSmartID, LHCb::RichSmartID::Vector >;

    /// Mapping from Level1 ID to list of PD RichSmartIDs
    using L1ToSmartIDs = GaudiUtils::HashMap< const Level1HardwareID, LHCb::RichSmartID::Vector >;
    /// Pair type in a L1ToSmartIDs
    using L1ToSmartIDsPair = std::pair< const Level1HardwareID, LHCb::RichSmartID::Vector >;

    /// Mapping from Level1 ID to list of PD RichSmartIDs
    using L1ToHardIDs = GaudiUtils::HashMap< const Level1HardwareID, HPDHardwareIDs >;
    /// Pair type in a L1ToHardIDs
    using L1ToHardIDsPair = std::pair< const Level1HardwareID, HPDHardwareIDs >;

  }
}
//----------------------------------------------------------------------------------------------------
