#ifndef ILinePersistenceSvc_H
#define ILinePersistenceSvc_H 1

// Include files
#include <string>
#include <vector>
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
 *  - locationsToPersist() returns the locations that were requested by all
 *    lines that fired in the given HltDecReports object. The returned object
 *    is a list of references to lists of locations.
 *    for all line that fired in the given HltDecReports object.
 *  - rawBanksToPersist() returns the union of the requested raw banks for 
 *    all line that fired in the given HltDecReports object. The returned object
 *    is an std::bitset.
 *
 *  @author Rosen Matev
 *  @date   2017-04-19
 */


struct ILinePersistenceSvc : extend_interfaces<INamedInterface> {
  /// Return the interface ID
  DeclareInterfaceID(ILinePersistenceSvc, 1, 0);

  using Locations = std::vector<std::reference_wrapper<const std::vector<std::string> > >;
  using RawBanks = std::bitset<LHCb::RawBank::LastType>;

  virtual Locations locationsToPersist(const LHCb::HltDecReports& hdr) const = 0;
  virtual RawBanks rawBanksToPersist(const LHCb::HltDecReports& hdr) const = 0;
};
#endif // ILinePersistenceSvc_H
