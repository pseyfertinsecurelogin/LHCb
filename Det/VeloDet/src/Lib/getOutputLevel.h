#ifndef GET_OUTPUT_LEVEL_H
#define GET_OUTPUT_LEVEL_H

#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/Property.h"

#include <string>
#include <exception>

namespace {
  /** Helper to get from JobOptionsSvc the OutputLevel property
   *  of something not implementing IProperty, by client name.
   */
  int getOutputLevel(const std::string& name) {
    IntegerProperty outputLevel{"OutputLevel", 0};
    auto jobSvc = Gaudi::svcLocator()->service<IJobOptionsSvc>("JobOptionsSvc");
    if (!jobSvc) throw std::runtime_error("cannot get JobOptionsSvc");
    auto props = jobSvc->getProperties(name);
    if (props) {
      for(auto p : *props) {
        if (p->name() == outputLevel.name()) {
          if (!outputLevel.assign(*p)) throw std::invalid_argument("invalid output level: " + p->toString());
          break;
        }
      }
    }
    return outputLevel;
  }
  StatusCode initOutputLevel(IMessageSvc* msgSvc, const std::string& name) {
    StatusCode sc = StatusCode::SUCCESS;
    try { // Trick to set the output level
      if (!msgSvc) throw std::invalid_argument("invalid IMessageSvc (nullptr)");
      const auto lvl = getOutputLevel(name);
      if (lvl > 0)
        msgSvc->setOutputLevel(name, lvl);
    } catch (const std::exception& x) {
      std::cerr << x.what() << std::endl;
      sc = StatusCode::FAILURE;
    }
    return sc;
  }
}

#endif // GET_OUTPUT_LEVEL_H
