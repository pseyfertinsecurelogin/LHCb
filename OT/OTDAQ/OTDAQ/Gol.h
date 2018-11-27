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
