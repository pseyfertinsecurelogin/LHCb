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
// Include files

// local
#include "Event/HltObjectSummary.h"

//-----------------------------------------------------------------------------
// Implementation file for class : HltObjectSummary
//
// 2011-02-12 : Chris Jones
//-----------------------------------------------------------------------------

std::ostream& LHCb::HltObjectSummary::fillStream( std::ostream& ss ) const {
  ss << "{ "
     << " summarizedObjectCLID : " << summarizedObjectCLID();
  ss << " numericalInfo : { ";
  for ( const auto& i : numericalInfo() ) { ss << i.first << ":" << i.second << " , "; }
  ss << " } ";
  ss << " substructure : { ";
  for ( const auto& j : substructure() ) { ss << j->index() << ":" << j->summarizedObjectCLID() << " , "; }
  ss << " } ";
  ss << " substructureExtended : { ";
  for ( const auto& j : substructureExtended() ) { ss << j->index() << ":" << j->summarizedObjectCLID() << " , "; }
  ss << " } ";
  ss << " lhcbIDs : { ";
  for ( const auto& k : lhcbIDs() ) { ss << k.lhcbID() << " , "; }
  ss << " } ";
  ss << " } ";
  return ss;
}
