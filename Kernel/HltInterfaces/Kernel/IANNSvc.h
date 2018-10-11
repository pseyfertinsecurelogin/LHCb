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
#ifndef IANNSVC_H
#define IANNSVC_H 1

// Include files
#include <string>
#include <vector>
#include <utility>
#include <optional>
#include "Kernel/IANSvc.h"
#include "GaudiKernel/VectorMap.h"

/** @class IANNSvc IANNSvc.h
 *
 * ANN: Assigned Names and Numbers
 *   or Assigned Numbers and Names
 *
 *  functionality:
 *         Interface for obtaining names for numbers,
 *         and numbers given names... similar to the
 *         mapping of IP addresses to names.
 *
 *         The code relies on an invertable relation between
 *         names and numbers, i.e. it represent a 'bijective'
 *         mapping.
 *
 *         Each mapping is labeled by a 'major' key,
 *         i.e. for each major key, there exists an individual
 *         map from a set of numbers to a set of names, and
 *         vice-versa.
 *
 *  @author Gerhard Raven
 *  @date   2008-01-02
 */


struct IANNSvc : extend_interfaces<IANSvc>  {
  /// Return the interface ID
  DeclareInterfaceID(IANNSvc,5,0);

  typedef IANSvc::major_key_type                      major_key_type;
  typedef IANSvc::minor_key_type                      minor_key_type;
  typedef int                                         minor_mapped_type;
  typedef std::pair<minor_key_type,minor_mapped_type> minor_value_type;

  /// the two main functions: map 'major/minor key' -> minor key, value
  virtual std::optional<minor_value_type>  value(const major_key_type& major, const std::string& minor) const = 0;
  ///                     and map 'major/minor value' -> minor key, value
  virtual std::optional<minor_value_type>  value(const major_key_type& major, int minor) const = 0;

  /// introspection, i.e. access to what is available: is a major present?
  bool hasMajor(const major_key_type& major) const override = 0;
  ///                                                  return list of majors
  std::vector< major_key_type > majors() const override = 0;
  ///                                                  return minors for a given major
  virtual std::vector<minor_value_type> items(const major_key_type& major) const = 0;
  ///                                                  optimal return of map for later use without additional lookups
  virtual GaudiUtils::VectorMap< minor_value_type::first_type, minor_value_type::second_type > item_map(const major_key_type& major) const = 0;
};
#endif // IANNSVC_H
