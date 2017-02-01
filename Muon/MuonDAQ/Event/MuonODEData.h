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

  std::array<unsigned int,35> m_data;

public:

  void resetODEData(){ m_data.fill(0); };

  void setData(unsigned int words[35]){ std::copy_n( words, 35, m_data.begin()); };

  unsigned int getWord(int index) const { return m_data[index]; };

  unsigned int getTDC(int index,int ch) const {
    int shift_rigth=32-(ch+1)*4;
    return ( m_data[index]<<shift_rigth)>>28;
  }

  unsigned int getTDC(int ch) const { return getTDC(ch/8+1,ch%8); }

};
#endif // MUONDAQ_MUONODEDATA_H
