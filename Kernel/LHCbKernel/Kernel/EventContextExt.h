/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once
#include "Kernel/EventLocalResource.h" // LHCb::Allocators::MemoryResource

#include "GaudiKernel/EventContext.h"

#include <any>
#include <cstddef>
#include <memory>

namespace LHCb {
  /** Store an Allocators::MemoryResource* in the given EventContext. Ownership is not transferred.
   */
  inline void setMemResource( EventContext& evtContext, Allocators::MemoryResource* memResource ) {
    // if std::size_t is too narrow then this should emit a compile-time warning...
    static_assert(
        EventContext::INVALID_CONTEXT_ID == std::numeric_limits<std::size_t>::max(),
        "LHCb::setMemResource() made use of the default subSlot being std::numeric_limits<std::size_t>::max()!" );
    evtContext.setSubSlot( EventContext::INVALID_CONTEXT_ID -
                           std::size_t{reinterpret_cast<std::uintptr_t>( memResource )} );
  }

  /** Retrieve an Allocators::MemoryResource* from the given EventContext. Ownership is not transferred.
   */
  [[nodiscard]] __attribute__( ( always_inline ) ) inline Allocators::MemoryResource*
  getMemResource( EventContext const& evtContext ) {
    // This is rather ugly, much nicer to have a dedicated slot in EventContext that is default-initialised to nullptr
    static_assert(
        EventContext::INVALID_CONTEXT_ID == std::numeric_limits<std::size_t>::max(),
        "LHCb::getMemResource() made use of the default subSlot being std::numeric_limits<std::size_t>::max()!" );
    return reinterpret_cast<Allocators::MemoryResource*>( EventContext::INVALID_CONTEXT_ID - evtContext.subSlot() );
  }

  struct EventContextExtension {
    template <typename ValueType, typename... Args>
    auto& emplaceSchedulerExtension( Args&&... args ) {
      return m_schedulerExtension.emplace<ValueType>( std::forward<Args>( args )... );
    }

    template <typename T>
    [[nodiscard]] auto& getSchedulerExtension() {
      return std::any_cast<std::decay_t<T>&>( m_schedulerExtension );
    }

    template <typename T>
    [[nodiscard]] const auto& getSchedulerExtension() const {
      return std::any_cast<std::decay_t<T> const&>( m_schedulerExtension );
    }

    /** Create a new memory resource that is owned by the extension.
     */
    template <typename ValueType, typename... Args>
    ValueType* emplaceMemResource( Args&&... args ) {
      m_memResource = std::make_shared<ValueType>( std::forward<Args>( args )... );
      return static_cast<ValueType*>( m_memResource.get() );
    }

    /** Transfer the memory resource out of this extension
     */
    std::shared_ptr<Allocators::MemoryResource> removeMemResource() { return std::move( m_memResource ); }

    /** Remove the memory resource owned by this event context extension.
     */
    void resetMemResource() { m_memResource.reset(); }

  private:
    /** Shared memory arena that is guaranteed to have a lifespan at least
     *  slightly longer than the event store.
     */
    std::shared_ptr<Allocators::MemoryResource> m_memResource;

    /** Extra extension reserved for the scheduler implementation
     *  TODO std::any inside std::any must cause the outer one to dynamically allocate, might we win by making the inner
     *  one unique_ptr<any> so the outer one fits in the static buffer?
     */
    std::any m_schedulerExtension;
  };
} // namespace LHCb
