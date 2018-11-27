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
#ifndef STBOARDMAPPING_H
#define STBOARDMAPPING_H 1

/** @namespace STBoardMapping STBoardMapping.h Kernel/STBoardMapping.h
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

namespace STBoardMapping{

  typedef std::map<unsigned int, unsigned int> Map;

  const Map& TTSourceIDToNumberMap();
  const Map& TTNumberToSourceIDMap();
  const Map& ITSourceIDToNumberMap();
  const Map& ITNumberToSourceIDMap();
  const Map& UTSourceIDToNumberMap();
  const Map& UTNumberToSourceIDMap();

  enum endValue {notFound = 999};

  /** trivial helper to find entries in the map safely */
  unsigned int find(const unsigned int key, const Map& testMap);

  /** printout the TT mapping */
  std::string printTTMap();

  /** printout the IT mapping */
  std::string printITMap();

  /** printout the UT mapping */
  std::string printUTMap();

  void ClearUTMap();
  void AddUTMapEntry(unsigned int sourceID, unsigned int number);

}

inline unsigned int STBoardMapping::find(const unsigned int key, const STBoardMapping::Map& testMap){
  auto iter = testMap.find(key);
  return (iter == testMap.end() ? (unsigned int)STBoardMapping::notFound : iter->second);
}

#endif // STBOARDMAPPING_H
