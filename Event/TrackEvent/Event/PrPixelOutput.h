#pragma once
#include "Event/State.h"
#include "Event/Track_v2.h" // nfc
#include <array>
#include <vector>

namespace foo::details {
template <std::size_t nbits, std::size_t offset, typename T>
constexpr T makeMask() {
  static_assert(std::numeric_limits<T>::is_integer);
  static_assert(!std::numeric_limits<T>::is_signed);
  static_assert(std::numeric_limits<T>::radix == 2);

  constexpr auto maxbits = std::numeric_limits<T>::digits;

  static_assert(nbits <= maxbits);
  static_assert(offset <= maxbits - nbits);

  return (~T{0} >> (maxbits - nbits)) << offset;
}
template <std::size_t n_bits, std::size_t offset, typename UInt = std::uint32_t>
constexpr auto mask_v = details::makeMask<n_bits, offset, UInt>();

constexpr int trailing_zeros(uint32_t v) {
  // from http://graphics.stanford.edu/~seander/bithacks.html
  constexpr auto tbl = std::array<int, 32>{
      0,  1,  28, 2,  29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4,  8,
      31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6,  11, 5,  10, 9};
  return tbl[(static_cast<uint32_t>((v & -v) * 0x077CB531U)) >> 27];
}
}

namespace foo {
struct PrPixelOutputForVertexing;
using PrPixelOutputForVertexings = std::vector<PrPixelOutputForVertexing>;
struct PrPixelOutputForTracking;
using PrPixelOutputForTrackings = std::vector<PrPixelOutputForTracking>;

struct PrPixelOutputForVertexing {
  // visibility redundant for the compiler, potentially nice for the reader,
  // depends on the reader.
public:
  LHCb::State state;
};

struct PrPixelOutputForTracking {
  // visibility redundant for the compiler, potentially nice for the reader,
  // depends on the reader.
public:
  /// Track general flags enumerations
  enum class Flags {
    FlagsUnknown = 0, //
    Backward = 1,     // VELO track in the backward direction
    Invalid = 2,      // invalid track for physics
    Clone = 4,        // clone track (of a corresponding unique track)
    Used = 8,         //
    IPSelected = 16,  //
    PIDSelected = 32, //
    Selected = 64,    //
    L0Candidate = 128 //
  };

  operator LHCb::Event::v2::Track() const;
  LHCb::State state;
  LHCb::Event::v2::Track::Type type;
  LHCb::Event::v2::Track::History history;
  LHCb::Event::v2::Track::PatRecStatus patrecstatus;
  std::vector<LHCb::LHCbID> lhcbIDs;
  float chi2perdof;
  int dofs;

  unsigned int m_flags{0}; ///< The variety of track flags

  void setFlag(Flags const flag, bool const ok) {
    uint32_t val = ((static_cast<uint32_t>(flag))
                    << details::trailing_zeros(flagsMasks::flagMask)) &
                   flagsMasks::flagMask;
    if (ok)
      m_flags |= val;
    else
      m_flags &= ~val;
  }
  bool checkFlag( Flags const flag ) const
  {
    uint32_t val = ( static_cast<uint32_t>( flag ) << details::trailing_zeros( flagsMasks::flagMask ) );
    return ( 0 != ( m_flags & flagsMasks::flagMask & val ) );
  }


  /// Make sure that the offset is the sum of the previous entries
  enum flagsMasks : uint32_t {
    typeMask = details::mask_v<4, 0>,
    historyMask = details::mask_v<7, 4>,
    fitHistoryMask = details::mask_v<3, 11>,
    patRecStatusMask = details::mask_v<2, 14>,
    fitStatusMask = details::mask_v<2, 16>,
    flagMask = details::mask_v<10, 18>,
    specificMask = details::mask_v<4, 28>
  };
  static_assert(flagsMasks::typeMask == 0xfL);
  static_assert(flagsMasks::historyMask == 0x7f0L);
  static_assert(flagsMasks::fitHistoryMask == 0x3800L);
  static_assert(flagsMasks::patRecStatusMask == 0xc000L);
  static_assert(flagsMasks::fitStatusMask == 0x30000L);
  static_assert(flagsMasks::flagMask == 0xffc0000L);
  static_assert(flagsMasks::specificMask == 0xf0000000L);
};
}
