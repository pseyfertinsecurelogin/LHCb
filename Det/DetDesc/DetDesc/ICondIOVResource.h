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
  public:
    /// Default location of the IOVLock in the TES.
    static constexpr auto DefaultLocation = "IOVLock";

    /// Base class for the implementation specific lock manager.
    /// An implementation of ICondIOVResource will have to subclass LockManager and
    /// return an IOVLock wrapping a LockHandle.
    struct LockManager { virtual ~LockManager() = default; };
    using LockHandle = std::unique_ptr<LockManager>;

    IOVLock(LockHandle&& lock): m_resourceLock{std::move(lock)} {}

  private:
    LockHandle m_resourceLock;
  };

  /// Reserve the IOV valid for the given event time
  virtual IOVLock reserve( const Gaudi::Time& eventTime ) const = 0;
};

#endif // ICONDIOVRESOURCE_H
