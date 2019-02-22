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
#ifndef IANSVC_H
#define IANSVC_H 1

// Include files
#include "GaudiKernel/INamedInterface.h"
#include "GaudiKernel/StringKey.h"
#include <string>
#include <vector>

/** @class IANSvc IANSvc.h
 *
 * AN: Assigned Names
 *
 *  functionality:
 *         Interface for obtaining a list of names
 *
 *         Each list is labeled by a 'major' key,
 *         i.e. for each major key, there exists an individual
 *         list of names.
 *
 *  @author Gerhard Raven
 *  @date   2008-06-25
 */

struct IANSvc : extend_interfaces<INamedInterface> {
  /// Return the interface ID
  DeclareInterfaceID( IANSvc, 4, 0 );

  typedef Gaudi::StringKey major_key_type;
  typedef Gaudi::StringKey minor_key_type;

  /// introspection, i.e. access to what is available: is a major present?
  virtual bool hasMajor( const major_key_type& major ) const = 0;
  ///                                                  return list of majors
  virtual std::vector<major_key_type> majors() const = 0;
  ///                                                  return minors for a given major
  virtual std::vector<minor_key_type> keys( const major_key_type& major ) const = 0;
};
#endif // IANSVC_H
