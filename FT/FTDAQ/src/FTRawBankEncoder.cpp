// Include files
#include "boost/optional.hpp"
#include "Event/FTCluster.h"
#include "Event/RawEvent.h"

#include <numeric>

// local
#include "FTRawBankEncoder.h"
#include "FTRawBankParams.h"

constexpr static int s_nbBanks = FTRawBank::NbBanks;
constexpr static int s_nbSipmPerTELL40 = FTRawBank::NbSiPMPerTELL40;

//-----------------------------------------------------------------------------
// Implementation file for class : FTRawBankEncoder
//
// 2012-05-11 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FTRawBankEncoder )

//=============================================================================
// Main execution
//=============================================================================
StatusCode FTRawBankEncoder::execute() {

  //typedef FastClusterContainer<LHCb::FTLiteCluster,int> FTLiteClusters;
  LHCb::FTClusters* clusters = get<LHCb::FTClusters>( m_inputLocation );
  if ( msgLevel( MSG::DEBUG) ) debug() << "Retrieved " << clusters->size() << " clusters" << endmsg;
  LHCb::RawEvent* event = getOrCreate<LHCb::RawEvent,LHCb::RawEvent>( m_outputLocation );

  // Incremented to deal with new numbering scheme
  int codingVersion = 3;

  //== create the array of arrays of vectors with the proper size...
  std::array< std::array< std::vector<uint16_t>, s_nbSipmPerTELL40>, s_nbBanks> sipmData{{}};

  for ( const auto& cluster : *clusters ) {

    if(cluster->isLarge() > 1) continue;
    
    LHCb::FTChannelID id = cluster->channelID();
    //    int module = id.module();
    //    unsigned int bankNumber = (module==1 || module ==2) + ((module==3 || module==4)<<1) + FTRawBank::nTELL40PerQuarter*(id.quarter() + 4*id.layer() + 16*(id.station()-1u));
    unsigned int bankNumber = FTRawBank::nTELL40PerQuarter*(id.quarter() + 4*id.layer() + 16*(id.station()-1u));
    //    debug() << "LoH: " << bankNumber << ": " << id.module() << " " << id.quarter() << " " << id.layer() << " " << id.station()-1u << endmsg;
    //== Temp, will only work with 1 TELL40/quarter.

    if ( sipmData.size() <= bankNumber ) {
      error() << "*** Invalid bank number " << bankNumber << " channelID " << id << endmsg;
      return StatusCode::FAILURE;
    }
    unsigned int sipmNumber = id.sipm() + 4*id.mat() + 16 * id.module();
    if ( sipmData[bankNumber].size() <= sipmNumber ) {
      error() << "Invalid SiPM number " << sipmNumber << " in bank " << bankNumber << " channelID " << id << endmsg;
      return StatusCode::FAILURE;
    }

    auto& data = sipmData[bankNumber][sipmNumber];
    
    if ( (id.module() > 0  && data.size() > FTRawBank::nbClusFFMaximum) ||
         (id.module() == 0 && data.size() > FTRawBank::nbClusMaximum) ) continue; 
    
    // one extra word for sipm number + nbClus
    if ( data.empty() ) data.push_back( sipmNumber << FTRawBank::sipmShift );
    
    data.push_back( ( id.channel()          << FTRawBank::cellShift ) |
                    ( cluster->fractionBit() << FTRawBank::fractionShift ) |
                    ( cluster->lastEdge()     << FTRawBank::sizeShift )
                    );
    ++data[0]; // counts the number of clusters (in the header)
    if ( msgLevel( MSG::VERBOSE ) ) {
      verbose() << format( "Bank%3d sipm%4d channel %4d frac %3.1f isLarge %1d lastEdge %1d code %4.4x",
                           bankNumber, sipmNumber, id.channel(), cluster->fraction(),
                           cluster->isLarge(), cluster->lastEdge(), data.back() ) << endmsg;
    }
  }
  
  
  //== Now build the banks: We need to put the 16 bits content into 32 bits words.
  for ( unsigned int iBank = 0; sipmData.size() > iBank; ++iBank ) {
    if( msgLevel( MSG::VERBOSE ) ) verbose() << "*** Bank " << iBank << endmsg;
    auto words = std::accumulate( sipmData[iBank].begin(), sipmData[iBank].end(),
                                  0, [](int w, std::vector<uint16_t>& d) {
                                    return w  + d.size();
                                  });
    std::vector<unsigned int> bank; bank.reserve((words+1)/2);
    boost::optional<unsigned int> buf;
    for ( const auto& pm : sipmData[iBank] ) {
      for ( const auto& data : pm ) {
        if (!buf) {
          buf = data;
        } else {
          bank.emplace_back( *buf | ( static_cast<unsigned int>(data) << 16 ) );
          buf = boost::none;
        }
      }
    }
    if (buf) bank.emplace_back( *buf) ;
    if ( msgLevel( MSG::VERBOSE ) ) {
      size_t offset = 0;
      for( const auto& d : bank ) {
        verbose() << format( "    at %5d data %8.8x", offset++, d ) << endmsg;
      }
    }
    event->addBank( iBank, LHCb::RawBank::FTCluster, codingVersion, bank );
  }
  return StatusCode::SUCCESS;
}
