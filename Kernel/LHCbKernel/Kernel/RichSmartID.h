
#ifndef LHCbKernel_RichSmartID_H
#define LHCbKernel_RichSmartID_H 1

// STL
#include <vector>
#include <ostream>
#include <cstdint>

// Local
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichSide.h"

// Gaudi
#include "GaudiKernel/boost_allocator.h"

namespace LHCb
{

  // Forward declaration of 32 bit version
  class RichSmartID32;

  /** @class RichSmartID RichSmartID.h
   *
   *  Identifier for RICH detector objects (RICH Detector, PD panel, PD or PD pixel)
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date  24/02/2011
   */
  class RichSmartID final
  {

  public:

    /// Type for internal key
    typedef uint64_t KeyType;

    /// Vector of RichSmartIDs
    typedef std::vector<LHCb::RichSmartID> Vector;

    /// Numerical type for bit packing
    typedef uint64_t BitPackType;

    /// Type for values in data fields
    typedef uint64_t DataType;

    /// Number of bits
    static constexpr const BitPackType NBits = 64;

  private:

    /// Get the initialisation value from a value, shift and mask
    inline static constexpr KeyType initData( const BitPackType value,
                                              const BitPackType shift,
                                              const BitPackType mask ) noexcept
    {
      return ( ( value << shift ) & mask );
    }

    /** The bit-packed internal data word.
     *  Default initialisation is as an HPD ID */
    KeyType m_key { initData(HPDID,ShiftIDType,MaskIDType) };

  public:

    /// Retrieve the bit-packed internal data word
    inline constexpr KeyType key()       const noexcept { return m_key; }

    /// implicit conversion to unsigned long
    inline constexpr operator uint64_t() const noexcept { return key(); }

    /// implicit conversion to signed long
    inline           operator int64_t()  const noexcept { return as_int(); }

    /// Set the bit-packed internal data word
    inline void setKey( const LHCb::RichSmartID::KeyType value ) noexcept { m_key = value; }

  private:

    // Setup up the type bit field

    /// Number of bits to use for the PD type
    static constexpr const BitPackType BitsIDType  = 3; 
    /// Use the last bits of the word. Breaks compat with old 32 bit IDs.
    static constexpr const BitPackType ShiftIDType = (BitPackType) (NBits-BitsIDType);
    /// Mask for the PD type
    static constexpr const BitPackType MaskIDType  = (BitPackType) ( ( 1ULL << BitsIDType ) - 1ULL ) << ShiftIDType;
    /// Max possible value that can be stored in the PD type field
    static constexpr const BitPackType MaxIDType   = (BitPackType) ( 1ULL << BitsIDType ) - 1ULL;

  public:

    /** @enum IDType
     *
     *  The type of photon detector this RichSmartID represents
     *
     *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
     *  @date   25/02/2011
     */
    enum IDType : int8_t
    {
      Undefined = -1, ///< Undefined
        MaPMTID = 0,  ///< MaPMT channel 
        HPDID   = 1   ///< HPD channel
        };
    
  public:
    
    /// Access the ID type
    inline constexpr RichSmartID::IDType idType() const noexcept
    {
      return (RichSmartID::IDType) ( (key() & MaskIDType) >> ShiftIDType );
    }

  public:

    /// Set the ID type
    inline void setIDType( const LHCb:: RichSmartID::IDType type )
#ifdef NDEBUG
      noexcept
#endif
    {
#ifndef NDEBUG
      checkRange(type,MaxIDType,"IDType");
#endif
      setData( type, ShiftIDType, MaskIDType );
    }

  public:

    /** @class HPD RichSmartID.h
     *
     *  Implementation details for HPDs
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date  24/02/2011
     */
    class HPD
    {

    public:

      // Number of bits for each data field in the word
      static constexpr const BitPackType BitsPixelSubRow       = 3; ///< Number of bits for HPD sub pixel field
      static constexpr const BitPackType BitsPixelCol          = 5; ///< Number of bits for HPD pixel column
      static constexpr const BitPackType BitsPixelRow          = 5; ///< Number of bits for HPD pixel row
      static constexpr const BitPackType BitsPDNumInCol        = 5; ///< Number of bits for HPD 'number in column'
      static constexpr const BitPackType BitsPDCol             = 5; ///< Number of bits for HPD column
      static constexpr const BitPackType BitsPanel             = 1; ///< Number of bits for HPD panel
      static constexpr const BitPackType BitsRich              = 1; ///< Number of bits for RICH detector
      static constexpr const BitPackType BitsPixelSubRowIsSet  = 1;
      static constexpr const BitPackType BitsPixelColIsSet     = 1;
      static constexpr const BitPackType BitsPixelRowIsSet     = 1;
      static constexpr const BitPackType BitsPDIsSet           = 1;
      static constexpr const BitPackType BitsPanelIsSet        = 1;
      static constexpr const BitPackType BitsRichIsSet         = 1;

