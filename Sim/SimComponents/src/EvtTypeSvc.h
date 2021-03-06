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
#ifndef EVTTYPESVC_H
#define EVTTYPESVC_H 1

// Include files
#include "GaudiKernel/Service.h"
// from Gaudi
#include "EvtTypeInfo.h"
#include "Kernel/IEvtTypeSvc.h"

#include <cstdlib>
#include <iostream>
#include <string>

#include <set>
#include <vector>

/** @class EvtTypeSvc EvtTypeSvc.h
 *
 *  Service that provide information for (MC)EVTTYPE code.
 *
 *  @author Gloria CORTI
 *  @date   2004-04-30
 *
 *  Edited R. Lambert  2009-03-04
 *  Added implimentation of allTypes(void), returning a std::set of all known types
 *
 */
class EvtTypeSvc : public extends<Service, IEvtTypeSvc> {
public:
  /// Initialize the service.
  StatusCode initialize() override;

  /// Finalize the service.
  StatusCode finalize() override;

  /** Given the integer code of an Event type provide its Nick Name
   *  if it does not exist returns an empty string
   *  @see IEvtTypeSvc
   */
  std::string nickName( const int evtCode ) const override;

  /** Given an EventType provide its ASCII decay descriptor
   *  @see IEvtTypeSvc
   */
  std::string decayDescriptor( const int evtCode ) const override;

  /** Check if an event type corresponding to the integer code is known
   *  @see IEvtTypeSvc
   */
  bool typeExists( const int evtCode ) const override;

  /** return a set of all known event types
   *  @see IEvtTypeSvc
   */
  LHCb::EventTypeSet allTypes() const override;

  /** Standard Constructor.
   *  @param  name   String with service name
   *  @param  svc    Pointer to service locator interface
   */
  EvtTypeSvc( const std::string& name, ISvcLocator* svc );

private:
  /// Parse the input table containing all known event types and
  /// theirs' nicknames and ascii descriptor
  StatusCode parseFile( const std::string input );

  /// Name of file with input table
  const std::string& inputFile() const { return m_inputFile; }

  /// Typedefs

  // Data
  std::string              m_inputFile;    ///< Name of input file with necessary info
  std::vector<EvtTypeInfo> m_evtTypeInfos; ///< List of objects containing all EvtType info
};
#endif // EVTTYPESVC_H
