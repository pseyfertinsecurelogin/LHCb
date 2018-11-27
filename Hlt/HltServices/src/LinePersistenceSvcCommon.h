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
#ifndef LinePersistenceSvcCommon_H
#define LinePersistenceSvcCommon_H 1

#include <string>
#include <vector>
#include "GaudiKernel/Service.h"
#include "GaudiKernel/INamedInterface.h"
#include "Kernel/ILinePersistenceSvc.h"


class LinePersistenceSvcCommon : public virtual INamedInterface {
protected:
  using NameListPerLine = std::map<std::string, std::vector<std::string> >;
  using RawBanksPerLine = std::map<std::string, ILinePersistenceSvc::RawBanks>;

  ILinePersistenceSvc::Locations locationsToPersistImpl(
    const LHCb::HltDecReports& hdr, const std::set<std::string>& lines,
    const NameListPerLine& locationsMap) const;
  ILinePersistenceSvc::Locations turboPPLocationsToPersistImpl(
    const LHCb::HltDecReports& hdr, const std::set<std::string>& lines,
    const std::set<std::string>& turboPPLines,
    const NameListPerLine& locationsMap) const;
  ILinePersistenceSvc::RawBanks rawBanksToPersistImpl(
    const LHCb::HltDecReports& hdr, const std::set<std::string>& lines,
    const RawBanksPerLine& rawBanksMap) const;
  RawBanksPerLine typeNamesToBitset(const NameListPerLine& rawBankTypesMap) const;
};

#endif // LinePersistenceSvcCommon_H