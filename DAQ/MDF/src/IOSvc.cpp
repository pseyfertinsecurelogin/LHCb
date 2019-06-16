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

#include "MDF/IOSvc.h"
#include "MDF/Buffer.h"

#include "Event/RawEvent.h"

#include <thread>
#include <tuple>

StatusCode LHCb::MDF::IOSvc::finalize() {
  // join the thread that may be running in the bask, trying to prefetch more data
  m_nextBuffer.wait();
  return Service::finalize();
}

std::tuple<LHCb::RawEvent, std::shared_ptr<LHCb::MDF::Buffer>> LHCb::MDF::IOSvc::next() {
  // get hold of current buffer, by copying the shared_ptr
  auto buffer = m_curBuffer;
  // pick an event in it atomically
  auto event = buffer->get();
  if ( event.has_value() ) {
    return {std::move( event.value() ), buffer};
  } else {
    // No events remaining in current buffer, we need to renew the buffer
    while ( true ) {
      // new scope with serialized access so that only on thread deal with the renewal
      std::unique_lock guard( m_changeBufferLock );
      // We got the lock, but maybe the buffer was renewed while we waited. So double check
      // get hold of current buffer, by copying the shared_ptr
      buffer = m_curBuffer;
      // pick an event in it atomicall
      auto event = buffer->get();
      if ( event.has_value() ) {
        return {std::move( event.value() ), buffer};
      } else {
        // ok, buffers still need to be renewed, or needs it again. Anyway we are in charge now
        // let's just use the "ready to use" nextBuffer. Note that in case it's not yet
        // fully ready, the "get" call will be waiting for it to be ready
        m_curBuffer = m_nextBuffer.get();
        // check whether we reached the end of input, only continue if no
        if ( m_curBuffer->size() != 0 ) {
          // Now launch the preload of next buffer. Note that the lock will be released
          // before the actual preloading but after m_nextBuffer has been filled with a new future
          // This ensures that other threads will wait on that future if they exhaust the current
          // buffer before the preloading is over
          preloadNextBuffer( guard );
        }
      }
    } // guard is released here, if not already released by the preload
  }
}
