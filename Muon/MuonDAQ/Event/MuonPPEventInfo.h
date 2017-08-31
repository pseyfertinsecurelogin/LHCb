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

   std::array<unsigned int,8> m_data = { { 0 } };

public:

  void setWord(unsigned int i, unsigned int word) { m_data[i]=word; };

  unsigned int getBCN() const { return m_data[0]&0x0FFF; };
  unsigned int getDetID() const { return m_data[0]&0xF000; }
  unsigned int getL0EvtID() const { return m_data[1]; };

};
#endif // EVENT_MUONPPEVENTINFO_H