      // The shifts
      static constexpr const BitPackType ShiftPixelSubRow      = 0;
      static constexpr const BitPackType ShiftPixelCol         = ShiftPixelSubRow      + BitsPixelSubRow;
      static constexpr const BitPackType ShiftPixelRow         = ShiftPixelCol         + BitsPixelCol;
      static constexpr const BitPackType ShiftPDNumInCol       = ShiftPixelRow         + BitsPixelRow;
      static constexpr const BitPackType ShiftPDCol            = ShiftPDNumInCol       + BitsPDNumInCol;
      static constexpr const BitPackType ShiftPanel            = ShiftPDCol            + BitsPDCol;
      static constexpr const BitPackType ShiftRich             = ShiftPanel            + BitsPanel;
      static constexpr const BitPackType ShiftPixelSubRowIsSet = ShiftRich             + BitsRich;
      static constexpr const BitPackType ShiftPixelColIsSet    = ShiftPixelSubRowIsSet + BitsPixelSubRowIsSet;
      static constexpr const BitPackType ShiftPixelRowIsSet    = ShiftPixelColIsSet    + BitsPixelColIsSet;
      static constexpr const BitPackType ShiftPDIsSet          = ShiftPixelRowIsSet    + BitsPixelRowIsSet;
      static constexpr const BitPackType ShiftPanelIsSet       = ShiftPDIsSet          + BitsPDIsSet;
      static constexpr const BitPackType ShiftRichIsSet        = ShiftPanelIsSet       + BitsPanelIsSet;

      // The masks
      static constexpr const BitPackType MaskPixelSubRow       = (BitPackType) ((1ULL << BitsPixelSubRow)-1ULL)      << ShiftPixelSubRow;
      static constexpr const BitPackType MaskPixelCol          = (BitPackType) ((1ULL << BitsPixelCol)-1ULL)         << ShiftPixelCol;
      static constexpr const BitPackType MaskPixelRow          = (BitPackType) ((1ULL << BitsPixelRow)-1ULL)         << ShiftPixelRow;
      static constexpr const BitPackType MaskPDNumInCol        = (BitPackType) ((1ULL << BitsPDNumInCol)-1ULL)       << ShiftPDNumInCol;
      static constexpr const BitPackType MaskPDCol             = (BitPackType) ((1ULL << BitsPDCol)-1ULL)            << ShiftPDCol;
      static constexpr const BitPackType MaskPanel             = (BitPackType) ((1ULL << BitsPanel)-1ULL)            << ShiftPanel;
      static constexpr const BitPackType MaskRich              = (BitPackType) ((1ULL << BitsRich)-1ULL)             << ShiftRich;
      static constexpr const BitPackType MaskPixelSubRowIsSet  = (BitPackType) ((1ULL << BitsPixelSubRowIsSet)-1ULL) << ShiftPixelSubRowIsSet;
      static constexpr const BitPackType MaskPixelColIsSet     = (BitPackType) ((1ULL << BitsPixelColIsSet)-1ULL)    << ShiftPixelColIsSet;
      static constexpr const BitPackType MaskPixelRowIsSet     = (BitPackType) ((1ULL << BitsPixelRowIsSet)-1ULL)    << ShiftPixelRowIsSet;
      static constexpr const BitPackType MaskPDIsSet           = (BitPackType) ((1ULL << BitsPDIsSet)-1ULL)          << ShiftPDIsSet;
      static constexpr const BitPackType MaskPanelIsSet        = (BitPackType) ((1ULL << BitsPanelIsSet)-1ULL)       << ShiftPanelIsSet;
      static constexpr const BitPackType MaskRichIsSet         = (BitPackType) ((1ULL << BitsRichIsSet)-1ULL)        << ShiftRichIsSet;

      // Max Values
      static constexpr const DataType MaxPixelSubRow           = (DataType) ( 1ULL << BitsPixelSubRow  ) - 1ULL;
      static constexpr const DataType MaxPixelCol              = (DataType) ( 1ULL << BitsPixelCol     ) - 1ULL;
      static constexpr const DataType MaxPixelRow              = (DataType) ( 1ULL << BitsPixelRow     ) - 1ULL;
      static constexpr const DataType MaxPDNumInCol            = (DataType) ( 1ULL << BitsPDNumInCol   ) - 1ULL;
      static constexpr const DataType MaxPDCol                 = (DataType) ( 1ULL << BitsPDCol        ) - 1ULL;
      static constexpr const DataType MaxPanel                 = (DataType) ( 1ULL << BitsPanel        ) - 1ULL;
      static constexpr const DataType MaxRich                  = (DataType) ( 1ULL << BitsRich         ) - 1ULL;

    };

