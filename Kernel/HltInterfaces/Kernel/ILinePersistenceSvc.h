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
#ifndef ILinePersistenceSvc_H
#define ILinePersistenceSvc_H 1

// Include files
#include <string>
#include <set>
#include <bitset>
#include <functional>
#include "GaudiKernel/INamedInterface.h"
#include "Event/HltDecReports.h"
#include "Event/RawBank.h"


/** @class ILinePersistenceSvc ILinePersistenceSvc.h
 *
 *  Interface for services declaring what is to be persisted for each line.
 *
 *  There are two methods:
 *  - locationsToPersist() returns the union of locations that were requested
 *    by all lines from `lines` that fired in the HltDecReports object `hdr`.
 *  - turboPPLocationsToPersist() returns the union of locations that were requested
 *    by all Turbo++ lines from `lines` that fired in the HltDecReports object `hdr`.
 *  - rawBanksToPersist() returns the union of the requested raw banks for 
 *    all lines from `lines` that fired in the HltDecReports object `hdr`.
 *
 *  @author Rosen Matev
 *  @date   2017-04-19
 */


struct ILinePersistenceSvc : extend_interfaces<INamedInterface> {
  /// Return the interface ID
  DeclareInterfaceID(ILinePersistenceSvc, 1, 1);

  using LineDecNames = std::set<std::string>;
  using Locations = std::set<std::string>;
  using RawBanks = std::bitset<LHCb::RawBank::LastType>;

  virtual Locations locationsToPersist(
    const LHCb::HltDecReports& hdr, const LineDecNames& lines) const = 0;
  virtual Locations turboPPLocationsToPersist(
    const LHCb::HltDecReports& hdr, const LineDecNames& lines) const = 0;
  virtual RawBanks rawBanksToPersist(
    const LHCb::HltDecReports& hdr, const LineDecNames& lines) const = 0;
};
#endif // ILinePersistenceSvc_H
