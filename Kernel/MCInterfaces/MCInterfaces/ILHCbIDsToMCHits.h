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
#ifndef _ILHCbIDsToMCHits_H
#define _ILHCbIDsToMCHits_H

#include "GaudiKernel/IAlgTool.h"
#include <utility>
#include <vector>
#include <map>

#include "Event/Track.h"

namespace LHCb{
  class LHCbID;
  class MCParticle;
  class MCHit;
}


/** @class ILHCbIDsToMCHits ILHCbIDsToMCHits.h TsaKernel/ILHCbIDsToMCHits.h
*
*  Link ids to MCHits
*
*  @author M.Needham
*  @date   31/05/2004
*/


struct ILHCbIDsToMCHits: extend_interfaces<IAlgTool> {
  
  /// Retrieve interface ID
  DeclareInterfaceID(ILHCbIDsToMCHits, 1, 0 );

  typedef std::vector<LHCb::LHCbID> LHCbIDs;
  typedef std::map<LHCb::MCHit*, unsigned int> LinkMap;
  typedef std::pair<LHCb::MCHit*, unsigned int> LinkPair;

  /**
    Trivial link from list of IDs to all particles contributing
    @param start  iterator to first id
    @param stop   iterator to last id
    @param output vector by reference
    @return StatusCode
  */
  virtual StatusCode link(LHCbIDs::const_iterator& start, 
                          LHCbIDs::const_iterator& stop, LinkMap& output) const = 0;


  /**
    Trivial link from list of ALL ids in track to particles contributing
    @param aTrack track
    @param output vector by reference
    @return StatusCode
  */
  virtual StatusCode link(const LHCb::Track& aTrack, LinkMap& output) const = 0;


  /**
    Trivial link from single id to particles contributing
    @param id
    @param output vector by reference
    @return StatusCode
  */
  virtual StatusCode link(const LHCb::LHCbID& id, LinkMap& output) const = 0;

};

#endif
