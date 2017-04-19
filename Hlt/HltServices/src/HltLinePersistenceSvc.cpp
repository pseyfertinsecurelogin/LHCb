#include <string>
#include <vector>
#include "GaudiKernel/Service.h"
#include "GaudiKernel/Property.h"
#include "Kernel/ILinePersistenceSvc.h"


class HltLinePersistenceSvc : public extends1<Service, ILinePersistenceSvc> {
public:
  HltLinePersistenceSvc(const std::string& name, ISvcLocator* svcLocator);

  Locations locationsToPersist(const LHCb::HltDecReports& hdr) const override;
  RawBanks rawBanksToPersist(const LHCb::HltDecReports& hdr) const override;

private:
  using NameListPerLine = std::map<std::string, std::vector<std::string> >;
  using RawBanksPerLine = std::map<std::string, RawBanks>;

  Gaudi::Property<NameListPerLine> m_locations{this, "Locations", {},
    "Map of line decisison name to list of output locations."};
  Gaudi::PropertyWithReadHandler<NameListPerLine> m_rawBankTypes{
    this, "RawBankTypes", {},
    "Map of line decisison name to list of rawbank type names."};
  RawBanksPerLine m_rawBanks;

  RawBanksPerLine typeNamesToBitset(const NameListPerLine& map) const;

};

DECLARE_COMPONENT(HltLinePersistenceSvc)


HltLinePersistenceSvc::HltLinePersistenceSvc(const std::string& name, ISvcLocator* svcLocator)
  : base_class(name, svcLocator)
{
  m_rawBankTypes.declareUpdateHandler([=](Property&) {
    this->m_rawBanks = typeNamesToBitset(this->m_rawBankTypes);
  });
}


ILinePersistenceSvc::Locations
HltLinePersistenceSvc::locationsToPersist(const LHCb::HltDecReports& hdr) const {
  Locations locations;
  for (const auto& pair : hdr) {
    if (pair.second.decision()) {
      auto lineLocations = m_locations.find(pair.first);
      if (lineLocations == std::end(m_locations)) {
        throw GaudiException("Decision name not in Locations map.",
                             this->name(), StatusCode::FAILURE);
      }
      locations.push_back(lineLocations->second);
    }
  }
  return locations;
}


ILinePersistenceSvc::RawBanks
HltLinePersistenceSvc::rawBanksToPersist(const LHCb::HltDecReports& hdr) const {
  RawBanks rawBanks;
  for (const auto& pair : hdr) {
    if (pair.second.decision()) {
      auto lineRawBanks = m_rawBanks.find(pair.first);
      if (lineRawBanks == std::end(m_rawBanks)) {
        throw GaudiException("Decision name not in RawBankTypes map.",
                             this->name(), StatusCode::FAILURE);
      }
      rawBanks |= lineRawBanks->second;
    }
  }
  return rawBanks;
}


HltLinePersistenceSvc::RawBanksPerLine
HltLinePersistenceSvc::typeNamesToBitset(const NameListPerLine& map) const {
  RawBanksPerLine result;
  for (const auto& pair : map) {
    RawBanks rawBanks{};
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
