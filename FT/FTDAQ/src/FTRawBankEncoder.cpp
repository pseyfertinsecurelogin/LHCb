// Include files
#include "boost/optional.hpp"
#include "Event/FTCluster.h"
#include "Event/RawEvent.h"

#include <numeric>

// local
#include "FTRawBankEncoder.h"
#include "FTRawBankParams.h"

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
  int codingVersion = 4;

  //== create the array of arrays of vectors with the proper size...  
  //== The 0.4 is empirical
  std::array<std::vector<uint16_t>, s_nbBanks> sipmData;
  for (auto& data: sipmData)
    data.reserve((unsigned int) 0.4*48);
  std::vector<unsigned int> modules;
  std::vector<unsigned int> mats;
  std::vector<unsigned int> sipms;
  std::vector<unsigned int> relsipms;
  std::vector<unsigned int> abssipms;
  std::vector<unsigned int> channels;
  //Stores the number of clusters per sipm
  std::array<int,s_nbBanks*s_nbSipmPerTell40> nClustersPerSipm = {0};
  for ( const auto& cluster : *clusters ) {
    if(cluster->isLarge() > 1) continue;
    
    LHCb::FTChannelID id = cluster->channelID();
    unsigned int bankNumber = readoutTool()->bankNumber(id);
    unsigned int absSipmNumber  = id.sipm() + 4*id.mat() + 16 * id.module();
    unsigned int relSipmNumber  = absSipmNumber - 16 * readoutTool()->moduleShift(id);
    
    //Todo
    //    if ( sipmData[bankNumber].size() <= relSipmNumber ) {
    //      error() << "Invalid SiPM number " << relSipmNumber << " in bank " << bankNumber << " channelID " << id << endmsg;
    //      return StatusCode::FAILURE;
    //    }

    auto& data = sipmData[bankNumber];
    unsigned int indexSipm = (bankNumber)*s_nbSipmPerTell40+relSipmNumber;
    nClustersPerSipm[indexSipm]++;
    //Todo
    if ( (id.module() > 0  && nClustersPerSipm[indexSipm] > FTRawBank::nbClusFFMaximum) ||
         (id.module() == 0 && nClustersPerSipm[indexSipm] > FTRawBank::nbClusMaximum) )
      {
        warning() << "LoH: Skipping " <<  bankNumber << " " << relSipmNumber << " " << id.channel() << "(" << nClustersPerSipm[indexSipm] << ")" << endmsg;
        continue; 
      }
    
    // one extra word for sipm number + nbClus
    // So each data is [Sipm & number of clusters][clus1][clus2]...
    //    if ( data.empty() ) data.push_back( absSipmNumber << FTRawBank::sipmShift );
    
    data.push_back(( id.channel()           << FTRawBank::cellShift ) |
                   ( cluster->fractionBit() << FTRawBank::fractionShift ) |
                   ( cluster->lastEdge()    << FTRawBank::sizeShift ) |
                   ( absSipmNumber          << FTRawBank::clusSipmShift)
                   );
    warning() << "LoH: " << std::bitset<16>( ( id.channel()          << FTRawBank::cellShift ) |
                                           ( cluster->fractionBit() << FTRawBank::fractionShift ) |
                                           ( cluster->lastEdge()     << FTRawBank::sizeShift ) |
                                           ( absSipmNumber          << FTRawBank::clusSipmShift)) << endmsg;
    
    modules .push_back(id.module());
    mats    .push_back(id.mat());
    sipms   .push_back(id.sipm());
    relsipms   .push_back(relSipmNumber);
    abssipms   .push_back(absSipmNumber);
    channels.push_back(id.channel());
    //    ++data[0]; // counts the number of clusters (in the header)
    if ( msgLevel( MSG::VERBOSE ) ) {
      verbose() << format( "Bank%3d sipm%4d channel %4d frac %3.1f isLarge %1d lastEdge %1d code %4.4x",
                           bankNumber, absSipmNumber, id.channel(), cluster->fraction(),
                           cluster->isLarge(), cluster->lastEdge(), data.back() ) << endmsg;
    }
  }
  
  //== Now build the banks: We need to put the 16 bits content into 32 bits words.
  int i = 0;
  for ( unsigned int iBank = 0; sipmData.size() > iBank; ++iBank ) {
    if( msgLevel( MSG::VERBOSE ) ) verbose() << "*** Bank " << iBank << endmsg;
    //    auto words = std::accumulate( sipmData[iBank].begin(), sipmData[iBank].end(),0);//number of clusters
    auto words = sipmData[iBank].size();
    
    std::vector<unsigned int> bank; bank.reserve((words+1)/2);
    LHCb::FTChannelID bankID = readoutTool()->uniqueQuarter(iBank);
    warning() << "LoH FTRawBankEncoder: : " << iBank << " " << words << "(" << bankID.station() << "," << bankID.layer() << "," << bankID.quarter() << ")" << endmsg;
    boost::optional<unsigned int> buf;
    for ( const auto& cluster : sipmData[iBank] ) {
      warning() << "LoH DaComp: " << cluster << " " << modules[i] << " " << mats[i] << " " << sipms[i] << " " << relsipms[i] << " " << abssipms[i] << " " << channels[i] << endmsg;
      //      warning() << "LoH DaComp: " << channels[i] << endmsg;
      i++;
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
  warning() << "Done the encoding" << endmsg;
  return StatusCode::SUCCESS;
}
