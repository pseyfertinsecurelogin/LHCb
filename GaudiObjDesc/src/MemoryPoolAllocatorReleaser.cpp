#include "GaudiObjDesc/MemoryPoolAllocatorReleaser.h"

#include <utility>

#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"


Gaudi::MemoryPoolAllocatorReleaser& Gaudi::MemoryPoolAllocatorReleaser::instance() {
  static Gaudi::MemoryPoolAllocatorReleaser releaser;
  return releaser;
}

void Gaudi::MemoryPoolAllocatorReleaser::releaseMemory() {
  for(auto& entry: instance().m_releaseFunctions) entry.first();
}

void Gaudi::MemoryPoolAllocatorReleaser::releaseMemory(MsgStream &log) {
  ReleaseFuncCollType &funcMap = instance().m_releaseFunctions;
  // remember the level, so that we can print the heading in all lines
  const MSG::Level lvl = log.level();
  log << "Released " << funcMap.size() << " memory pools:" << endmsg;
  for(auto& entry: funcMap) {
    log << lvl << "  - " << entry.second
        << ((entry.first()) ? " (freed)" : "")
        << endmsg;
  }
}
