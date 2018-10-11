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
#ifndef UNPACKPROTOPARTICLE_H
#define UNPACKPROTOPARTICLE_H 1

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Event/PackedProtoParticle.h"
#include "Event/ProtoParticle.h"

/** @class UnpackProtoParticle UnpackProtoParticle.h
 *  Unpack a ProtoParticle
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
class UnpackProtoParticle : public GaudiAlgorithm
{

public:

  /// Standard constructor
  UnpackProtoParticle( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override;    ///< Algorithm execution

private:

  std::string m_inputName;
  std::string m_outputName;
  bool m_alwaysOutput;      ///< Flag to turn on the creation of output, even when input is missing

};

#endif // UNPACKPROTOPARTICLE_H
