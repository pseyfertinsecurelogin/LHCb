// $Id: TypeMap.h,v 1.2 2007-03-01 12:08:55 cattanem Exp $ 
#ifndef _TsaTypeMap_H
#define _TsaTypeMap_H 1

/** @class Tsa::TypeMap TypeMap.h TsaKernel/TypeMap.h
* Map string identifying spacepoint to enum and vice-versa
* @author M. Needham
* @date 11/05/2004
*/

#include <map>
#include <string>
#include "TsaKernel/SpacePoint.h"

namespace Tsa{
class TypeMap{

public:

  /// map to enum
  Tsa::SpacePoint::pointType toType(std::string aName) const;

  /// map to type
  std::string toString(Tsa::SpacePoint::pointType aType) const;

  /// destructer
  virtual ~TypeMap() {}

  friend TypeMap& theMap();

private:

  /// constructer
  TypeMap();  
  TypeMap(const TypeMap& rhs){
    m_mapping = rhs.m_mapping;
  }

  typedef std::map<std::string, Tsa::SpacePoint::pointType> typeMap; 
  mutable typeMap m_mapping;

};

TypeMap& theMap(){
  static TypeMap sMap;
  return sMap;
}

};

inline Tsa::SpacePoint::pointType Tsa::TypeMap::toType(std::string aName) const{

  //  map to enum
  typeMap::iterator iter = m_mapping.find(aName);

  return (iter != m_mapping.end() ? iter->second : Tsa::SpacePoint::Unknown);
}

inline std::string Tsa::TypeMap::toString(Tsa::SpacePoint::pointType aType) const{
  // map to string
  typeMap::iterator iter = m_mapping.begin();
  while ((iter != m_mapping.end())&&(iter->second != aType)) {
    ++iter;
  } // iter 

  return (iter != m_mapping.end() ? iter->first : "unknown");
}

#endif