  public:

    /** @class MaPMT RichSmartID.h
     *
     *  Implementation details for MaPMTs
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date  24/02/2011
     */
    class MaPMT
    {

    public:

      // Number of bits for each data field in the word
      static constexpr const BitPackType BitsPixelCol          = 3; ///< Number of bits for MaPMT pixel column
      static constexpr const BitPackType BitsPixelRow          = 3; ///< Number of bits for MaPMT pixel row
      static constexpr const BitPackType BitsPDNumInCol        = 4; ///< Number of bits for MaPMT 'number in column'
      static constexpr const BitPackType BitsPDCol             = 9; ///< Number of bits for MaPMT column
      static constexpr const BitPackType BitsPanel             = 1; ///< Number of bits for MaPMT panel
      static constexpr const BitPackType BitsRich              = 1; ///< Number of bits for RICH detector
      static constexpr const BitPackType BitsPixelSubRowIsSet  = 1;
      static constexpr const BitPackType BitsPixelColIsSet     = 1;
      static constexpr const BitPackType BitsPixelRowIsSet     = 1;
      static constexpr const BitPackType BitsPDIsSet           = 1;
      static constexpr const BitPackType BitsPanelIsSet        = 1;
      static constexpr const BitPackType BitsRichIsSet         = 1;
      static constexpr const BitPackType BitsLargePixel        = 1;

      // The shifts
      static constexpr const BitPackType ShiftPixelCol         = 0;
      static constexpr const BitPackType ShiftPixelRow         = ShiftPixelCol         + BitsPixelCol;
      static constexpr const BitPackType ShiftPDNumInCol       = ShiftPixelRow         + BitsPixelRow;
      static constexpr const BitPackType ShiftPDCol            = ShiftPDNumInCol       + BitsPDNumInCol;
      static constexpr const BitPackType ShiftPanel            = ShiftPDCol            + BitsPDCol;
      static constexpr const BitPackType ShiftRich             = ShiftPanel            + BitsPanel;
      static constexpr const BitPackType ShiftPixelSubRowIsSet = ShiftRich             + BitsRich;
      static constexpr const BitPackType ShiftPixelColIsSet    = ShiftPixelSubRowIsSet + BitsPixelSubRowIsSet;
      static constexpr const BitPackType ShiftPixelRowIsSet    = ShiftPixelColIsSet    + BitsPixelColIsSet;
      static constexpr const BitPackType ShiftPDIsSet          = ShiftPixelRowIsSet    + BitsPixelRowIsSet;
      static constexpr const BitPackType ShiftPanelIsSet       = ShiftPDIsSet          + BitsPDIsSet;
      static constexpr const BitPackType ShiftRichIsSet        = ShiftPanelIsSet       + BitsPanelIsSet;
      static constexpr const BitPackType ShiftLargePixel       = ShiftRichIsSet        + BitsRichIsSet;

      // The masks
      static constexpr const BitPackType MaskPixelCol          = (BitPackType) ((1ULL << BitsPixelCol)-1ULL)         << ShiftPixelCol;
      static constexpr const BitPackType MaskPixelRow          = (BitPackType) ((1ULL << BitsPixelRow)-1ULL)         << ShiftPixelRow;
      static constexpr const BitPackType MaskPDNumInCol        = (BitPackType) ((1ULL << BitsPDNumInCol)-1ULL)       << ShiftPDNumInCol;
      static constexpr const BitPackType MaskPDCol             = (BitPackType) ((1ULL << BitsPDCol)-1ULL)            << ShiftPDCol;
      static constexpr const BitPackType MaskPanel             = (BitPackType) ((1ULL << BitsPanel)-1ULL)            << ShiftPanel;
      static constexpr const BitPackType MaskRich              = (BitPackType) ((1ULL << BitsRich)-1ULL)             << ShiftRich;
      static constexpr const BitPackType MaskPixelSubRowIsSet  = (BitPackType) ((1ULL << BitsPixelSubRowIsSet)-1ULL) << ShiftPixelSubRowIsSet;
      static constexpr const BitPackType MaskPixelColIsSet     = (BitPackType) ((1ULL << BitsPixelColIsSet)-1ULL)    << ShiftPixelColIsSet;
      static constexpr const BitPackType MaskPixelRowIsSet     = (BitPackType) ((1ULL << BitsPixelRowIsSet)-1ULL)    << ShiftPixelRowIsSet;
      static constexpr const BitPackType MaskPDIsSet           = (BitPackType) ((1ULL << BitsPDIsSet)-1ULL)          << ShiftPDIsSet;
      static constexpr const BitPackType MaskPanelIsSet        = (BitPackType) ((1ULL << BitsPanelIsSet)-1ULL)       << ShiftPanelIsSet;
      static constexpr const BitPackType MaskRichIsSet         = (BitPackType) ((1ULL << BitsRichIsSet)-1ULL)        << ShiftRichIsSet;
      static constexpr const BitPackType MaskLargePixel        = (BitPackType) ((1ULL << BitsLargePixel)-1ULL)       << ShiftLargePixel;

