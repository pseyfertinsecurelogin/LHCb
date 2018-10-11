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
/** @file RichPIDsFromProtoParticlesAlg.h
 *
 * Header file for algorithm RichPIDsFromProtoParticlesAlg
 *
 * @author Chris Jones   Christopher.Rob.Jones@cern.ch
 * @date 29/03/2006
 */
//-----------------------------------------------------------------------------

#ifndef GLOBALRECO_RICHPIDSFROMPROTOPARTICLESALG_H
#define GLOBALRECO_RICHPIDSFROMPROTOPARTICLESALG_H 1

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

// Event
#include "Event/ProtoParticle.h"
#include "Event/RichPID.h"

//-----------------------------------------------------------------------------
/** @class RichPIDsFromProtoParticlesAlg RichPIDsFromProtoParticlesAlg.h
 *
 *  Simple algorithm to recreate RichPID data objects from the data
 *  stored in the ProtoParticles
 *m_richPIDloc
 *  @author Chris Jones
 *  @date   2006-08-01
 */
//-----------------------------------------------------------------------------

class RichPIDsFromProtoParticlesAlg : public GaudiAlgorithm
{

public:

  /// Standard constructor
  RichPIDsFromProtoParticlesAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  /// Input location of ProtoParticles in TES
  std::string m_protoPloc;

  /// Output location of RichPIDs in TES
  std::string m_richPIDloc;

};

#endif // GLOBALRECO_RICHPIDSFROMPROTOPARTICLESALG_H
