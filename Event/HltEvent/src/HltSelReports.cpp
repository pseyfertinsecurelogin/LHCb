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
#include "Event/HltSelReports.h"

//-----------------------------------------------------------------------------
// Implementation file for class : HltSelReports
//
// 2011-02-12 : Chris Jones
//-----------------------------------------------------------------------------

std::ostream& LHCb::HltSelReports::fillStream( std::ostream& ss ) const {
  ss << " HltSelReports : { \n";
  for ( const auto& i : m_selReports ) {
    ss << " selectionName : " << i.first << " HltObjectSummary : " << i.second << '\n';
  }
  ss << " }" << std::endl;
  return ss;
}
