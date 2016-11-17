#ifndef ICONDIOVRESOURCE_H
#define ICONDIOVRESOURCE_H

#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/IInterface.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StringKey.h"
#include "GaudiKernel/Time.h"

#include <shared_mutex>

/**
 * @brief Interface to allow reservation/lock of IOVs.
 *
 * This interface should be implemented by an IOV service that allows pinning of the IOV for the
 * event time passed to the method @c reserve().  The lifetime of the reservation is that of the
 * returned IOVLock.
 */
class ICondIOVResource : virtual public IInterface {
public:
  DeclareInterfaceID(ICondIOVResource, 1, 0);

  /**
   * The reserved IOV (via ICondIOVResource::reserve) will stay reserved for the lifetime
   * of the returned instance of this class.
   */
  class IOVLock : public DataObject {
    using read_lock = std::shared_lock<std::shared_timed_mutex>;
  public:
    static constexpr auto DefaultLocation = "IOVLock";
    IOVLock(read_lock&& lock): m_resourceLock{std::move(lock)} {}
  private:
    read_lock m_resourceLock;
  };

  /// Reserve the IOV valid for the given event time
  virtual IOVLock reserve( const Gaudi::Time& eventTime ) const = 0;
};

#endif // ICONDIOVRESOURCE_H
