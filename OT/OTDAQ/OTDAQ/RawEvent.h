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
#ifndef OTDAQ_RAWEVENT_H
#define OTDAQ_RAWEVENT_H

#include <vector>
#include <numeric>
#include "OTDAQ/RawBank.h"

namespace OTDAQ
{

  class RawEvent
  {
  public:
    typedef std::vector<RawBank> RawBankContainer ;
    RawEvent() {}
    void clear() { m_banks.clear() ; }
    const RawBankContainer& banks() const { return m_banks ; }
    RawBankContainer& banks() { return m_banks ; }
    size_t numRawHits() const ;
    size_t numGols() const ;
  private:
    RawBankContainer m_banks ;
  } ;

  inline size_t RawEvent::numRawHits() const {
    return std::accumulate( m_banks.begin(), m_banks.end(), size_t{0},
                            [](size_t s0, const OTDAQ::RawBank& rb) {
      return std::accumulate( rb.gols().begin(), rb.gols().end(), s0,
                            [](size_t s1, const OTDAQ::Gol& gol)
                            { return s1 + gol.hits().size(); } );
    });
  }

  inline size_t RawEvent::numGols() const {
    return std::accumulate( m_banks.begin(), m_banks.end(), size_t{0},
                            [](size_t s, const OTDAQ::RawBank& rb)
                            { return s + rb.gols().size(); } );
  }
  
}

#endif
