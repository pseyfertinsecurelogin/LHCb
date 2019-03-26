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
#ifndef EVENT_MUONPPEVENTINFO_H
#define EVENT_MUONPPEVENTINFO_H 1
#include <array>

// Include files

/** @class MuonPPEventInfo MuonPPEventInfo.h Event/MuonPPEventInfo.h
 *
 *
 *  @author
 *  @date   2008-01-07
 */
class MuonPPEventInfo final {

  std::array<unsigned int, 8> m_data = {{0}};

public:
  void setWord( unsigned int i, unsigned int word ) { m_data[i] = word; };

  unsigned int getBCN() const { return m_data[0] & 0x0FFF; };
  unsigned int getDetID() const { return m_data[0] & 0xF000; }
  unsigned int getL0EvtID() const { return m_data[1]; };
};
#endif // EVENT_MUONPPEVENTINFO_H
