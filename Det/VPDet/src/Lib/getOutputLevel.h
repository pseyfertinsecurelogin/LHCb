#ifndef GET_OUTPUT_LEVEL_H
#define GET_OUTPUT_LEVEL_H

#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"

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
}

#endif // GET_OUTPUT_LEVEL_H
