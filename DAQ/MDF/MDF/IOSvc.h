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
#pragma once

#include "MDF/Buffer.h"
#include "MDF/IIOSvc.h"

#include "Event/RawEvent.h"

#include "GaudiKernel/Service.h"

#include <future>
#include <memory>
#include <mutex>
#include <string>
#include <tuple>
#include <vector>

namespace LHCb::MDF {

  /**
   * Service allowing to read raw events from a set of MDF files,
   * implementing IIOSvc interface
   * The next() method is reentrant and allows to dispatch
   * events between multiple threads
   */
  class IOSvc : public extends<Service, IIOSvc> {

  public:
    using extends::extends;
    virtual ~IOSvc() = default;

    /// Service finalization
    StatusCode finalize() override;

    /**
     * get next event from input
     * This method is reentrant
     * @return a tuple RawEvent, shared_ptr<ByteBuffer> where the second one
     * owns the data pointed by the first one
     * @throws IIOSvc::EndOfInput
     */
    std::tuple<RawEvent, std::shared_ptr<Buffer>> next() override;

  private:
    /**
     * preloads data into the next buffer
     * Practically it first creates a task that will do the job,
     * then fills m_nextBuffer with the future of this task,
     * then releases the lock hold by the guard to release other
     * threads running on current buffer and finally does the job
     */
    virtual void preloadNextBuffer( std::unique_lock<std::mutex>& guard ) = 0;

  protected:
    Gaudi::Property<unsigned int> m_bufferNbEvents{
        this, "BufferNbEvents", 20000,
        "approximate size of the buffer used to prefetch rawbanks in terms of number of events. Default is 20000"};
    Gaudi::Property<std::vector<std::string>> m_input{this, "Input", {}, "List of inputs"};
    Gaudi::Property<unsigned int>             m_nbSkippedEvents{this, "NSkip", 0, "First event to process"};

    /// current Buffer to events
    std::shared_ptr<Buffer> m_curBuffer{nullptr};

    /// std::future holding the next buffer to use
    std::future<std::shared_ptr<Buffer>> m_nextBuffer;

    /// lock for handling the change of buffer
    std::mutex m_changeBufferLock;
  };
} // namespace LHCb::MDF
