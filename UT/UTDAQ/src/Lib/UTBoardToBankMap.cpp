/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "UTDAQ/UTBoardToBankMap.h"
#include <algorithm>

void UTBoardToBankMap::addEntry(UTTell1ID aBoard, unsigned int aBank){
  // add entry to map
 m_bankMapping[aBoard] =aBank;
}

UTTell1ID UTBoardToBankMap::findBoard(const unsigned int aBank) const {
  // board to bank
  auto i = std::find_if( m_bankMapping.begin(), m_bankMapping.end(),
                         [&](const std::pair<const UTTell1ID, unsigned int>& p ) {
                             return p.second == aBank;
  });
  return i != m_bankMapping.end() ? i->first : UTTell1ID(UTTell1ID::nullBoard);
}

