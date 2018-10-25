/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef KERNEL_IDQFILTER_H
#define KERNEL_IDQFILTER_H 1
// Include files
// from STL
#include <string>
#include <map>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

/** Interface for a tool implementing the acceptance rules for Data Quality
 *  flags.
 *
 * @author Marco Clemencic
 * @date 04/11/2011
 */
struct IDQFilter: extend_interfaces<IAlgTool> {

  /// InterfaceID
  DeclareInterfaceID(IDQFilter, 1, 0);

  /// Type used for the DQ flags.
  typedef std::map<std::string, int> FlagsType;

  /// Check if a set of flags is acceptable according to the rules defined
  /// by the implementation.
  virtual bool accept(const FlagsType& flags) const = 0;

};

#endif // KERNEL_IDQFILTER_H
