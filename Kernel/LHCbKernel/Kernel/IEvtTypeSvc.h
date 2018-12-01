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
#ifndef KERNEL_IEVTTYPESVC_H
#define KERNEL_IEVTTYPESVC_H 1

// Include files
#include "GaudiKernel/IService.h"
#include <set>

//definition of how to store sets of events
namespace LHCb
{
  typedef std::greater<> EventTypeComp;
  typedef std::set<long unsigned int, EventTypeComp > EventTypeSet;
}


/** @class IEvtTypeSvc IEvtTypeSvc.h Kernel/IEvtTypeSvc.h
 *
 *  Interface to service that provide information for EVTTYPE code.
 *
 *  @author Gloria CORTI
 *  @date   2004-04-30
 *
 *  Version 2, R. Lambert  04/03/2009
 *  Added allTypes method, and defined the EventTypeComp and EventTypeSet
 *  to handle groups of Event type codes in an ordered, unique std::set
 *
 */
struct IEvtTypeSvc : extend_interfaces<IService> {

    DeclareInterfaceID(IEvtTypeSvc, 3 , 0);

  /// Given an event code number provide its Nick Name as in input table.
  /// If an event type does not exist in the table this method
  /// will return "Unknown"
  virtual std::string nickName( const int evtCode ) const = 0;

  /// Given an event code number provide the ASCII decay descriptor
  /// as in input table. Some event types have "No signal" as
  /// decay descriptor, if an event type does not exist in the
  /// table this method will return "Unknown"
  virtual std::string decayDescriptor( const int evtCode ) const = 0;

  /// Given an event code number check if it 'exist', i.e. is known by the
  /// service
  virtual bool typeExists( const int evtCode ) const = 0;

  /// Return all known event types in a unique and ordered std::set
  virtual LHCb::EventTypeSet allTypes( void ) const = 0;

};
#endif // KERNEL_IEVTTYPESVC_H
