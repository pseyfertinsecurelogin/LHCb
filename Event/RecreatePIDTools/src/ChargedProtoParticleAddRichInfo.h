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

// Event
#include "Event/RichPID.h"
#include "Event/ProtoParticle.h"

/** @class ChargedProtoParticleAddRichInfo ChargedProtoParticleAddRichInfo.h
 *
 *  Updates the RichPID information stored in the ProtoParticles
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date 28/08/2009
 */

class ChargedProtoParticleAddRichInfo : public GaudiAlgorithm
{

public:

  /// Standard constructor
  ChargedProtoParticleAddRichInfo( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~ChargedProtoParticleAddRichInfo( ); ///< Destructor

  StatusCode execute() override;    ///< Algorithm execution

private:

  /// Load the RichPIDs and build reverse mappings
  bool getRichData();

  /// Add (or update) the RICH information for the given ProtoParticle
  void updateRICH( LHCb::ProtoParticle * proto ) const;

private:

  std::string m_protoPath; ///< Location of the ProtoParticles in the TES
  std::string m_richPath;  ///< Location in TES of input RichPIDs

  /// mapping type from Track to RichPID data objects
  typedef std::map<const LHCb::Track *, const LHCb::RichPID *> TrackToRichPID;
  /// mapping from Track to RichPID data objects
  TrackToRichPID m_richMap;

};

#endif // GLOBALRECO_ChargedProtoParticleAddRichInfo_H
