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
#include "STBoardToBankMap.h"
#include <algorithm>

void STBoardToBankMap::addEntry( STTell1ID aBoard, unsigned int aBank ) {
  // add entry to map
  m_bankMapping[aBoard] = aBank;
}

STTell1ID STBoardToBankMap::findBoard( const unsigned int aBank ) const {
  // board to bank
  auto i = std::find_if( m_bankMapping.begin(), m_bankMapping.end(),
                         [&]( const std::pair<const STTell1ID, unsigned int>& p ) { return p.second == aBank; } );
  return i != m_bankMapping.end() ? i->first : STTell1ID( STTell1ID::nullBoard );
}
