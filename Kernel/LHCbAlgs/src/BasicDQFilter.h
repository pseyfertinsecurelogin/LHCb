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
#ifndef SRC_BASICDQFILTER_H
#define SRC_BASICDQFILTER_H 1
// Include files
// from STL
#include <string>
#include <vector>
#include <set>

// from Gaudi
#include "GaudiAlg/GaudiTool.h"

// Implemented interfaces
#include "Kernel/IDQFilter.h" // IDQFilter

/** Basic implementation of a IDQFilter.
 *
 * Accept the list of flags only if none is set, except those specified in the
 * IgnoredFlags property.
 *
 * @author Marco Clemencic
 * @date 04/11/2011
 */
class BasicDQFilter final : public extends1<GaudiTool, IDQFilter>
{

public:

  /// Standard constructor
  BasicDQFilter(const std::string& type, const std::string& name, const IInterface* parent);

  virtual ~BasicDQFilter() = default; ///< Destructor

  /// Accept the flags if there is not flag set, except the ignored ones
  /// (property IgnoredFlags).
  bool accept(const FlagsType& flags) const override;

private:

  /// List of flag (names) to ignore during the filtering. The matching is
  /// case-insensitive. (property IgnoredFlags)
  std::vector<std::string> m_ignoredFlagsProp;

  /// Internal storage for ignored flags.
  std::set<std::string> m_ignoredFlags;

 private:

  /// Call-back function to update the internal storage of ignored flags.
  void i_propUpdate(Property&);

};

#endif // SRC_BASICDQFILTER_H
