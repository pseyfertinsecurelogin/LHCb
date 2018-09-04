#ifndef  _UTBoardToBankMap_H
#define _UTBoardToBankMap_H 1

/** @class UTBoardToBankMap UTBoardToBankMap.h
 *
 *  Helper class for mapping boards to banks
 *  basically hides a a map - used in 2 places....
 *
 *  @author M Needham
 *  @date   2004-08-01
 */

#include <map>

#include "Kernel/UTTell1ID.h"

class UTBoardToBankMap final {

public:

 void addEntry(UTTell1ID aBoard, unsigned int aBank);
 UTTell1ID findBoard(const unsigned int aBank) const;

 // bank to board
 unsigned int findBank(const UTTell1ID aBoard) const
 { return m_bankMapping.at(aBoard); }

 void clear() { m_bankMapping.clear(); }

private:

 std::map<UTTell1ID,unsigned int> m_bankMapping;

};

#endif // _UTBoardToBankMap_H
