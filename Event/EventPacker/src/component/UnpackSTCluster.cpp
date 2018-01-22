#include <string>

#include "UnpackSTCluster.h"

DECLARE_COMPONENT( UnpackSTCluster )

UnpackSTCluster::UnpackSTCluster(const std::string& name, ISvcLocator* svcLocator)
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

StatusCode UnpackSTCluster::execute() {
  if (msgLevel(MSG::DEBUG)) {
    debug() << "==> Execute" << endmsg;
  }

  // If input does not exist, and we aren't making the output regardless, just return
  if (!m_alwaysOutput && !exist<LHCb::PackedClusters>(m_inputName)) {
    return StatusCode::SUCCESS;
  }

  // Check to see if packed output already exists. If it does print a warning and return
  auto out = getIfExists<LHCb::STClusters>(m_outputName);
  if (out) {
    return Warning("Unpacked Clusters already exist at '" + m_outputName + "' -> Abort",
                   StatusCode::SUCCESS);
  }

  // Create and save the output container
  out = new LHCb::STClusters();
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
    if ( det != 2 && det != 3 )
    {
      Warning("Cannot unpack PackedCluster of detector type '" + std::to_string(det) + "'",
              StatusCode::SUCCESS).ignore();
      continue;
    }
    int id = packedCluster.id & 0x1FFFFFFF;
    LHCb::VeloCluster::ADCVector adcs;
    for ( unsigned int idx = packedCluster.begin; packedCluster.end != idx; ++idx )
    {
      adcs.push_back( std::pair<int,unsigned int>( packedClusters->strips()[idx],
                                                   packedClusters->adcs()[idx]  ) );
    }

    const LHCb::STChannelID sId( id & 0xFFFFFF );
    id = id >> 24;
    const double frac = 0.25 * ( id & 3 );
    int size = 1;
    if ( 0 != frac ) size = 2;
    if ( ( id & 4 ) != 0 ) size = 3;
    bool high = ( id & 8 ) != 0;
    const LHCb::STLiteCluster sl( sId, frac, size, high );
    if ( 0 == packedCluster.spill )
    {
      LHCb::STCluster* sCl = new LHCb::STCluster( sl, adcs,
                                                  double(packedCluster.sum),
                                                  packedCluster.sourceID,
                                                  packedCluster.tell1Channel,
                                                  LHCb::STCluster::Spill::Central );
      if ( msgLevel(MSG::VERBOSE) )
        verbose() << " Unpacked " << sCl->channelID() << endmsg;
      try
      {
        out->insert( sCl, sCl->channelID() );
      }
      catch ( const GaudiException & excpt )
      {
        if ( msgLevel(MSG::VERBOSE) )
          verbose() << excpt.message() << endmsg;
        delete sCl;
      }
    }
    else
    {
      Warning( "ST cluster from non central spill !" ).ignore();
    }
  }

  // It is expected that cluster containers are sorted
  std::sort( out->begin(),  out->end(),  compareKeys<LHCb::STCluster> );

  // Summary of the size of the packed container
  counter("# UnpackedSTClusters") += out->size();

  return StatusCode::SUCCESS;
}
