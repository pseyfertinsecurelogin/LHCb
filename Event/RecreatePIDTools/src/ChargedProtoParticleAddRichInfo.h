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

//-----------------------------------------------------------------------------
/** @file ChargedProtoParticleAddRichInfo.h
 *
 * Header file for algorithm ChargedProtoParticleAddRichInfo
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date 29/03/2006
 */
//-----------------------------------------------------------------------------

#ifndef GLOBALRECO_ChargedProtoParticleAddRichInfo_H
#define GLOBALRECO_ChargedProtoParticleAddRichInfo_H 1

// STL
#include <sstream>

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/DataObjectHandle.h"

// Event
#include "Event/ProtoParticle.h"
#include "Event/RichPID.h"

/** @class ChargedProtoParticleAddRichInfo ChargedProtoParticleAddRichInfo.h
 *
 *  Updates the RichPID information stored in the ProtoParticles
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date 28/08/2009
 */

class ChargedProtoParticleAddRichInfo : public GaudiAlgorithm {

public:
  /// mapping type from Track to RichPID data objects
  using TrackToRichPID = std::map<const LHCb::Track*, const LHCb::RichPID*>;

  /// Standard constructor
  ChargedProtoParticleAddRichInfo( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:
  /// Load the RichPIDs and build reverse mappings
  TrackToRichPID getRichData() const;

  /// Add (or update) the RICH information for the given ProtoParticle
  void updateRICH( LHCb::ProtoParticle* proto, TrackToRichPID const& richMap ) const;

private:
  // ProtoParticles
  DataObjectReadHandle<LHCb::ProtoParticles> m_protos{this, "ProtoParticleLocation",
                                                      LHCb::ProtoParticleLocation::Charged};
  DataObjectReadHandle<LHCb::RichPIDs>       m_richpids{this, "InputRichPIDLocation", LHCb::RichPIDLocation::Offline};
};

#endif // GLOBALRECO_ChargedProtoParticleAddRichInfo_H
