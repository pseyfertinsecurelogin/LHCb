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
#ifndef _ITrackGhostClassification_H
#define _ITrackGhostClassification_H

#include "GaudiKernel/IAlgTool.h"
#include <vector>

#include "Event/Track.h"

namespace LHCb{
  class LHCbID;
  class GhostTrackInfo;
}

struct ITrackGhostClassification: extend_interfaces<IAlgTool> {

 typedef std::vector<LHCb::LHCbID> LHCbIDs;
 
 /// Retrieve interface ID
 DeclareInterfaceID(ITrackGhostClassification, 1,0 );

 /**
 *  Information on what a ghost track is....
 *  @param aTrack to link
 *  @param tinfo link info 
 *  @return statuscode whether classification was successful
 */
 virtual StatusCode info(const LHCb::Track& aTrack, LHCb::GhostTrackInfo& tinfo) const= 0;

 /**
 *  Information on a list of LHCbIDs
 *  @param start first iterator 
 *  @param stop  last iterator
 *  @param tinfo link info
 *  @return statuscode whether classification was successful
 */
 virtual StatusCode info(LHCbIDs::const_iterator& start, 
                   LHCbIDs::const_iterator& stop, LHCb::GhostTrackInfo& tinfo) const= 0;

  /**
  *  Check whether this is a ghost .
  *  @param aTrack to link
  *  @return bool true if a ghost
  */
  virtual bool isGhost(const LHCb::Track& aTrack) const= 0;

  /**
   *  Check whether this is a ghost .
   *  @param aTrack to link
   *  @return bool true if a ghost
   */
  virtual bool isGhost(LHCbIDs::const_iterator& start, 
                       LHCbIDs::const_iterator& stop) const = 0;

};

#endif
