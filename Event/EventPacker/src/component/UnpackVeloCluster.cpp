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
#include <string>

#include "UnpackVeloCluster.h"

DECLARE_COMPONENT( UnpackVeloCluster )

UnpackVeloCluster::UnpackVeloCluster(const std::string& name, ISvcLocator* svcLocator)
  : GaudiAlgorithm(name , svcLocator)
{
}

// explicit comparison for cluster sorting (hidden in anonymous namespace)
namespace 
{
  template < class TYPE >
  inline bool compareKeys( const TYPE * a, const TYPE * b ) 
  {
    return ( a->key() < b->key() );
  }
}

StatusCode UnpackVeloCluster::execute() {
  if (msgLevel(MSG::DEBUG)) {
    debug() << "==> Execute" << endmsg;
  }

  // If input does not exist, and we aren't making the output regardless, just return
  if (!m_alwaysOutput && !exist<LHCb::PackedClusters>(m_inputName)) {
    return StatusCode::SUCCESS;
  }

  // Check to see if packed output already exists. If it does print a warning and return
  auto out = getIfExists<LHCb::VeloClusters>(m_outputName);
  if (out) {
    return Warning("Unpacked Clusters already exist at '" + m_outputName + "' -> Abort",
                   StatusCode::SUCCESS);
  }

  // Create and save the output container
  out = new LHCb::VeloClusters();
  put(out, m_outputName);

  // Load the input. If not existing just return
  const auto packedClusters = getIfExists<LHCb::PackedClusters>(m_inputName);
  if (!packedClusters) {
    return StatusCode::SUCCESS;
  }

  // Unpack the clusters
  for (const auto& packedCluster : packedClusters->clusters())
  {
    if (msgLevel(MSG::VERBOSE)) {
      verbose() << "Unpacking PackedCluster ID" << packedCluster.id << endmsg;
    }

    const int det = packedCluster.id >> 29;
    if ( det != 1 )
    {
      Warning("Cannot unpack PackedCluster of detector type '" + std::to_string(det) + "'").ignore();
      continue;
    }
    int id = packedCluster.id & 0x1FFFFFFF;
    LHCb::VeloCluster::ADCVector adcs;
    for ( unsigned int idx = packedCluster.begin; packedCluster.end != idx; ++idx )
    {
      adcs.push_back( std::pair<int,unsigned int>( packedClusters->strips()[idx],
                                                   packedClusters->adcs()[idx]  ) );
    }

    const LHCb::VeloChannelID vId( id & 0xFFFFFF );
    id = id >> 24;
    const double frac = 0.125 * ( id & 7 );
    int size = 1;
    if ( 0 != frac ) size = 2;
    if ( ( id & 8 ) != 0 ) size = 3;
    bool high = ( id & 0x10 ) != 0;
    const LHCb::VeloLiteCluster vl( vId, frac, size, high );
    LHCb::VeloCluster* vCl = new LHCb::VeloCluster( vl, adcs );
    if ( msgLevel(MSG::VERBOSE) ) {
      verbose() << " Unpacked " << vCl->channelID() << endmsg;
    }

    try
    {
      out->insert( vCl, vCl->channelID() );
    }
    catch ( const GaudiException & excpt )
    {
      if ( msgLevel(MSG::VERBOSE) )
        verbose() << excpt.message() << endmsg;
      delete vCl;
    }
  }

  // It is expected that cluster containers are sorted
  std::sort( out->begin(),  out->end(),  compareKeys<LHCb::VeloCluster> );

  // Summary of the size of the packed container
  counter("# UnpackedVeloClusters") += out->size();

  return StatusCode::SUCCESS;
}
