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
// $Id: HltTrackingCoder.h $
#ifndef HLTTRACKINGCODER_H 
#define HLTTRACKINGCODER_H 1

// Include files
#include "Event/Track.h"
#include <vector>

/** @function HltTrackingCoder HltTrackingCoder.h
 *  
 *
 *  @author Sebastian Neubert
 *  @date   2014-01-10
 *
 *  Helper to code / decode Tracks into / from RawBank
 *
 *  Unit tests for these functions are in utestTrackingCoder.cxx 
 *
 *  Encoding of track information into RawBank
 *  For each track:
 *  (nLhHCbIDs, ID0, ID1, ... IDn)
 *   
 *  tracks are just concatenated
 *
 */

void
encodeTracks(const LHCb::Tracks& tracks,                 // Input track container
             std::vector<unsigned int>& rawBank,
             bool writeStates);        // Output RawBank body

unsigned int
// returns number of decoded tracks
decodeTracks(const unsigned int* rawBank,                      // Input pointer to rawBank body
	         unsigned int nentries,                      // Input number of entries in rawBank
	         LHCb::Tracks& tracks);                      // Output track container


#endif // HLTTRACKINGCODER_H 
