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
#include "GaudiObjDesc/MemoryPoolAllocatorReleaser.h"

#include "GaudiKernel/MsgStream.h"

Gaudi::MemoryPoolAllocatorReleaser& Gaudi::MemoryPoolAllocatorReleaser::instance() {
  static Gaudi::MemoryPoolAllocatorReleaser releaser;
  return releaser;
}

void Gaudi::MemoryPoolAllocatorReleaser::releaseMemory() {
  for ( auto& entry : instance().m_releaseFunctions ) entry.first();
}

void Gaudi::MemoryPoolAllocatorReleaser::releaseMemory( MsgStream& log ) {
  ReleaseFuncCollType& funcMap = instance().m_releaseFunctions;
  // remember the level, so that we can print the heading in all lines
  const MSG::Level lvl = log.level();
  log << "Released " << funcMap.size() << " memory pools:" << endmsg;
  for ( auto& entry : funcMap ) {
    log << lvl << "  - " << entry.second << ( ( entry.first() ) ? " (freed)" : "" ) << endmsg;
  }
}
