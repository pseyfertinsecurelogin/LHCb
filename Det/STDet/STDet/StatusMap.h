// 
#ifndef _Status_H 
#define _Status_H 1

#include<map>
#include <string>
#include <vector>

#include "STDet/DeSTSector.h"

namespace Status{
 
  typedef std:: map<DeSTSector::Status,std::string> StatusToStringMap;
  typedef std::vector<DeSTSector::Status> StatusVector;

  /** conversion from enum to string */ 
  const StatusToStringMap& statusDescription();  

  /** allow state for beetles */ 
  const StatusVector& validBeetleStates();   

  /** protected states, should not be overwritten */
  const StatusVector& protectedStates();

}

/** stream operator for status */
std::ostream& operator << (std::ostream& s, DeSTSector::Status e);
#endif
