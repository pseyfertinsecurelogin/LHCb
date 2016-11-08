#ifndef MUONHLTDIGITFORMAT_H 
#define MUONHLTDIGITFORMAT_H 1

// Include files
#include "MuonHLTBase.h"

/** @class MuonHLTDigitFormat MuonHLTDigitFormat.h
 *  
 *
 *  @author Alessia Satta
 *  @date   2004-01-23
 */
class MuonHLTDigitFormat final {
public: 
  /// Standard constructor
  MuonHLTDigitFormat(unsigned int num ,unsigned int type) : m_data{num},m_type{type} {}
  MuonHLTDigitFormat(unsigned int type ) : MuonHLTDigitFormat(0,type) {}
  inline void setData(unsigned int num){m_data=num;};
  void setAddress(unsigned int num);
  void setTime(unsigned int num);
  unsigned int getTime() const;
  unsigned int getAddress() const;
  unsigned int getWord() const { return m_data; }
  
private:
  unsigned int m_data;
  unsigned int m_type;
  

};






#endif // MUONHLTDIGITFORMAT_H
