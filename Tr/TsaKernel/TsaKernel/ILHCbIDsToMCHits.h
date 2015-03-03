#ifndef _ILHCbIDsToMCHits_H
#define _ILHCbIDsToMCHits_H

#include "GaudiKernel/IAlgTool.h"
#include <vector>
#include <map>


namespace LHCb{
  class LHCbID;
  class MCParticle;
  class Track;
  class MCHit;
}

static const InterfaceID IID_ILHCbIDsToMCHits( "ILHCbIDsToMCHits", 0, 0 );

/** @class ILHCbIDsToMCHits
*
*  Link ids to MCHits
*
*  @author M.Needham
*  @date   31/05/2004
*/


class ILHCbIDsToMCHits: virtual public IAlgTool {

public:

  
  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_ILHCbIDsToMCHits ; }

  typedef std::vector<LHCb::LHCbID> LHCbIDs;
  typedef std::map<LHCb::MCHit*, unsigned int> LinkMap;
  typedef std::pair<LHCb::MCHit*, unsigned int> LinkPair;

  /**
    Trivial link from list of IDs to all particles contributing
    @param iterator to first id
    @param iterator to last id
    @param output vector by reference
    @return StatusCode
  */
  virtual StatusCode link(LHCbIDs::const_iterator& start, 
                          LHCbIDs::const_iterator& stop, LinkMap& output) const = 0;


  /**
    Trivial link from list of ALL ids in track to particles contributing
    @param track
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



  
