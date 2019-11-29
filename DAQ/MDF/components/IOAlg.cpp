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

#include "Event/RawEvent.h"
#include "Gaudi/Algorithm.h"
#include "GaudiAlg/FixTESPath.h"
#include "GaudiAlg/Producer.h"
#include "MDF/Buffer.h"
#include "MDF/IIOSvc.h"

#include <memory>
#include <string>
#include <tuple>

namespace LHCb::MDF {

  class IOAlg final
      : public Gaudi::Functional::Producer<std::tuple<LHCb::RawEvent, std::shared_ptr<LHCb::MDF::Buffer>>(),
                                           Gaudi::Functional::Traits::BaseClass_t<FixTESPath<Gaudi::Algorithm>>> {

  public:
    IOAlg( const std::string& name, ISvcLocator* pSvcLocator )
        : Producer( name, pSvcLocator,
                    {KeyValue{"RawEventLocation", LHCb::RawEventLocation::Default},
                     KeyValue{"RawBanksBufferLocation", LHCb::RawEventLocation::Default + "Banks"}} ){};

    std::tuple<LHCb::RawEvent, std::shared_ptr<LHCb::MDF::Buffer>> operator()() const override { return iosvc->next(); }

  private:
    ServiceHandle<LHCb::IIOSvc> iosvc{this, "IOSvc", "LHCb::MDF::IOSvcFileRead", "Service to use to read input data"};
  };
} // namespace LHCb::MDF

DECLARE_COMPONENT( LHCb::MDF::IOAlg )
