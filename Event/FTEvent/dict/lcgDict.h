#ifndef FTEVENT_LCGDICT_H 
#define FTEVENT_LCGDICT_H 1

// Additional classes to be added to automatically generated lcgdict

// Include files
#include "Kernel/MultiIndexedContainer.h"
#include "Event/FTLiteCluster.h"
// end include files

/** @class lcgDict lcgDict.h dict/lcgDict.h
 *  
 *
 *  @author Eric Cogneras
 *  @date   2013-05-06
 */
namespace {
  struct _Instantiations {
    // begin instantiations
    LHCb::Container::MultiIndexedContainer<LHCb::FTLiteCluster, 48> _;
    // end instantiations
  };
}

#endif // FTEVENT_LCGDICT_H
