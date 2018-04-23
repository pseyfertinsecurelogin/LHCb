// Include files
#include "boost/optional.hpp"
#include "Event/FTCluster.h"
#include "Event/RawEvent.h"

#include <numeric>

// local
#include "FTRawBankEncoder.h"
#include "FTRawBankParams.h"

#include "boost/container/static_vector.hpp"

constexpr static int s_nbBanks = FTRawBank::NbBanks;
constexpr static int s_nbSipmPerTell40 = FTRawBank::NbSiPMPerTell40;

//-----------------------------------------------------------------------------
// Implementation file for class : FTRawBankEncoder
//
// 2012-05-11 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FTRawBankEncoder )

//=============================================================================
// Initializer of the ReadoutTool (taken from STCommonBase)
//=============================================================================

IFTReadoutTool* FTRawBankEncoder::getReadoutTool() const {
  m_readoutTool = this -> template tool<IFTReadoutTool>("FTReadoutTool","FTReadoutTool");
  return m_readoutTool;
}


IFTReadoutTool* FTRawBankEncoder::readoutTool() const {
  return m_readoutTool != 0 ? m_readoutTool : getReadoutTool();
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode FTRawBankEncoder::execute() {

  //typedef FastClusterContainer<LHCb::FTLiteCluster,int> FTLiteClusters;
  LHCb::FTClusters* clusters = get<LHCb::FTClusters>( m_inputLocation );
  if ( msgLevel( MSG::DEBUG) ) debug() << "Retrieved " << clusters->size() << " clusters" << endmsg;
  LHCb::RawEvent* event = getOrCreate<LHCb::RawEvent,LHCb::RawEvent>( m_outputLocation );

  // Incremented to deal with new numbering scheme
  int codingVersion = 5;

  //== create the array of arrays of vectors with the proper size...  
  //== The 0.4 is empirical
  //== TODO: reserve
  std::array<std::vector<uint16_t>, s_nbBanks> sipmData;
  std::array<int,s_nbBanks*s_nbSipmPerTell40> nClustersPerSipm = {0};
  for ( const auto& cluster : *clusters ) {
    if(cluster->isLarge() > 1) continue;
    
    LHCb::FTChannelID id = cluster->channelID();
    unsigned int bankNumber = readoutTool()->bankNumber(id);
    unsigned int linkID  = (id - readoutTool()->channelIDShift(bankNumber)) >> 7;
    auto& data = sipmData[bankNumber];
    unsigned int indexSipm = (bankNumber)*s_nbSipmPerTell40+linkID;
    nClustersPerSipm[indexSipm]++;
    //Todo
    if ( (id.module() > 0  && nClustersPerSipm[indexSipm] > FTRawBank::nbClusFFMaximum) ||
         (id.module() == 0 && nClustersPerSipm[indexSipm] > FTRawBank::nbClusMaximum) )
        continue; 
    
    // one extra word for sipm number + nbClus
    // So each data is [Sipm & number of clusters][clus1][clus2]...
    //    if ( data.empty() ) data.push_back( absSipmNumber << FTRawBank::sipmShift );
    
    data.push_back(( linkID                 << FTRawBank::sipmShift) |
                   ( id.channel()           << FTRawBank::cellShift ) |
                   ( cluster->fractionBit() << FTRawBank::fractionShift ) |
                   ( cluster->lastEdge()    << FTRawBank::sizeShift )                   
                   );
    if ( msgLevel( MSG::VERBOSE ) ) {
      verbose() << format( "Bank%3d sipm%4d channel %4d frac %3.1f isLarge %1d lastEdge %1d code %4.4x",
                           bankNumber, linkID, id.channel(), cluster->fraction(),
                           cluster->isLarge(), cluster->lastEdge(), data.back() ) << endmsg;
    }
  }
  
  //== Now build the banks: We need to put the 16 bits content into 32 bits words.
  for ( unsigned int iBank = 0; sipmData.size() > iBank; ++iBank ) {
    if( msgLevel( MSG::VERBOSE ) ) verbose() << "*** Bank " << iBank << endmsg;
    auto words = sipmData[iBank].size();
    
    std::vector<unsigned int> bank; bank.reserve((words+1)/2);
    boost::optional<unsigned int> buf;
    for ( const auto& cluster : sipmData[iBank] ) {
      if (!buf) {
        buf = cluster;
      } else {
        bank.emplace_back( *buf | ( static_cast<unsigned int>(cluster) << 16 ) );
        buf = boost::none;
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