      // Max Values
      static constexpr const DataType MaxPixelCol              = (DataType) ( 1ULL << BitsPixelCol     ) - 1ULL;
      static constexpr const DataType MaxPixelRow              = (DataType) ( 1ULL << BitsPixelRow     ) - 1ULL;
      static constexpr const DataType MaxPDNumInCol            = (DataType) ( 1ULL << BitsPDNumInCol   ) - 1ULL;
      static constexpr const DataType MaxPDCol                 = (DataType) ( 1ULL << BitsPDCol        ) - 1ULL;
      static constexpr const DataType MaxPanel                 = (DataType) ( 1ULL << BitsPanel        ) - 1ULL;
      static constexpr const DataType MaxRich                  = (DataType) ( 1ULL << BitsRich         ) - 1ULL;

    };

  private:

    /// Reinterpret the internal unsigned representation as a signed 64 bit int
    inline int64_t as_int( ) const noexcept { return reinterpret_cast<const int64_t&>(m_key); }

    /// Set the given data into the given field, without validity bit
    inline void setData( const DataType value,
                         const BitPackType shift,
                         const BitPackType mask ) noexcept
    {
      setKey( ( ( value << shift ) & mask ) | ( m_key & ~mask ) );
    }

    /// Set the given data into the given field, with validity bit
    inline void setData( const DataType value,
                         const BitPackType shift,
                         const BitPackType mask,
                         const BitPackType okMask ) noexcept
    {
      setKey( ( ( value << shift ) & mask ) | ( m_key & ~mask ) | okMask );
    }

    /// Checks if a data value is within range for a given field
    inline void checkRange( const DataType value,
                            const DataType maxValue,
                            const std::string& message) const
    {
      if ( value > maxValue ) { rangeError( value, maxValue, message ); }
    }

    /// Issue an exception in the case of a range error
    void rangeError( const DataType value,
                     const DataType maxValue,
                     const std::string& message ) const;

  public:

    /// Default Constructor
    RichSmartID() { }

    /// Default Destructor
    ~RichSmartID() = default;

    /// Default Copy Constructor
    RichSmartID( const RichSmartID& ) = default;

    /// Default Copy Operator
    RichSmartID& operator=( const RichSmartID& ) = default;

    /// Default Move Constructor
    RichSmartID( RichSmartID&& ) = default;

    /// Default Move Operator
    RichSmartID& operator=( RichSmartID&& ) = default;

  public:

    /// Constructor from internal type (unsigned int)
    explicit constexpr RichSmartID( const RichSmartID::KeyType key ) noexcept
      : m_key( key ) { }

    /// Constructor from signed 64 bit int
    explicit           RichSmartID( const int64_t key ) noexcept
      : m_key( reinterpret_cast<const RichSmartID::KeyType&>(key) ) { }

    /// Constructor from a 32 bit RichSmartID, for backwards compatibility.
    explicit           RichSmartID( const RichSmartID32& id );

    /// Constructor from an unsigned 32 bit int.
    explicit           RichSmartID( const uint32_t key );

    /// Constructor from a signed 32 bit int.
    explicit           RichSmartID( const int32_t key );
    
    /// Pixel level constructor including sub-pixel information
    RichSmartID( const Rich::DetectorType rich,
                 const Rich::Side panel,
                 const DataType pdNumInCol,
                 const DataType pdCol,
                 const DataType pixelRow,
                 const DataType pixelCol,
                 const DataType pixelSubRow,
                 const IDType type = HPDID )
#ifdef NDEBUG
    noexcept
#endif
    {
      setIDType        ( type              );
      setRich          ( rich              );
      setPanel         ( panel             );
      setPD            ( pdCol, pdNumInCol );
      setPixelRow      ( pixelRow          );
      setPixelCol      ( pixelCol          );
      setPixelSubRow   ( pixelSubRow       );
    }

    /// Pixel level constructor
    RichSmartID( const Rich::DetectorType rich,
                 const Rich::Side panel,
                 const DataType pdNumInCol,
                 const DataType pdCol,
                 const DataType pixelRow,
                 const DataType pixelCol,
                 const IDType type = HPDID )
#ifdef NDEBUG
    noexcept
#endif
    {
      setIDType        ( type              );
      setRich          ( rich              );
      setPanel         ( panel             );
      setPD            ( pdCol, pdNumInCol );
      setPixelRow      ( pixelRow          );
      setPixelCol      ( pixelCol          );
    }

