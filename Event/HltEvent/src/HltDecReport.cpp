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
#include "Event/HltDecReport.h"

//-----------------------------------------------------------------------------
// Implementation file for class : HltDecReport
//-----------------------------------------------------------------------------

std::ostream& LHCb::HltDecReport::fillStream(std::ostream& s) const
{
  return s << "{ "
           << " decision : " << decision()
           << " errorBits : " << errorBits()
           << " numberOfCandidates : " << numberOfCandidates()
           << " executionStage : " << executionStage()
           << " intDecisionID : " << intDecisionID()
           << " }" ;
}
