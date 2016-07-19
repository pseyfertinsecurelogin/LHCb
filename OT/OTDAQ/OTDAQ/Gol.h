#ifndef OTDAQ_GOL_H
#define OTDAQ_GOL_H

#include "OTDAQ/RawHit.h"
#include "OTDAQ/GolHeader.h"
#include <vector>

namespace OTDAQ
{
  class Gol final {
  public:
    typedef std::vector<RawHit> RawHitContainer ;
    template <typename Iter1, typename Iter2>
    Gol( const GolHeader& header, Iter1&& begin, Iter2&& end ) : m_header(header), m_hits{ std::forward<Iter1>(begin), std::forward<Iter2>(end) } {}
    const GolHeader& header() const { return m_header ; }
    const RawHitContainer& hits() const { return m_hits ; }
  private:
    GolHeader m_header ;
    RawHitContainer m_hits ;
  };
}

#endif
