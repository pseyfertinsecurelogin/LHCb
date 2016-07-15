
// from Gaudi
#include "Event/Track.h"

// local
#include "PackCluster.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PackCluster
//
// 2012-03-05 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( PackCluster )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
  PackCluster::PackCluster( const std::string& name,
                            ISvcLocator* pSvcLocator)
    : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "InputName" ,    m_inputName  = LHCb::TrackLocation::Default );
  declareProperty( "OutputName",    m_outputName = LHCb::PackedClusterLocation::Default );
  declareProperty( "VeloClusters", m_veloClusLoc = LHCb::VeloClusterLocation::Default );
  declareProperty( "TTClusters",     m_ttClusLoc = LHCb::STClusterLocation::TTClusters );
  declareProperty( "UTClusters",     m_utClusLoc = LHCb::STClusterLocation::UTClusters );
  declareProperty( "ITClusters",     m_itClusLoc = LHCb::STClusterLocation::ITClusters );
  declareProperty( "AlwaysCreateOutput", m_alwaysOutput = false );
  //setProperty( "OutputLevel", 1 );
}

//=============================================================================
// Destructor
//=============================================================================
PackCluster::~PackCluster() {}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PackCluster::execute()
{
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  // If input does not exist, and we aren't making the output regardless, just return
  if ( !m_alwaysOutput && !exist<LHCb::Tracks>(m_inputName) ) return StatusCode::SUCCESS;

  // Check to see if packed output already exists. If it does print a warning and return
  LHCb::PackedClusters * out = getIfExists<LHCb::PackedClusters>(m_outputName);
  if ( out )
  {
    return Warning( "Packed Clusters already exist at '"+m_outputName+"' -> Abort",
                    StatusCode::SUCCESS, 3 );
  }

  // Create and save the output container
  out = new LHCb::PackedClusters();
  put( out, m_outputName );
  
  // Load the input. If not existing just return
  const LHCb::Tracks* tracks = getIfExists<LHCb::Tracks>( m_inputName );
  if ( !tracks ) return StatusCode::SUCCESS;

  // Select LHCbIDs from the input tracks
  std::vector<LHCb::LHCbID> allIds;
  allIds.reserve( tracks->size() * 40 );
  for ( const auto* tk : *tracks )
  {
    for ( const auto& id : tk->lhcbIDs() )
    {
      // Skip OT hits ...
      if ( !id.isOT() ) { allIds.emplace_back(id); }
    }
  }
 
  // Sort and remove duplicate LHCbIDs
  std::sort( allIds.begin(), allIds.end() );
  allIds.erase( std::unique(allIds.begin(),allIds.end()), allIds.end() );

  // Cluster pointers. Only loaded when really needed.
  const LHCb::VeloClusters* vClus = nullptr;
  const LHCb::STClusters*  ttClus = nullptr;
  const LHCb::STClusters*  utClus = nullptr;
  const LHCb::STClusters*  itClus = nullptr;

  // pack the clusters
  for ( const auto& id : allIds )
  {
    if ( msgLevel(MSG::VERBOSE) ) { verbose() << "Packing " << id << endmsg; }

    // Pack by type
    if ( id.isVelo() )
    {
      if ( UNLIKELY(!vClus) ) 
      { 
        vClus = getIfExists<LHCb::VeloClusters>(m_veloClusLoc); 
        if ( !vClus ) { Warning("Failed to load '"+m_veloClusLoc+"'"); }
      }
      const auto * cl = ( vClus ? vClus->object(id.veloID()) : nullptr );
      if ( cl ) { out->addVeloCluster( cl ); }
      else 
      {
        Warning( "Failed to locate a Velo cluster. Activate debug for details" ).ignore();
        if ( msgLevel(MSG::DEBUG) ) debug() << "Unknown Velo cluster : " << id << endmsg;
      }
    }
    else if ( id.isTT() )
    {
      if ( UNLIKELY(!ttClus) ) 
      {
        ttClus = getIfExists<LHCb::STClusters>(m_ttClusLoc);
        if ( !ttClus ) { Warning("Failed to load '"+m_ttClusLoc+"'"); }
      }
      const auto * cl = ( ttClus ? ttClus->object(id.stID()) : nullptr );
      if ( cl ) { out->addTTCluster( cl ); }
      else 
      {
        Warning( "Failed to locate a TT cluster. Activate debug for details" ).ignore();
        if ( msgLevel(MSG::DEBUG) ) debug() << "Unknown TT cluster : " << id << endmsg;
      }
    } 
    else if ( id.isUT() )
    {
      if ( UNLIKELY(!utClus) ) 
      {
        utClus = getIfExists<LHCb::STClusters>(m_utClusLoc);
        if ( !utClus ) { Warning("Failed to load '"+m_utClusLoc+"'"); }
      }
      const auto * cl = ( utClus ? utClus->object(id.stID()) : nullptr );
      if ( cl ) { out->addUTCluster( cl ); }
      else 
      {
        Warning( "Failed to locate a UT cluster. Activate debug for details" ).ignore();
        if ( msgLevel(MSG::DEBUG) ) debug() << "Unknown UT cluster : " << id << endmsg;
      }
    } 
    else if ( id.isIT() ) 
    {
      if ( UNLIKELY(!itClus) ) 
      {
        itClus = getIfExists<LHCb::STClusters>(m_itClusLoc);
        if ( !itClus ) { Warning("Failed to load '"+m_itClusLoc+"'"); }
      }
      const auto * cl = ( itClus ? itClus->object(id.stID()) : nullptr );
      if ( cl ) { out->addITCluster( cl ); }
      else
      {
        Warning( "Failed to locate a IT cluster. Activate debug for details" ).ignore();
        if ( msgLevel(MSG::DEBUG) ) debug() << "Unknown IT cluster : " << id << endmsg;
      }
    } 
    else 
    {
      Warning( "Unknown LHCbID. Activate debug for details" ).ignore();
      if ( msgLevel(MSG::DEBUG) ) debug() << "Unknown LHCbID type : " << id << endmsg;
    }
  }

  if ( msgLevel(MSG::DEBUG) )
    debug() << "Stored " << out->clusters().size() << " clusters" << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
