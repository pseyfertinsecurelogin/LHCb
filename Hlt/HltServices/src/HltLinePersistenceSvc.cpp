#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "Gaudi/Details/PluginServiceDetails.h"
#include "Gaudi/PluginService.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/PropertyFwd.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/extends.h"
#include "Kernel/ILinePersistenceSvc.h"
#include "LinePersistenceSvcCommon.h"

class ISvcLocator;
namespace LHCb {
class HltDecReports;
}  // namespace LHCb


class HltLinePersistenceSvc : public extends<Service, ILinePersistenceSvc>,
                              private LinePersistenceSvcCommon {
public:
  HltLinePersistenceSvc(const std::string& name, ISvcLocator* svcLocator);

  Locations locationsToPersist(const LHCb::HltDecReports& hdr,
    const std::set<std::string>& lines) const override;
  RawBanks rawBanksToPersist(const LHCb::HltDecReports& hdr,
    const std::set<std::string>& lines) const override;

private:
  Gaudi::Property<NameListPerLine> m_locations{this, "Locations", {},
    "Map of line decisison name to list of output locations."};
  Gaudi::PropertyWithReadHandler<NameListPerLine> m_rawBankTypes{
    this, "RawBankTypes", {},
    "Map of line decisison name to list of rawbank type names."};
  RawBanksPerLine m_rawBanks;
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
HltLinePersistenceSvc::locationsToPersist(
  const LHCb::HltDecReports& hdr, const std::set<std::string>& lines) const
{
  return locationsToPersistImpl(hdr, lines, m_locations);
}


ILinePersistenceSvc::RawBanks
HltLinePersistenceSvc::rawBanksToPersist(
  const LHCb::HltDecReports& hdr, const std::set<std::string>& lines) const
{
  return rawBanksToPersistImpl(hdr, lines, m_rawBanks);
}
