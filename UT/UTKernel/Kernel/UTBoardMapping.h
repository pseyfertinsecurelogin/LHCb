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
#ifndef UTBOARDMAPPING_H
#define UTBOARDMAPPING_H 1

/** @namespace UTBoardMapping UTBoardMapping.h Kernel/UTBoardMapping.h
 *
 *  Namespace for source ID to board number (and vice-versa) for IT and TT & UT
 *
 *  @author Andy Beiter based on code by:
 *  @author Johan Luisier
 *  @author M Needham
 *  @date   2018-09-04
 */

#include <map>
#include <sstream>
#include <string>

namespace UTBoardMapping {

  typedef std::map<unsigned int, unsigned int> Map;

  const Map& UTSourceIDToNumberMap();
  const Map& UTNumberToSourceIDMap();

  enum endValue { notFound = 999 };

  /** trivial helper to find entries in the map safely */
  unsigned int find( const unsigned int key, const Map& testMap );

  /** printout the UT mapping */
  std::string printUTMap();

  void ClearUTMap();
  void AddUTMapEntry( unsigned int sourceID, unsigned int number );

} // namespace UTBoardMapping

inline unsigned int UTBoardMapping::find( const unsigned int key, const UTBoardMapping::Map& testMap ) {
  auto iter = testMap.find( key );
  return ( iter == testMap.end() ? (unsigned int)UTBoardMapping::notFound : iter->second );
}

#endif // UTBOARDMAPPING_H
