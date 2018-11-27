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
