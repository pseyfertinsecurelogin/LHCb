#ifndef KERNEL_IDQSCANNER_H
#define KERNEL_IDQSCANNER_H 1
// Include files
// from STL
#include <string>

// from LHCb
#include "Kernel/IDQFilter.h"

// Forward declaration
namespace Gaudi {
  class Time;
}

/** Interface for a tool extracting a summary of the Data Quality flags from the
 * conditions database in a given period of time.
 *
 * @author Marco Clemencic
 * @date 04/11/2011
 */
struct IDQScanner: extend_interfaces<IAlgTool> {
  /// InterfaceID
  DeclareInterfaceID(IDQScanner, 1, 0);

  /// Scan all the Data Quality flags in the give time range.
  /// @return merged list of DQ flags
  virtual IDQFilter::FlagsType scan(const Gaudi::Time& since, const Gaudi::Time& until) const = 0;

};

#endif // KERNEL_IDQSCANNER_H