    /// PD level constructor
    RichSmartID( const Rich::DetectorType rich,
                 const Rich::Side panel,
                 const DataType pdNumInCol,
                 const DataType pdCol,
                 const IDType type = HPDID )
#ifdef NDEBUG
    noexcept
#endif
    {
      setIDType        ( type              );
      setRich          ( rich              );
      setPanel         ( panel             );
      setPD            ( pdCol, pdNumInCol );
    }

    /// PD panel level constructor
    RichSmartID( const Rich::DetectorType rich,
                 const Rich::Side panel,
                 const IDType type = HPDID )
#ifdef NDEBUG
    noexcept
#endif
    {
      setIDType        ( type  );
      setRich          ( rich  );
      setPanel         ( panel );
    }

  public:

    /// Equality operator
    inline constexpr bool operator==( const LHCb::RichSmartID& id ) const noexcept
    {
      return ( key() == id.key() );
    }

    /// Inequality operator
    inline constexpr bool operator!=( const LHCb::RichSmartID& id ) const noexcept
    {
      return ( key() != id.key() );
    }

    /// > operator
    inline constexpr bool operator>( const LHCb::RichSmartID& id ) const noexcept
    {
      return ( key() >  id.key() );
    }

    /// < operator
    inline constexpr bool operator<( const LHCb::RichSmartID& id ) const noexcept
    {
      return ( key() <  id.key() );
    }

    /// >= operator
    inline constexpr bool operator>=( const LHCb::RichSmartID& id ) const noexcept
    {
      return ( key() >= id.key() );
    }

    /// <= operator
    inline constexpr bool operator<=( const LHCb::RichSmartID& id ) const noexcept
    {
      return ( key() <= id.key() );
    }

  public:

    /// Set the RICH detector identifier
    inline void setRich( const Rich::DetectorType rich )
#ifdef NDEBUG
      noexcept
#endif
    {
      if ( HPDID == idType() )
      {
#ifndef NDEBUG
        checkRange( rich, HPD::MaxRich, "RICH" );
#endif
        setData( rich, HPD::ShiftRich,   HPD::MaskRich,   HPD::MaskRichIsSet   );
      }
      else // assume only two ID types ...
      {
#ifndef NDEBUG
        checkRange( rich, MaPMT::MaxRich, "RICH" );
#endif
        setData( rich, MaPMT::ShiftRich, MaPMT::MaskRich, MaPMT::MaskRichIsSet );
      }
    }

    /// Set the RICH PD panel identifier
    inline void setPanel( const Rich::Side panel )
#ifdef NDEBUG
      noexcept
#endif
    {
      if ( HPDID == idType() )
      {
#ifndef NDEBUG
        checkRange( panel, HPD::MaxPanel, "Panel" );
#endif
        setData( panel, HPD::ShiftPanel,   HPD::MaskPanel,   HPD::MaskPanelIsSet   );
      }
      else // assume only two ID types ...
      {
#ifndef NDEBUG
        checkRange( panel, MaPMT::MaxPanel, "Panel" );
#endif
        setData( panel, MaPMT::ShiftPanel, MaPMT::MaskPanel, MaPMT::MaskPanelIsSet );
      }
    }

    /// Set the RICH PD column and number in column identifier
    inline void setPD( const DataType col, const DataType nInCol )
#ifdef NDEBUG
      noexcept
#endif
    {
      if ( HPDID == idType() )
      {
#ifndef NDEBUG
        checkRange ( col,    HPD::MaxPDCol,      "PDColumn"   );
        checkRange ( nInCol, HPD::MaxPDNumInCol, "PDNumInCol" );
#endif
        setData( col,    HPD::ShiftPDCol,      HPD::MaskPDCol,      HPD::MaskPDIsSet );
        setData( nInCol, HPD::ShiftPDNumInCol, HPD::MaskPDNumInCol                   );
      }
      else // assume only two ID types ...
      {
#ifndef NDEBUG
        checkRange ( col,    MaPMT::MaxPDCol,      "PDColumn"   );
        checkRange ( nInCol, MaPMT::MaxPDNumInCol, "PDNumInCol" );
#endif
        setData( col,    MaPMT::ShiftPDCol,      MaPMT::MaskPDCol,      MaPMT::MaskPDIsSet );
        setData( nInCol, MaPMT::ShiftPDNumInCol, MaPMT::MaskPDNumInCol                     );
      }
    }

