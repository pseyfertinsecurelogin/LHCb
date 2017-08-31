#ifndef MUONDAQ_MUONODEDATA_H
#define MUONDAQ_MUONODEDATA_H 1

#include <array>
#include <algorithm>

/** @class MuonODEData MuonODEData.h Event/MuonODEData.h
 *
 *
 *  @author Alessia Satta
 *  @date   2007-12-05
 */


class MuonODEData final {

  std::array<unsigned int,35> m_data = { { 0 } };

public:

  void resetODEData(){ *this = {}; };

  void setData(const std::array<unsigned int,35>& d){ m_data = d; }

  unsigned int getWord(int index) const { return m_data[index]; };

  unsigned int getTDC(int index,int ch) const {
    int shift_rigth=32-(ch+1)*4;
    return ( m_data[index]<<shift_rigth)>>28;
  }

  unsigned int getTDC(int ch) const { return getTDC(ch/8+1,ch%8); }

};
#endif // MUONDAQ_MUONODEDATA_H
