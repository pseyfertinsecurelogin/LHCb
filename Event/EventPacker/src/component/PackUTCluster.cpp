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
#include "PackUTCluster.h"

DECLARE_COMPONENT( PackUTCluster )

PackUTCluster::PackUTCluster(const std::string& name, ISvcLocator* svcLocator)
  : GaudiAlgorithm(name , svcLocator)
{
}

StatusCode PackUTCluster::execute() {
  if (msgLevel(MSG::DEBUG)) {
    debug() << "==> Execute" << endmsg;
  }

  // If input does not exist, and we aren't making the output regardless, just return
  if (!m_alwaysOutput && !exist<LHCb::UTClusters>(m_inputName)) {
    return StatusCode::SUCCESS;
  }

  // Check to see if packed output already exists. If it does print a warning and return
  auto out = getIfExists<LHCb::PackedClusters>(m_outputName);
  if (out) {
    return Warning("Packed Clusters already exist at '" + m_outputName + "' -> Abort",
                   StatusCode::SUCCESS);
  }

  // Create and save the output container
  out = new LHCb::PackedClusters();
  out->setPackingVersion( LHCb::PackedClusters::defaultPackingVersion() );
  put(out, m_outputName);

  // Load the input. If not existing just return
  const auto clusters = getIfExists<LHCb::UTClusters>(m_inputName);
  if (!clusters) {
    return StatusCode::SUCCESS;
  }

  // Pack the clusters
  for (const auto& cluster : *clusters)
  {
    if (msgLevel(MSG::VERBOSE)) {
      verbose() << "Packing " << cluster << endmsg;
    }

    if (cluster->isUT()) {
      out->addUTCluster(cluster);
    } else {
      Warning("Unsupported cluster type, not packing",
              StatusCode::SUCCESS).ignore();
    }
  }

  // If requested, remove the input data from the TES and delete
  if ( UNLIKELY(m_deleteInput) )
  {
    const StatusCode sc = evtSvc()->unregisterObject( clusters );
    if ( sc.isSuccess() )
    {
      delete clusters;
    }
    else
    {
      return Error("Failed to delete input data as requested", sc );
    }
  }
  else
  {
    // Clear the registry address of the unpacked container, to prevent reloading
    clusters->registry()->setAddress( 0 );
  }

  // Summary of the size of the packed container
  counter("# PackedUTClusters") += out->clusters().size();

  return StatusCode::SUCCESS;
}