    /// Set the RICH PD pixel row identifier
    inline void setPixelRow( const DataType row )
#ifdef NDEBUG
      noexcept
#endif
    {
      if ( HPDID == idType() )
      {
#ifndef NDEBUG
        checkRange( row, HPD::MaxPixelRow, "PixelRow" );
#endif
        setData( row, HPD::ShiftPixelRow,   HPD::MaskPixelRow,   HPD::MaskPixelRowIsSet   );
      }
      else
      {
#ifndef NDEBUG
        checkRange( row, MaPMT::MaxPixelRow, "PixelRow" );
#endif
        setData( row, MaPMT::ShiftPixelRow, MaPMT::MaskPixelRow, MaPMT::MaskPixelRowIsSet );
      }
    }

    /// Set the RICH PD pixel column identifier
    inline void setPixelCol( const DataType col )
#ifdef NDEBUG
      noexcept
#endif
    {
      if ( HPDID == idType() )
      {
#ifndef NDEBUG
        checkRange( col, HPD::MaxPixelCol, "PixelColumn" );
#endif
        setData( col, HPD::ShiftPixelCol,   HPD::MaskPixelCol,   HPD::MaskPixelColIsSet   );
      }
      else
      {
#ifndef NDEBUG
        checkRange( col, MaPMT::MaxPixelCol, "PixelColumn" );
#endif
        setData( col, MaPMT::ShiftPixelCol, MaPMT::MaskPixelCol, MaPMT::MaskPixelColIsSet );
      }
    }

    /// Set the RICH photon detector pixel sub-row identifier (Alice mode only)
    void setPixelSubRow( const DataType pixelSubRow );

  public:

    /// Decoding function to strip the sub-pixel information and return a pixel RichSmartID
    inline constexpr LHCb::RichSmartID pixelID() const noexcept
    {
      return RichSmartID( HPDID == idType()                                         ?
                          key() & ~(HPD::MaskPixelSubRow+HPD::MaskPixelSubRowIsSet) :
                          key()                                                     );
    }

    /// Decoding function to return an identifier for a single PD, stripping all pixel level information
    inline constexpr LHCb::RichSmartID pdID() const noexcept
    {
      return RichSmartID( key() &
                          ( HPDID == idType()                                                          ?
                            (HPD::MaskRich+HPD::MaskPanel+HPD::MaskPDNumInCol+HPD::MaskPDCol+
                             HPD::MaskRichIsSet+HPD::MaskPanelIsSet+HPD::MaskPDIsSet+MaskIDType)       :
                            (MaPMT::MaskRich+MaPMT::MaskPanel+MaPMT::MaskPDNumInCol+MaPMT::MaskPDCol+
                             MaPMT::MaskRichIsSet+MaPMT::MaskPanelIsSet+MaPMT::MaskPDIsSet+MaskIDType) )
                          );
    }

    /// Decoding function to strip the photon-detector information and return a PD panel RichSmartID
    inline constexpr LHCb::RichSmartID panelID() const noexcept
    {
      return RichSmartID( key() &
                          ( HPDID == idType()                                         ?
                            (HPD::MaskRich+HPD::MaskPanel+HPD::MaskRichIsSet+
                             HPD::MaskPanelIsSet+MaskIDType)                          :
                            (MaPMT::MaskRich+MaPMT::MaskPanel+MaPMT::MaskRichIsSet+
                             MaPMT::MaskPanelIsSet+MaskIDType)                        )
                          );
    }

    /// Decoding function to strip all but the RICH information and return a RICH RichSmartID
    inline constexpr LHCb::RichSmartID richID() const noexcept
    {
      return RichSmartID( key() &
                          ( HPDID == idType()                                  ?
                            (HPD::MaskRich+HPD::MaskRichIsSet+MaskIDType)      :
                            (MaPMT::MaskRich+MaPMT::MaskRichIsSet+MaskIDType)  )
                          );
    }

    /// Returns only the data fields, with the validity bits stripped
    inline constexpr LHCb::RichSmartID dataBitsOnly() const noexcept
    {
      return RichSmartID( key() &
                          ( HPDID == idType()                                                  ?
                            (HPD::MaskRich+HPD::MaskPanel+HPD::MaskPDNumInCol+HPD::MaskPDCol+
                             HPD::MaskPixelRow+HPD::MaskPixelCol+HPD::MaskPixelSubRow)         :
                            (MaPMT::MaskRich+MaPMT::MaskPanel+MaPMT::MaskPDNumInCol+
                             MaPMT::MaskPDCol+MaPMT::MaskPixelRow+MaPMT::MaskPixelCol)         )
                          );
    }

  public:

    /// Retrieve The pixel sub-row (Alice mode) number
    inline constexpr DataType pixelSubRow() const noexcept
    {
      // Note MaPMTs have no sub-pixel ...
      return (DataType) ( HPDID == idType() ?
                          ((key() & HPD::MaskPixelSubRow) >> HPD::ShiftPixelSubRow) :
                          0 );
    }

