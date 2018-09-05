#ifndef _UTRawBankMap_H_
#define _UTRawBankMap_H_

/** @namespace UTRawBankMap UTRawBankMap.h SKernel/UTRawBank
 *
 *  Map string to corresponding bank type enum
 *  @author A. Beiter (based on code by M.Needham)
 *  @date   2018-09-04
 */

#include "Event/RawBank.h"

namespace UTRawBankMap{

inline LHCb::RawBank::BankType stringToType(const std::string& type){

  // string to bank type
  LHCb::RawBank::BankType bankType = LHCb::RawBank::Velo;

  if (type == "UT"){
    bankType = LHCb::RawBank::UT;
  } 
  else if (type == "UTFull"){
    bankType = LHCb::RawBank::UTFull;
  }
  else if (type == "UTError" ){
    bankType = LHCb::RawBank::UTError;
  }
  else if (type == "UTPedestal" ){
    bankType = LHCb::RawBank::UTPedestal;
  }
  else {
    // use the Velo as NULL
  }
  return  bankType;
}




}

#endif // _UTRawBankMap_H_
