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

#include "Event/RawEvent.h"

#include "Kernel/STLExtensions.h"

#include <atomic>
#include <iostream>
#include <optional>
#include <vector>

namespace LHCb::MDF {

  /**
   * This class represents a single event in the event buffer
   * Upon creation, it only gets a raw buffer and a size.
   * Banks are then decoded from the buffer and added to the m_event when
   * calling get on the Buffer object
   */
  class MDFEvent {
  public:
    MDFEvent() = default;
    MDFEvent( LHCb::span<std::byte> data ) : m_data( data ) {}
    LHCb::RawEvent& event() { return m_event; }
    std::byte*      data() { return m_data.data(); }
    unsigned int    size() { return m_data.size(); }

  private:
    LHCb::RawEvent        m_event;
    LHCb::span<std::byte> m_data;
  };

  /**
   * Base class for an MDFEvent buffer. Handles a vector of MDFEvents
   * and gives thread safe access to them through the get method
   */
  class Buffer {
  public:
    /// constructor
    Buffer( std::vector<MDFEvent>&& events )
        : m_events( std::move( events ) ), m_nbAvailableEvents( m_events.size() ) {}
    /// virtual destructor
    virtual ~Buffer() = default;
    /// returns number of events in buffer
    unsigned int size() { return m_events.size(); }
    /**
     * get the next event in the Buffer. This method is thread safe
     * and garantees to return evry single event exactly once
     * @returns the event or nothing when the buffer is empty
     */
    std::optional<LHCb::RawEvent> get();

  private:
    /// vector of RawEvents, pointing to the Rawbanks in rawBuffer
    std::vector<MDFEvent> m_events;
    /// number of events still available in current buffer
    std::atomic<int> m_nbAvailableEvents;
  };

  // Helper template magic to constrain OwningBuffer to actually be an owning buffer
  namespace details {
    template <typename T>
    struct is_owner_ptr_t : std::false_type {};
    template <typename T, typename D>
    struct is_owner_ptr_t<std::unique_ptr<T, D>> : std::true_type {};
    template <typename T>
    struct is_owner_ptr_t<std::shared_ptr<T>> : std::true_type {};
    template <typename T>
    inline constexpr bool is_owner_ptr_v = is_owner_ptr_t<T>::value;
  } // namespace details

  /**
   * Templated implementation of a Buffer owning its underlying RawBuffer
   * Accepted RawBuffer types are std::unique_ptr<byte[]> or std::shared_ptr<some file wrapper>
   * The key point being that the RawBufferPtr will be detroyed when the Buffer object is
   * destroyed
   */
  template <typename RawBuffer, typename = std::enable_if_t<details::is_owner_ptr_v<RawBuffer>>>
  class OwningBuffer : public Buffer {
  public:
    OwningBuffer( RawBuffer&& rawBuffer, std::vector<MDFEvent>&& events )
        : Buffer( std::move( events ) ), m_rawBuffer( std::move( rawBuffer ) ) {}

  private:
    /// raw buffer containing rawbanks
    RawBuffer m_rawBuffer;
  };

} // namespace LHCb::MDF
