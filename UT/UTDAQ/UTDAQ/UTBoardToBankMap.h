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
#ifndef  _UTBoardToBankMap_H
#define _UTBoardToBankMap_H 1

/** @class UTBoardToBankMap UTBoardToBankMap.h
 *
 *  Helper class for mapping boards to banks
 *  basically hides a a map - used in 2 places....
 *
 *  @author A Beiter (based on code by M Needham)
 *  @date   2018-09-04
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
