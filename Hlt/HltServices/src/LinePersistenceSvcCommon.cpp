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
#include "LinePersistenceSvcCommon.h"

ILinePersistenceSvc::Locations
LinePersistenceSvcCommon::locationsToPersistImpl(
  const LHCb::HltDecReports& hdr, const std::set<std::string>& lines,
  const NameListPerLine& locationsMap) const
{
  ILinePersistenceSvc::Locations locations;
  for (const auto& pair : hdr) {
    if (pair.second.decision() && lines.count(pair.first)) {
      auto lineLocations = locationsMap.find(pair.first);
      if (lineLocations == std::end(locationsMap)) {
        throw GaudiException("Decision name not in Locations map.",
                             this->name(), StatusCode::FAILURE);
      }
      locations.insert(std::begin(lineLocations->second),
                       std::end(lineLocations->second));
    }
  }
  return locations;
}


ILinePersistenceSvc::Locations
LinePersistenceSvcCommon::turboPPLocationsToPersistImpl(
  const LHCb::HltDecReports& hdr, const std::set<std::string>& lines,
  const std::set<std::string>& turboPPLines,
  const NameListPerLine& locationsMap) const
{
  ILinePersistenceSvc::Locations locations;
  for (const auto& pair : hdr) {
    if (pair.second.decision() && lines.count(pair.first) &&
        turboPPLines.count(pair.first)) {
      auto lineLocations = locationsMap.find(pair.first);
      if (lineLocations == std::end(locationsMap)) {
        throw GaudiException("Decision name not in Locations map.",
                             this->name(), StatusCode::FAILURE);
      }
      locations.insert(std::begin(lineLocations->second),
                       std::end(lineLocations->second));
    }
  }
  return locations;
}

ILinePersistenceSvc::RawBanks
LinePersistenceSvcCommon::rawBanksToPersistImpl(
  const LHCb::HltDecReports& hdr, const std::set<std::string>& lines,
  const RawBanksPerLine& rawBanksMap) const
{
  ILinePersistenceSvc::RawBanks rawBanks;
  for (const auto& pair : hdr) {
    if (pair.second.decision() && lines.count(pair.first)) {
      auto lineRawBanks = rawBanksMap.find(pair.first);
      if (lineRawBanks == std::end(rawBanksMap)) {
        throw GaudiException("Decision name not in RawBankTypes map.",
                             this->name(), StatusCode::FAILURE);
      }
      rawBanks |= lineRawBanks->second;
    }
  }
  return rawBanks;
}


LinePersistenceSvcCommon::RawBanksPerLine
LinePersistenceSvcCommon::typeNamesToBitset(
  const NameListPerLine& rawBankTypesMap) const
{
  RawBanksPerLine result;
  for (const auto& pair : rawBankTypesMap) {
    ILinePersistenceSvc::RawBanks rawBanks{};
    for (const auto& typeName : pair.second) {
      int bankid = -1;
      for (auto i = 0; i < LHCb::RawBank::LastType; ++i) {
        if (typeName == LHCb::RawBank::typeName(static_cast<LHCb::RawBank::BankType>(i))) {
          bankid = i;
          break;
        }
      }
      if (bankid < 0) {
        throw GaudiException("Rawbank type name does not exist.",
                             this->name(), StatusCode::FAILURE);
      }
      rawBanks.set(bankid);
    }
    result[pair.first] = rawBanks;
  }
  return result;
}
