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
#include <string>
#include <vector>
#include "GaudiKernel/Service.h"
#include "GaudiKernel/Property.h"
#include "Kernel/ILinePersistenceSvc.h"
#include "LinePersistenceSvcCommon.h"


class HltLinePersistenceSvc : public extends<Service, ILinePersistenceSvc>,
                              private LinePersistenceSvcCommon {
public:
  HltLinePersistenceSvc(const std::string& name, ISvcLocator* svcLocator);

  Locations locationsToPersist(const LHCb::HltDecReports& hdr,
    const LineDecNames& lines) const override;
  RawBanks rawBanksToPersist(const LHCb::HltDecReports& hdr,
    const LineDecNames& lines) const override;
  Locations turboPPLocationsToPersist(const LHCb::HltDecReports& hdr,
    const LineDecNames& lines) const override;

private:
  Gaudi::Property<NameListPerLine> m_locations{this, "Locations", {},
    "Map of line decisison name to list of output locations."};
  Gaudi::Property<std::vector<std::string>> m_turboPPLines{this, "TurboPPLines", {},
    "List of Turbo++ line decision names."};
  Gaudi::PropertyWithReadHandler<NameListPerLine> m_rawBankTypes{
    this, "RawBankTypes", {},
    "Map of line decisison name to list of rawbank type names."};
  LineDecNames m_turboPPLinesSet;
  RawBanksPerLine m_rawBanks;
};

DECLARE_COMPONENT(HltLinePersistenceSvc)


HltLinePersistenceSvc::HltLinePersistenceSvc(const std::string& name, ISvcLocator* svcLocator)
  : base_class(name, svcLocator)
{
  m_turboPPLines.declareUpdateHandler([this](Property&) {
      this->m_turboPPLinesSet = ILinePersistenceSvc::LineDecNames{
        this->m_turboPPLines.begin(), this->m_turboPPLines.end()};
  });
  m_rawBankTypes.declareUpdateHandler([=](Property&) {
    this->m_rawBanks = typeNamesToBitset(this->m_rawBankTypes);
  });
}


ILinePersistenceSvc::Locations
HltLinePersistenceSvc::locationsToPersist(
  const LHCb::HltDecReports& hdr,
  const ILinePersistenceSvc::LineDecNames& lines) const
{
  return locationsToPersistImpl(hdr, lines, m_locations);
}


ILinePersistenceSvc::Locations
HltLinePersistenceSvc::turboPPLocationsToPersist(
  const LHCb::HltDecReports& hdr,
  const ILinePersistenceSvc::LineDecNames& lines) const
{
  return turboPPLocationsToPersistImpl(hdr, lines, m_turboPPLinesSet, m_locations);
}

ILinePersistenceSvc::RawBanks
HltLinePersistenceSvc::rawBanksToPersist(
  const LHCb::HltDecReports& hdr,
  const ILinePersistenceSvc::LineDecNames& lines) const
{
  return rawBanksToPersistImpl(hdr, lines, m_rawBanks);
}