    /// Retrieve The pixel column number
    inline constexpr DataType pixelCol() const noexcept
    {
      return (DataType) ( HPDID == idType()                                        ?
                          ((key() & HPD::MaskPixelCol)   >> HPD::ShiftPixelCol)    :
                          ((key() & MaPMT::MaskPixelCol) >> MaPMT::ShiftPixelCol)  );
    }

    /// Retrieve The pixel row number
    inline constexpr DataType pixelRow() const noexcept
    {
      return (DataType) ( HPDID == idType()                                       ?
                          ((key() & HPD::MaskPixelRow)   >> HPD::ShiftPixelRow)   :
                          ((key() & MaPMT::MaskPixelRow) >> MaPMT::ShiftPixelRow) );
    }

    /// Retrieve The PD number in column
    inline constexpr DataType pdNumInCol() const noexcept
    {
      return (DataType) ( HPDID == idType()                                           ?
                          ((key() & HPD::MaskPDNumInCol)   >> HPD::ShiftPDNumInCol)   :
                          ((key() & MaPMT::MaskPDNumInCol) >> MaPMT::ShiftPDNumInCol) );
    }

    /// Retrieve The PD column number
    inline constexpr DataType pdCol() const noexcept
    {
      return (DataType) ( HPDID == idType()                                 ?
                          ((key() & HPD::MaskPDCol)   >> HPD::ShiftPDCol)   :
                          ((key() & MaPMT::MaskPDCol) >> MaPMT::ShiftPDCol) );
    }

    /// Retrieve The RICH panel
    inline constexpr Rich::Side panel() const noexcept
    {
      return (Rich::Side) ( HPDID == idType()                                 ?
                            ((key() & HPD::MaskPanel)   >> HPD::ShiftPanel)   :
                            ((key() & MaPMT::MaskPanel) >> MaPMT::ShiftPanel) );
    }

    /// Retrieve The RICH Detector
    inline constexpr Rich::DetectorType rich() const noexcept
    {
      return (Rich::DetectorType) ( HPDID == idType()                               ?
                                    ((key() & HPD::MaskRich)   >> HPD::ShiftRich)   :
                                    ((key() & MaPMT::MaskRich) >> MaPMT::ShiftRich) );
    }

  public:

    /// Retrieve Pixel sub-row field is set
    inline constexpr bool pixelSubRowIsSet() const noexcept
    {
      // Note MaPMTs have no sub-pixel ...
      return ( HPDID == idType()                                                          ?
               0 != ((key() & HPD::MaskPixelSubRowIsSet)   >> HPD::ShiftPixelSubRowIsSet) :
               false                                                                      );
    }

    /// Retrieve Pixel column field is set
    inline constexpr bool pixelColIsSet() const noexcept
    {
      return ( HPDID == idType()                                                      ?
               0 != ((key() & HPD::MaskPixelColIsSet)   >> HPD::ShiftPixelColIsSet)   :
               0 != ((key() & MaPMT::MaskPixelColIsSet) >> MaPMT::ShiftPixelColIsSet) );
    }

    /// Retrieve Pixel row field is set
    inline constexpr bool pixelRowIsSet() const noexcept
    {
      return ( HPDID == idType()                                                       ?
               0 != ((key() & HPD::MaskPixelRowIsSet)   >> HPD::ShiftPixelRowIsSet)    :
               0 != ((key() & MaPMT::MaskPixelRowIsSet) >> MaPMT::ShiftPixelRowIsSet)  );
    }

    /// Retrieve PD column field is set
    inline constexpr bool pdIsSet() const noexcept
    {
      return ( HPDID == idType()                                           ?
               0 != ((key() & HPD::MaskPDIsSet)   >> HPD::ShiftPDIsSet)    :
               0 != ((key() & MaPMT::MaskPDIsSet) >> MaPMT::ShiftPDIsSet)  );
    }

    /// Retrieve RICH panel field is set
    inline constexpr bool panelIsSet() const noexcept
    {
      return ( HPDID == idType()                                                 ?
               0 != ((key() & HPD::MaskPanelIsSet)   >> HPD::ShiftPanelIsSet)    :
               0 != ((key() & MaPMT::MaskPanelIsSet) >> MaPMT::ShiftPanelIsSet)  );
    }

    /// Retrieve RICH detector field is set
    inline constexpr bool richIsSet() const noexcept
    {
      return ( HPDID == idType()                                              ?
               0 != ((key() & HPD::MaskRichIsSet)   >> HPD::ShiftRichIsSet)   :
               0 != ((key() & MaPMT::MaskRichIsSet) >> MaPMT::ShiftRichIsSet) );
    }

  public:

