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
#ifndef ICONFIGTREEEDITOR_H
#define ICONFIGTREEEDITOR_H 1

// Include files
#include "GaudiKernel/INamedInterface.h"
#include "Kernel/ConfigTreeNode.h"
#include <map>
#include <string>
#include <utility>

/** @class IConfigTreeEditor IConfigTreeEditor.h
 *
 *  functionality:
 *         Interface for obtaining an
 *         individual configuration description
 *         or (an element of) the graph structure
 *         given its ID
 *
 *         The ID corresponds to the MD5 digest of the contents
 *         of the configration description
 *
 *  @author Gerhard Raven
 *  @date   2007-12-12
 */

struct IConfigTreeEditor : extend_interfaces<INamedInterface> {
  DeclareInterfaceID( IConfigTreeEditor, 5, 0 );

  // given a configuration tree identified by 'in', apply the changes
  // defined by 'updates' (which is a multimap of 'component' -> 'key','value'),
  // and return a new configuration tree
  // Note that the new tree is assumed to have been written to the underlying
  // storage...
  // Failure is indicated by returning an 'invalid' digest.
  virtual ConfigTreeNode::digest_type
  updateAndWrite( const ConfigTreeNode::digest_type&                                     in,
                  const std::multimap<std::string, std::pair<std::string, std::string>>& updates,
                  const std::string&                                                     label ) const = 0;
};
#endif // ICONFIGTREEEDITOR_H
