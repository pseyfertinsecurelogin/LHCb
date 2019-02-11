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
#include "Event/FTCluster.h"
#include "Event/RawEvent.h"

#include <numeric>
#include <optional>

// local
#include "FTRawBankEncoder.h"
#include "FTRawBankParams.h"

#include "boost/container/static_vector.hpp"

constexpr static int s_nbBanks = FTRawBank::NbBanks;
constexpr static int s_nbLinksPerBank = FTRawBank::NbLinksPerBank;

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

  LHCb::FTClusters* clusters = get<LHCb::FTClusters>( m_inputLocation );
  if ( msgLevel( MSG::DEBUG) ) debug() << "Retrieved " << clusters->size() << " clusters" << endmsg;
  LHCb::RawEvent* event = getOrCreate<LHCb::RawEvent,LHCb::RawEvent>( m_outputLocation );

  // Incremented to deal with new numbering scheme
  int codingVersion = 6;

  //== create the array of arrays of vectors with the proper size...
  std::array<std::vector<uint16_t>, s_nbBanks> sipmData;
  std::array<uint32_t, s_nbBanks> headerData{};
  std::array<int,s_nbBanks*s_nbLinksPerBank> nClustersPerSipm = {0};
  for ( const auto& cluster : *clusters ) {
    if(cluster->isLarge() > 1) continue;

    LHCb::FTChannelID id = cluster->channelID();
    unsigned int bankNumber = m_readoutTool->bankNumber(id);
    unsigned int linkID  = (id - m_readoutTool->channelIDShift(bankNumber)) >> 7;
    auto& data = sipmData[bankNumber];
    unsigned int indexSipm = (bankNumber)*s_nbLinksPerBank+linkID;
    nClustersPerSipm[indexSipm]++;

    // Truncate clusters when maximum per SiPM is reached
    if ( (id.module() > 0  && nClustersPerSipm[indexSipm] > FTRawBank::nbClusFFMaximum) ||
         (id.module() == 0 && nClustersPerSipm[indexSipm] > FTRawBank::nbClusMaximum) ) {
      headerData[bankNumber] += ( 1u << linkID) ; // set the truncation bit
      continue;
    }

    data.push_back(( linkID                 << FTRawBank::linkShift) |
                   ( id.channel()           << FTRawBank::cellShift ) |
                   ( cluster->fractionBit() << FTRawBank::fractionShift ) |
                   ( (cluster->isLarge()>0) << FTRawBank::sizeShift )
                   );
    if ( msgLevel( MSG::VERBOSE ) ) {
      verbose() << format( "Bank%3d sipm%4d channel %4d frac %3.1f isLarge %2d code %4.4x",
                           bankNumber, linkID, id.channel(), cluster->fraction(),
                           cluster->isLarge(), data.back() ) << endmsg;
    }
  }

  //== Now build the banks: We need to put the 16 bits content into 32 bits words.
  for ( unsigned int iBank = 0; iBank < sipmData.size() ; ++iBank ) {
    if( msgLevel( MSG::VERBOSE ) ) verbose() << "*** Bank " << iBank << endmsg;
    auto words = sipmData[iBank].size();
    std::vector<unsigned int> bank; bank.reserve((words+1)/2 + 1);
    bank.emplace_back( headerData[iBank] ) ; // insert the header
    std::optional<unsigned int> buf;
    for ( const auto& cluster : sipmData[iBank] ) {
      if (!buf) {
        buf = cluster;
      } else {
        bank.emplace_back( *buf | ( static_cast<unsigned int>(cluster) << 16 ) );
        buf = {};
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