    /// Returns true if the RichSmartID contains valid RICH detector data
    inline constexpr bool richDataAreValid() const noexcept
    {
      return richIsSet();
    }

    /// Returns true if the RichSmartID contains valid PD data
    inline constexpr bool pdDataAreValid() const noexcept
    {
      return ( pdIsSet() && panelIsSet() && richIsSet() );
    }

    /// Returns true if the RichSmartID contains valid pixel data
    inline constexpr bool pixelDataAreValid() const noexcept
    {
      return ( pixelColIsSet() && pixelRowIsSet() && pdDataAreValid() );
    }

    /// Returns true if the RichSmartID contains valid pixel sub-row (Alice mode) data
    inline constexpr bool pixelSubRowDataIsValid() const noexcept
    {
      return ( pixelSubRowIsSet() && pixelDataAreValid() );
    }

    /// Returns true if at least one data field has been set
    inline constexpr bool isValid() const noexcept
    {
      return ( richIsSet()     || panelIsSet()    || pdIsSet()         ||
               pixelRowIsSet() || pixelColIsSet() || pixelSubRowIsSet() );
    }

  public:

    /** Returns true if the SmartID is for a 'large' PMT.
     *  @attention Will always return false for HPDs... */
    inline constexpr bool isLargePMT() const noexcept
    {
      return ( HPDID == idType() ? false :
               0 != ((key() & MaPMT::MaskLargePixel) >> MaPMT::ShiftLargePixel ) );
    }

    /** Set the large PMT flag.
     *  @attention Does nothing for HPDs */
    inline void setLargePMT( const bool flag ) noexcept
    {
      if ( MaPMTID == idType() )
      {
        setData( flag, MaPMT::ShiftLargePixel, MaPMT::MaskLargePixel );
      }
    }

  public:

    //     // Implementation using a mutex for thread support
    // #ifndef GOD_NOALLOC

    //     /// operator new
    //     static void* operator new ( size_t size )
    //     {
    //       return ( sizeof(RichSmartID) == size ?
    //                boost::singleton_pool<RichSmartID, sizeof(RichSmartID)>::malloc() :
    //                ::operator new(size) );
    //     }

    //     /// placement operator new
    //     static void* operator new ( size_t size, void* pObj )
    //     {
    //       return ::operator new (size,pObj);
    //     }

    //     /// operator delete
    //     static void operator delete ( void* p )
    //     {
    //       boost::singleton_pool<RichSmartID, sizeof(RichSmartID)>::is_from(p) ?
    //         boost::singleton_pool<RichSmartID, sizeof(RichSmartID)>::free(p) :
    //         ::operator delete(p);
    //     }

    //     /// placement operator delete
    //     static void operator delete ( void* p, void* pObj )
    //     {
    //       ::operator delete (p, pObj);
    //     }

    // #endif

    // Implementation without thread support
#ifndef GOD_NOALLOC

    /// operator new
    static void* operator new ( size_t size )
    {
      using pool = boost::singleton_pool< RichSmartID, sizeof(RichSmartID),
        boost::default_user_allocator_new_delete,
        boost::details::pool::null_mutex, 128 >;
      return ( sizeof(RichSmartID) == size ? pool::malloc() : ::operator new(size) );
    }

    /// placement operator new
    static void* operator new ( size_t size, void* pObj )
    {
      return ::operator new ( size, pObj );
    }

    /// operator delete
    static void operator delete ( void* p )
    {
      using pool = boost::singleton_pool< RichSmartID, sizeof(RichSmartID),
        boost::default_user_allocator_new_delete,
        boost::details::pool::null_mutex, 128 >;
      pool::is_from(p) ? pool::free(p) : ::operator delete(p);
    }
    
    /// placement operator delete
    static void operator delete ( void* p, void* pObj )
    {
      ::operator delete ( p, pObj );
    }
    
#endif
    
  public:
    
    /// Print this RichSmartID in a human readable way
    std::ostream& fillStream( std::ostream& s,
#ifdef NDEBUG
                              const bool dumpSmartIDBits = false
#else
                              const bool dumpSmartIDBits = true
#endif
                              ) const;
    
    /** Return the output of the ostream printing of this object as a string.
     *  Mainly for use in GaudiPython. */
    std::string toString() const;
    
  public:
    
    /// Test if a given bit in the ID is on
    inline constexpr bool isBitOn( const int64_t pos ) const noexcept
    {
      return ( 0 != ( key() & ( 1ULL << pos ) ) );
    }
    
    /// Print the ID as a series of bits (0/1)
    std::ostream& dumpBits( std::ostream& s ) const;
    
  };
  
  /// ostream operator
  inline std::ostream& operator<< (std::ostream& str, const RichSmartID& obj)
  {
    return obj.fillStream(str);
  }
  
}

#endif
