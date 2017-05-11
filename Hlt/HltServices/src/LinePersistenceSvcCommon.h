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
  ILinePersistenceSvc::RawBanks rawBanksToPersistImpl(
    const LHCb::HltDecReports& hdr, const std::set<std::string>& lines,
    const RawBanksPerLine& rawBanksMap) const;
  RawBanksPerLine typeNamesToBitset(const NameListPerLine& rawBankTypesMap) const;
};

#endif // LinePersistenceSvcCommon_H