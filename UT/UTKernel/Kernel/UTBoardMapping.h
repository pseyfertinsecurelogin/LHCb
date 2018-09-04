#ifndef UTBOARDMAPPING_H
#define UTBOARDMAPPING_H 1

/** @namespace UTBoardMapping UTBoardMapping.h Kernel/UTBoardMapping.h
 *
 *  Namespace for source ID to board number (and vice-versa) for IT and TT & UT
 *
 *  @author Johan Luisier
 * @author M Needham
 *  @date   2008-07-14
 */

#include <sstream>
#include <map>
#include <string>

namespace UTBoardMapping{

  typedef std::map<unsigned int, unsigned int> Map;

  const Map& UTSourceIDToNumberMap();
  const Map& UTNumberToSourceIDMap();

  enum endValue {notFound = 999};

  /** trivial helper to find entries in the map safely */
  unsigned int find(const unsigned int key, const Map& testMap);

  /** printout the UT mapping */
  std::string printUTMap();

  void ClearUTMap();
  void AddUTMapEntry(unsigned int sourceID, unsigned int number);

}

inline unsigned int UTBoardMapping::find(const unsigned int key, const UTBoardMapping::Map& testMap){
  auto iter = testMap.find(key);
  return (iter == testMap.end() ? (unsigned int)UTBoardMapping::notFound : iter->second);
}

#endif // UTBOARDMAPPING_H
