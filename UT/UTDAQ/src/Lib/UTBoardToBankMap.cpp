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

