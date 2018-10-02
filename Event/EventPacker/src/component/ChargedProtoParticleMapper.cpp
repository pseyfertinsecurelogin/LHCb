
// Include files
#include <iterator>
// local
#include "ChargedProtoParticleMapper.h"

// ----------------------------------------------------------------------------
// Implementation file for class: ParticlesAndVerticesMapper
//
// 17/01/2012: Marco Clemencic
// ----------------------------------------------------------------------------

// ============================================================================
// Standard constructor, initializes variables
// ============================================================================
ChargedProtoParticleMapper::ChargedProtoParticleMapper( const std::string& type,
                                                        const std::string& name,
                                                        const IInterface* parent )
  : MapperToolBase( type, name, parent )
{
  declareProperty( "UnpackerOutputLevel", m_unpackersOutputLevel = -1 );
  declareProperty( "TrackTypes", m_tkTypes = {"Long","Downstream","Upstream"} );
  declareProperty( "PIDTypes", m_pidTypes = { "Electron", "Muon", "Pion",
                                              "Kaon", "Proton", "Ghost" } );
  declareProperty( "ANNPIDTune", m_pidTune = "" );
  declareProperty( "VersionRegex", m_regex = "" );
}

// ============================================================================

Gaudi::Utils::TypeNameString
ChargedProtoParticleMapper::algorithmForPath( const std::string & path )
{
  LOG_VERBOSE << "ChargedProtoParticleMapper::algorithmForPath '"
              << path << "'" << endmsg;

  updateNodeTypeMap(path);

  if ( pathIsHandled(path) )
  {
    // Choose a unique name
    const auto baseName = streamName(path) + "_ChargedPP";

    // Use a sequencer as the main type
    const std::string unpackerType = "GaudiSequencer";
    const auto        seqName      = baseName + "_Seq";

    // List of algorithms to run in the sequencer
    std::vector<std::string> algs;

    // Add the basic unpacker
    const auto unpackName = baseName + "_Unpack";
    algs.emplace_back( "UnpackProtoParticle/" + unpackName );
    auto sc = joSvc()->addPropertyToCatalogue( unpackName,
                                               StringProperty("InputName",
                                                              packedProtoLocation(streamRoot(path))) );
    if (sc) sc = joSvc()->addPropertyToCatalogue( unpackName,
                                                  StringProperty("OutputName",
                                                                 protoLocation(streamRoot(path))) );

    // PID recalibration
    if ( !m_pidTune.empty() )
    {
      if ( !m_regex.empty() )
      {
        const auto name = "Reco14Filter_" + baseName;
        if (sc) sc = joSvc()->addPropertyToCatalogue( name, StringProperty("HeaderLocation","Rec/Header") );
        if (sc) sc = joSvc()->addPropertyToCatalogue( name, StringProperty("VersionRegex",m_regex) );
        algs.emplace_back( "ApplicationVersionFilter/" + name );
      }
      for ( const auto& tkType : m_tkTypes )
      {
        for ( const auto& pidType : m_pidTypes )
        {
          const auto name = "ANNPID" + tkType + pidType + "_" + baseName;
          if (sc) sc = joSvc()->addPropertyToCatalogue( name, StringProperty("TrackType",tkType) );
          if (sc) sc = joSvc()->addPropertyToCatalogue( name, StringProperty("PIDType",pidType) );
          if (sc) sc = joSvc()->addPropertyToCatalogue( name, StringProperty("NetworkVersion",m_pidTune) );
          if (sc) sc = joSvc()->addPropertyToCatalogue( name, StringProperty("ProtoParticleLocation",
                                                                protoLocation(streamRoot(path))) );
          algs.emplace_back( "ANNGlobalPID::ChargedProtoANNPIDAlg/" + name );
        }
      }
    }

    // Set the sequencer alg list
    if (sc) sc = joSvc()->addPropertyToCatalogue( seqName, StringArrayProperty("Members",algs) );

    // Set output levels, if required
    if ( m_unpackersOutputLevel > 0 )
    {
      std::stringstream lvl;
      lvl << m_unpackersOutputLevel;
      if (sc) sc = joSvc()->addPropertyToCatalogue( seqName,
                                                    StringProperty("OutputLevel",lvl.str()));
      for ( const auto & a : algs )
      {
        const auto slash = a.find_last_of("/");
        const auto N = ( slash != std::string::npos ? a.substr(slash+1) : a );
        if (sc) sc = joSvc()->addPropertyToCatalogue( N, StringProperty("OutputLevel",lvl.str()) );
      }
    }

    if ( UNLIKELY(!sc) )
    {
      Exception( "Failed to configure Job Options Service" );
    }

    // Return the algorithm type/name.
    LOG_VERBOSE << " -> Use algorithm type '" << unpackerType << "'"
                << " name '" << seqName << "'"
                << endmsg;
    return Gaudi::Utils::TypeNameString(seqName,unpackerType);
  }

  return "";
}

// ============================================================================

std::string
ChargedProtoParticleMapper::nodeTypeForPath( const std::string & path )
{
  updateNodeTypeMap(path);

  auto it = m_nodeTypeMap.find( fixPath(path) );

  const auto retS = ( it != m_nodeTypeMap.end() ? it->second : "" );

  LOG_VERBOSE << "ChargedProtoParticleMapper::nodeTypeForPath '"
              << path << "' NodeType '" << retS << "'" << endmsg;

  return retS;
}

// ============================================================================

void ChargedProtoParticleMapper::updateNodeTypeMap( const std::string & path )
{
  // The stream TES root
  const auto streamR = streamRoot(path);

  LOG_VERBOSE << "ChargedProtoParticleMapper::updateNodeTypeMap Running for "
              << streamR << endmsg;

  // See if the packed clusters object exists for this stream
  if ( !m_streamsDone[streamR] )
  {
    m_streamsDone[streamR] = true;
    const auto packedLoc = packedProtoLocation(streamR);
    LOG_VERBOSE << "ChargedProtoParticleMapper::updateNodeTypeMap Looking for "
                << packedLoc << endmsg;
    if ( exist<LHCb::PackedProtoParticles*>(packedLoc) )
    {
      // Update the node type paths handled
      addPath( protoLocation(streamR) );
    }
    else
    {
      LOG_VERBOSE << " -> Missing" << endmsg;
    }
  }
}

// ============================================================================

void ChargedProtoParticleMapper::addPath( const std::string & path )
{
  // Make sure paths start with /Event/
  const auto npath = fixPath(path);

  // if not already there, add.
  if ( m_nodeTypeMap.find(npath) == m_nodeTypeMap.end() )
  {
    LOG_VERBOSE << " -> Path " << npath << endmsg;

    // Main path
    m_nodeTypeMap[npath] = "";

    // Data Node paths ...
    auto tmp = npath;
    auto slash = tmp.find_last_of("/");
    while ( !tmp.empty() && slash != std::string::npos )
    {
      tmp = tmp.substr(0,slash);
      if ( !tmp.empty() )
      {
        LOG_VERBOSE << "  -> Node " << tmp << endmsg;
        m_nodeTypeMap[tmp] = "DataObject";
      }
      slash = tmp.find_last_of("/");
    }
  }

}

// ============================================================================

DECLARE_COMPONENT( ChargedProtoParticleMapper )

// ============================================================================
