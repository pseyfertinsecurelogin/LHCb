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
// Include files
#include <iomanip>
// From Detector Element
#include "VeloDet/DeVelo.h"
#include "VeloDet/DeVeloSensor.h"

// From Event
#include "Event/VeloCluster.h"
#include "Event/VeloLiteCluster.h"

// local
#include "VeloClusterFilter.h"

namespace {
  void incrementCounters( LHCb::VeloChannelID id, int& countClusters, int& countRClusters, int& countPhiClusters ) {
    unsigned int sensorNumber = id.sensor();
    if ( sensorNumber < 64 ) {
      ++countRClusters;
    } else if ( sensorNumber < 128 ) {
      ++countPhiClusters;
    }
    ++countClusters;
  }
  static const std::array<std::string, 7> valid_criteria = {"All", "Left", "Right", "R", "Phi", "PU", "Overlap"};
} // namespace

StatusCode parse( VeloClusterFilter::filter_t& result, const std::string& input ) {
  auto i = std::find( valid_criteria.begin(), valid_criteria.end(), input );
  if ( i == valid_criteria.end() ) return StatusCode::FAILURE;
  result.criterion =
      static_cast<VeloClusterFilter::filter_t::criterion_t>( std::distance( valid_criteria.begin(), i ) );
  return StatusCode::SUCCESS;
}
std::ostream& toStream( const VeloClusterFilter::filter_t& crit, std::ostream& os ) {
  return os << std::quoted( crit.toString(), '\'' );
}
const std::string& VeloClusterFilter::filter_t::toString() const {
  return valid_criteria[static_cast<int>( criterion )];
}
bool VeloClusterFilter::filter_t::operator()( LHCb::VeloChannelID id ) const {
  unsigned int sensorNumber = id.sensor();
  switch ( criterion ) {
  case criterion_t::ALL:
    return true;
  case criterion_t::LEFT:
    return sensorNumber % 2 == 0;
  case criterion_t::RIGHT:
    return sensorNumber % 2 == 1;
  case criterion_t::R:
    return sensorNumber < 64;
  case criterion_t::PHI:
    return sensorNumber >= 64 && sensorNumber < 128;
  case criterion_t::PU:
    return sensorNumber >= 128;
  case criterion_t::OVERLAP: {
    unsigned int stripNumber = id.strip();
    if ( id.isRType() || id.isPileUp() ) { return stripNumber < 512 || stripNumber >= 1536; }
    if ( id.isPhiType() ) {
      const unsigned int range = 25;
      return ( stripNumber < range ) || ( stripNumber >= 2048 - range && stripNumber < 2048 ) ||
             ( stripNumber >= 683 - range && stripNumber < 683 + range );
    }
    return false;
  }
  default:
    assert( false ); // unreachable...
    return false;
  }
}

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( VeloClusterFilter )
//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
VeloClusterFilter::VeloClusterFilter( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode VeloClusterFilter::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;              // error printed already by GaudiAlgorithm
  debug() << "==> Initialize" << endmsg;
  m_velo = getDet<DeVelo>( DeVeloLocation::Default );
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode VeloClusterFilter::execute() {
  bool isDebug = msgLevel( MSG::DEBUG );
  if ( isDebug ) { debug() << "==> Execute" << endmsg; }

  int countClusters    = 0;
  int countRClusters   = 0;
  int countPhiClusters = 0;
  int totalClusters    = 0;

  const LHCb::VeloClusters*                   clusters     = m_inputClusterDh.get();
  const LHCb::VeloLiteCluster::FastContainer* liteClusters = m_inputLiteClusterDh.get();
  if ( ( !clusters ) && ( !liteClusters ) ) { return Warning( "No velo clusters or liteClusters on the TES !!!" ); }

  if ( clusters ) {

    auto filteredClusters = std::make_unique<LHCb::VeloClusters>();
    filteredClusters->reserve( clusters->size() );

    for ( LHCb::VeloCluster* cluster : *clusters ) {
      if ( m_filter.value()( cluster->channelID() ) ) {
        filteredClusters->insert( cluster );
        incrementCounters( cluster->channelID(), countClusters, countRClusters, countPhiClusters );
      }
      ++totalClusters;
    }
    m_outputClusterDh.put( filteredClusters.release() );
  }

  if ( liteClusters ) {

    LHCb::VeloLiteCluster::FastContainer filteredLiteClusters;
    filteredLiteClusters.reserve( liteClusters->size() );

    for ( const auto& c : *liteClusters ) {
      if ( m_filter.value()( c.channelID() ) ) {
        filteredLiteClusters.push_back( c );
        incrementCounters( c.channelID(), countClusters, countRClusters, countPhiClusters );
      }
      ++totalClusters;
    }
    m_outputLiteClusterDh.put( std::move( filteredLiteClusters ) );
  }

  if ( isDebug )
    debug() << "Number of \'" << m_filter.value().toString() << "\' clusters surviving = " << countClusters
            << "(R:" << countRClusters << " ,P:" << countPhiClusters << ")"
            << " from " << totalClusters << "." << endmsg;

  if ( countRClusters < m_minNRClustersCut ) setFilterPassed( false );
  if ( countPhiClusters < m_minNPhiClustersCut ) setFilterPassed( false );
  if ( countClusters < m_minNClustersCut ) setFilterPassed( false );
  if ( countRClusters > m_maxNRClustersCut ) setFilterPassed( false );
  if ( countPhiClusters > m_maxNPhiClustersCut ) setFilterPassed( false );
  if ( countClusters > m_maxNClustersCut ) setFilterPassed( false );

  return StatusCode::SUCCESS;
}
