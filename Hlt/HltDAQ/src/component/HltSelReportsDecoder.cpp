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
#include "boost/format.hpp"
#include <numeric>

#include "Event/HltDecReports.h"
#include "Event/HltObjectSummary.h"
#include "Event/HltSelReports.h"
#include "Event/RawEvent.h"

// bank structure
#include "HltDAQ/HltSelRepRBExtraInfo.h"
#include "HltDAQ/HltSelRepRBHits.h"
#include "HltDAQ/HltSelRepRBObjTyp.h"
#include "HltDAQ/HltSelRepRBStdInfo.h"
#include "HltDAQ/HltSelRepRBSubstr.h"
#include "HltDAQ/HltSelRepRawBank.h"

// local
#include "HltSelReportsDecoder.h"
#include "HltSelReportsWriter.h"

#include "Event/CaloCluster.h"
#include "Event/Particle.h"
#include "Event/RecVertex.h"
#include "Event/Track.h"
#include "pun.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : HltSelReportsDecoder
//
// 2008-08-01 : Tomasz Skwarnicki
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( HltSelReportsDecoder )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
HltSelReportsDecoder::HltSelReportsDecoder( const std::string& name, ISvcLocator* pSvcLocator )
    : HltRawBankMultiDecoder<LHCb::HltSelReports, LHCb::HltObjectSummary::Container>(
          name, pSvcLocator,
          KeyValue{"RawEventLocations", Gaudi::Functional::concat_alternatives( LHCb::RawEventLocation::Trigger,
                                                                                LHCb::RawEventLocation::Copied,
                                                                                LHCb::RawEventLocation::Default )},
          {KeyValue{"OutputHltSelReportsLocation", LHCb::HltSelReportsLocation::Default},
           KeyValue{"OutputHltObjectSummariesLocation", LHCb::HltSelReportsLocation::Default + "/Candidates"}} ) {}

//=============================================================================
// Initialize
//=============================================================================
StatusCode HltSelReportsDecoder::initialize() {
  auto sc = HltRawBankDecoderBase::initialize();
  if ( !sc ) return sc;

  // check that the 2nd handle has a key which is 1st handle key + "/Candidates"!!!
  //
  // TODO/FIXME: parse the property (yuk. It should have keys and values) , and
  //             get the key... then compare the keys.
  //
  // const auto& summaryLoc = getProperty("OutputHltObjectSummariesLocation");
  // const auto& selrepLoc = getProperty("OutputHltSelReportsLocation");
  // const auto& expected = selrepLoc.toString()+"/Candidates";
  // if (summaryLoc.toString().empty()) {
  //  setProperty("OutputHltObjectSummariesLocation", expected );
  //}
  // if ( symmaryLoc.toString() != expected ) {
  //  return Error("value of OutputHltObjectSummariesLocation not consistent", StatusCode::FAILURE);
  //}
  // Initialise the converter tool
  m_conv = tool<IReportConvert>( "ReportConvertTool", this );
  return m_conv ? StatusCode::SUCCESS : Error( "Unable to retrieve the Report converter tool" );
}
//=============================================================================
// Main execution
//=============================================================================
std::tuple<LHCb::HltSelReports, LHCb::HltObjectSummary::Container> HltSelReportsDecoder::
                                                                   operator()( const LHCb::RawEvent& rawEvent ) const {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // ----------------------------------------------------------
  // get the bank(s) from RawEvent
  // ----------------------------------------------------------
  auto hltselreportsRawBanks = selectRawBanks( rawEvent.banks( RawBank::HltSelReports ) );
  if ( hltselreportsRawBanks.empty() ) {
    throw GaudiException( " No HltSelReports RawBank in RawEvent. Not producing any HltSelReports. ", name(),
                          StatusCode::SUCCESS );
  }
  std::tuple<HltSelReports, HltObjectSummary::Container> outputs;
  auto&                                                  output = std::get<0>( outputs );
  // output container for Object Summaries
  auto& objectSummaries = std::get<1>( outputs );

  const RawBank* hltselreportsRawBank0 = hltselreportsRawBanks.front();

  // Tell the converter the version from the raw bank
  m_conv->setReportVersion( hltselreportsRawBank0->version() );

  // Check we know how to decode this version
  // If version is 99, this is the special case of the empty dummy bank
  if ( hltselreportsRawBank0->version() == 99 ) {

    // Get the list of ids and associated candidates
    auto             pBank99 = new unsigned int[hltselreportsRawBank0->size()];
    HltSelRepRawBank hltSelReportsBank99( pBank99 ); // bank assumes ownership!
    std::copy( hltselreportsRawBank0->begin<unsigned int>(), hltselreportsRawBank0->end<unsigned int>(), pBank99 );
    HltSelRepRBHits hitsSubBank99( hltSelReportsBank99.subBankFromID( HltSelRepRBEnums::SubBankIDs::kHitsID ) );

    // Populate map with line name and number of candidates
    LHCb::HltObjectSummary summary;

    auto                                                         tck_dummy = tck( rawEvent );
    bool                                                         settings = ( tck_dummy == 0 );
    GaudiUtils::VectorMap<int, HltRawBankDecoderBase::element_t> idmap_dummy;
    if ( !settings ) idmap_dummy = id2string( tck_dummy );

    unsigned int i = hitsSubBank99.seqBegin( 0 );
    while ( i < hitsSubBank99.seqEnd( 0 ) ) {
      int temp1 = hitsSubBank99.location()[i++];
      int temp2 = hitsSubBank99.location()[i++];
      if ( !settings )
        summary.addToInfo( idmap_dummy.find( temp1 )->second.str(), temp2 );
      else
        summary.addToInfo( std::to_string( temp1 ), temp2 );
    }

    output.insert( "0#Candidates", std::move( summary ) );
    Warning( "Version (99) indicates too many objects were requested to be saved. Returning debugging reports",
             StatusCode::SUCCESS, 20 )
        .ignore();
    return outputs;
  }
  if ( hltselreportsRawBank0->version() > kVersionNumber ) {
    Warning( " HltSelReports RawBank version is higher than expected. Will try to decode it anyway.",
             StatusCode::SUCCESS, 20 )
        .ignore();
  }
  // put the banks into the right order (in case the data was split across multiple banks...
  std::sort( std::begin( hltselreportsRawBanks ), std::end( hltselreportsRawBanks ),
             []( const RawBank* lhs, const RawBank* rhs ) {
               auto l = lhs->sourceID() & HltSelReportsWriter::kSourceID_MinorMask;
               auto r = rhs->sourceID() & HltSelReportsWriter::kSourceID_MinorMask;
               return l < r;
             } );
  // verify no duplicates...
  auto adj = std::adjacent_find( std::begin( hltselreportsRawBanks ), std::end( hltselreportsRawBanks ),
                                 []( const RawBank* lhs, const RawBank* rhs ) {
                                   auto l = lhs->sourceID() & HltSelReportsWriter::kSourceID_MinorMask;
                                   auto r = rhs->sourceID() & HltSelReportsWriter::kSourceID_MinorMask;
                                   return l == r;
                                 } );
  if ( adj != std::end( hltselreportsRawBanks ) ) {
    Error( " Duplicate sequential Source ID HltSelReports. Aborting decoder ", StatusCode::SUCCESS, 20 ).ignore();
    return outputs; // TODO: review whether to throw an exception instead
  }

  unsigned int nLastOne = hltselreportsRawBanks.back()->sourceID() & HltSelReportsWriter::kSourceID_MinorMask;
  if ( nLastOne + 1 != hltselreportsRawBanks.size() ) {
    Error( " Did not find the expected number of HltSelReports raw banks. Aborting decoder ", StatusCode::SUCCESS, 20 )
        .ignore();
    return outputs; // TODO: review whether to throw an exception instead
  }

  unsigned int bankSize =
      std::accumulate( std::begin( hltselreportsRawBanks ), std::end( hltselreportsRawBanks ), 0,
                       []( unsigned int s, const RawBank* bank ) {
                         return s + std::distance( bank->begin<unsigned int>(), bank->end<unsigned int>() );
                       } );

  if ( !bankSize ) {
    Warning( " No HltSelReports RawBank for requested SourceID in RawEvent. Quiting. ", StatusCode::SUCCESS, 10 )
        .ignore();
    return outputs; // TODO: review whether to throw an exception instead
  }

  // need to copy it to local array to concatenate  --- TODO: we could run a decompression such as LZMA at this point as
  // well...
  auto             pBank = new unsigned int[bankSize];
  HltSelRepRawBank hltSelReportsBank( pBank ); // bank assumes ownership
  std::accumulate( std::begin( hltselreportsRawBanks ), std::end( hltselreportsRawBanks ), pBank,
                   []( unsigned int* p, const LHCb::RawBank* bank ) {
                     return std::copy( bank->begin<unsigned int>(), bank->end<unsigned int>(), p );
                   } );

  HltSelRepRBHits      hitsSubBank( hltSelReportsBank.subBankFromID( HltSelRepRBEnums::SubBankIDs::kHitsID ) );
  HltSelRepRBObjTyp    objTypSubBank( hltSelReportsBank.subBankFromID( HltSelRepRBEnums::SubBankIDs::kObjTypID ) );
  HltSelRepRBSubstr    substrSubBank( hltSelReportsBank.subBankFromID( HltSelRepRBEnums::SubBankIDs::kSubstrID ) );
  HltSelRepRBStdInfo   stdInfoSubBank( hltSelReportsBank.subBankFromID( HltSelRepRBEnums::SubBankIDs::kStdInfoID ) );
  HltSelRepRBExtraInfo extraInfoSubBank(
      hltSelReportsBank.subBankFromID( HltSelRepRBEnums::SubBankIDs::kExtraInfoID ) );

  // ----------------------------------------- integrity checks -------------------------
  bool         errors  = false;
  bool         exInfOn = true;
  unsigned int ic;
  unsigned int nObj = objTypSubBank.numberOfObj();

  if ( bankSize < hltSelReportsBank.size() ) {
    Error( " HltSelReportsRawBank internally reported size " + std::to_string( hltSelReportsBank.size() ) +
               " less than bank size delivered by RawEvent " + std::to_string( bankSize ),
           StatusCode::SUCCESS, 100 )
        .ignore();
    errors = true;

  } else {

    ic = hltSelReportsBank.integrityCode();
    if ( ic ) {
      Error( " HltSelReportsRawBank fails integrity check with code " + std::to_string( ic ) + " " +
                 HltSelRepRBEnums::IntegrityCodesToString( ic ),
             StatusCode::SUCCESS, 100 )
          .ignore();
      errors = true;
    }

    ic = hitsSubBank.integrityCode();
    if ( ic ) {
      Error( " HltSelRepRBHits fails integrity check with code " + std::to_string( ic ) + " " +
                 HltSelRepRBEnums::IntegrityCodesToString( ic ),
             StatusCode::SUCCESS, 100 )
          .ignore();
      errors = true;
    }

    ic = objTypSubBank.integrityCode();
    if ( ic ) {
      Error( " HltSelRepRBObjTyp fails integrity check with code " + std::to_string( ic ) + " " +
                 HltSelRepRBEnums::IntegrityCodesToString( ic ),
             StatusCode::SUCCESS, 100 )
          .ignore();
      errors = true;
    }

    ic = substrSubBank.integrityCode();
    if ( ic ) {
      Error( " HltSelRepRBSubstr fails integrity check with code " + std::to_string( ic ) + " " +
                 HltSelRepRBEnums::IntegrityCodesToString( ic ),
             StatusCode::SUCCESS, 100 )
          .ignore();
      errors = true;
    }
    if ( nObj != substrSubBank.numberOfObj() ) {
      Error( " HltSelRepRBSubstr has number of objects " + std::to_string( substrSubBank.numberOfObj() ) +
                 " which is different than HltSelRepRBObjTyp " + std::to_string( nObj ),
             StatusCode::SUCCESS, 100 )
          .ignore();
      errors = true;
    }

    ic = stdInfoSubBank.integrityCode();
    if ( ic ) {
      Error( " HltSelRepRBStdInfo fails integrity check with code " + std::to_string( ic ) + " " +
                 HltSelRepRBEnums::IntegrityCodesToString( ic ),
             StatusCode::SUCCESS, 100 )
          .ignore();
      errors = true;
    }
    if ( nObj != stdInfoSubBank.numberOfObj() ) {
      Error( " HltSelRepRBStdInfo has number of objects " + std::to_string( stdInfoSubBank.numberOfObj() ) +
                 " which is different than HltSelRepRBObjTyp " + std::to_string( nObj ),
             StatusCode::SUCCESS, 100 )
          .ignore();
      errors = true;
    }

    ic = extraInfoSubBank.integrityCode();
    if ( ic ) {
      Error( " HltSelRepRBExtraInfo fails integrity check with code " + std::to_string( ic ) + " " +
                 HltSelRepRBEnums::IntegrityCodesToString( ic ),
             StatusCode::SUCCESS, 100 )
          .ignore();
      exInfOn = false; // the only non-fatal info corruption
    }
    if ( nObj != extraInfoSubBank.numberOfObj() ) {
      Error( " HltSelRepRBExtraInfo has number of objects " + std::to_string( extraInfoSubBank.numberOfObj() ) +
                 " which is different than HltSelRepRBObjTyp " + std::to_string( nObj ),
             StatusCode::SUCCESS, 100 )
          .ignore();
      exInfOn = false;
    }
  }

  if ( msgLevel( MSG::VERBOSE ) ) {
    // print created bank and subbanks inside
    verbose() << hltSelReportsBank << endmsg;
    verbose() << HltSelRepRBHits( hltSelReportsBank.subBankFromID( HltSelRepRBEnums::SubBankIDs::kHitsID ) ) << endmsg;
    verbose() << HltSelRepRBObjTyp( hltSelReportsBank.subBankFromID( HltSelRepRBEnums::SubBankIDs::kObjTypID ) )
              << endmsg;
    verbose() << HltSelRepRBSubstr( hltSelReportsBank.subBankFromID( HltSelRepRBEnums::SubBankIDs::kSubstrID ) )
              << endmsg;
    verbose() << HltSelRepRBStdInfo( hltSelReportsBank.subBankFromID( HltSelRepRBEnums::SubBankIDs::kStdInfoID ) )
              << endmsg;
    verbose() << HltSelRepRBExtraInfo( hltSelReportsBank.subBankFromID( HltSelRepRBEnums::SubBankIDs::kExtraInfoID ) )
              << endmsg;
  }

  if ( errors ) {
    hltSelReportsBank.deleteBank();
    throw GaudiException( "possible data corruption -- not producing any HltSelReports", name(), StatusCode::SUCCESS );
  }

  // -----------------------------------------------------------------
  // create object summaries
  // -----------------------------------------------------------------
  // TODO: check consistency of output location and source ID!!!!

  auto        mytck   = tck( rawEvent );
  const auto& idmap   = id2string( mytck );
  const auto& infomap = info2string( mytck );

  // put them in local vector until they are finished
  std::vector<HltObjectSummary*> objects;

  for ( unsigned int iObj = 0; iObj != nObj; ++iObj ) {

    auto hos = new HltObjectSummary();

    // =========== class ID
    hos->setSummarizedObjectCLID( objTypSubBank.next() );

    // =========== numerical info
    HltObjectSummary::Info infoPersistent;

    //           ============== standard
    HltSelRepRBStdInfo::StdInfo stdInfo = stdInfoSubBank.next();
    if ( stdInfo.size() ) switch ( hos->summarizedObjectCLID() ) {
      case LHCb::CLID_Track:
        m_conv->SummaryFromRaw( &infoPersistent, &stdInfo, LHCb::CLID_Track );
        break;
      case LHCb::CLID_RecVertex:
        m_conv->SummaryFromRaw( &infoPersistent, &stdInfo, LHCb::CLID_RecVertex );
        break;
      case LHCb::CLID_Vertex:
        m_conv->SummaryFromRaw( &infoPersistent, &stdInfo, LHCb::CLID_Vertex );
        break;
      case LHCb::CLID_RichPID:
        m_conv->SummaryFromRaw( &infoPersistent, &stdInfo, LHCb::CLID_RichPID );
        break;
      case LHCb::CLID_MuonPID:
        m_conv->SummaryFromRaw( &infoPersistent, &stdInfo, LHCb::CLID_MuonPID );
        break;
      case LHCb::CLID_ProtoParticle:
        m_conv->SummaryFromRaw( &infoPersistent, &stdInfo, LHCb::CLID_ProtoParticle );
        break;
      case LHCb::CLID_Particle:
        m_conv->SummaryFromRaw( &infoPersistent, &stdInfo, LHCb::CLID_Particle );
        break;
      case LHCb::CLID_RecSummary:
        m_conv->SummaryFromRaw( &infoPersistent, &stdInfo, LHCb::CLID_RecSummary );
        break;
      case LHCb::CLID_CaloCluster:
        m_conv->SummaryFromRaw( &infoPersistent, &stdInfo, LHCb::CLID_CaloCluster );
        break;
      case LHCb::CLID_CaloHypo:
        m_conv->SummaryFromRaw( &infoPersistent, &stdInfo, LHCb::CLID_CaloHypo );
        break;
      case 40:
        m_conv->SummaryFromRaw( &infoPersistent, &stdInfo, 40 );
        break;
      case 41:
        m_conv->SummaryFromRaw( &infoPersistent, &stdInfo, 41 );
        break;
      case 1: {
        infoPersistent.insert( "0#SelectionID", pun_to<float>( stdInfo[0] ) );
        if ( stdInfo.size() > 1 ) {
          int  id = (int)( pun_to<float>( stdInfo[1] ) + 0.1 );
          auto iselName = idmap.find( id );
          if ( iselName == std::end( idmap ) ) {
            Error( " Did not find string key for PV-selection-ID in trigger selection in storage id=" +
                       std::to_string( id ),
                   StatusCode::SUCCESS, 10 )
                .ignore();
            infoPersistent.insert( "10#Unknown", pun_to<float, unsigned int>( id ) );
          } else
            infoPersistent.insert( "10#" + iselName->second.str(), pun_to<float>( stdInfo[1] ) );
        }
        for ( unsigned int ipvkeys = 2; ipvkeys < stdInfo.size(); ++ipvkeys ) {
          infoPersistent.insert( "11#" + boost::str( boost::format( "%1$=08X" ) % ( ipvkeys - 2 ) ),
                                 pun_to<float>( stdInfo[ipvkeys] ) );
        }

      } break;
      default: {

        Warning( " StdInfo on unsupported class type " + std::to_string( hos->summarizedObjectCLID() ),
                 StatusCode::SUCCESS, 20 )
            .ignore();
        int e = 0;
        for ( const auto& i : stdInfo ) {
          infoPersistent.insert( "z#Unknown.unknown" + std::to_string( e++ ), pun_to<float>( i ) );
        }
      }
      }

    //           ============== extra

    if ( exInfOn ) {
      for ( const auto& i : extraInfoSubBank.next() ) {
        auto infos = infomap.find( i.first );
        if ( infos != std::end( infomap ) ) {
          infoPersistent.insert( infos->second, i.second );
        } else {
          Warning( " String key for Extra Info item in storage not found id=" + std::to_string( i.first ),
                   StatusCode::SUCCESS, 20 )
              .ignore();
        }
      }
    }
    hos->setNumericalInfo( infoPersistent );
    objects.push_back( hos );
  }

  // -----------------------------------------------------------------
  // reloop to add substructure or hits
  // -----------------------------------------------------------------
  for ( unsigned int iObj = 0; iObj != nObj; ++iObj ) {

    HltObjectSummary*&        hos = objects[iObj];
    HltSelRepRBSubstr::Substr sub = substrSubBank.next();

    if ( sub.first ) {
      // hits
      unsigned int              nSeq = hitsSubBank.numberOfSeq();
      std::vector<LHCb::LHCbID> hits;
      for ( const auto& iSeq : sub.second ) {
        if ( iSeq < nSeq ) {
          std::vector<LHCb::LHCbID> hitseq = hitsSubBank.sequence( iSeq );
          //   for bank version zero, first hit in the first sequence was corrupted ------
          //                   for odd number of sequences saved - omit this hit
          if ( iSeq == 0 && hltselreportsRawBank0->version() == 0 && nSeq % 2 == 1 ) { hitseq.erase( hitseq.begin() ); }
          // ------------------------- end fix --------------------------------------------
          if ( hitseq.size() ) { hits.insert( std::end( hits ), std::begin( hitseq ), std::end( hitseq ) ); }
        } else {
          Error( "Hit sequence index out of range", StatusCode::SUCCESS, 10 ).ignore();
        }
      }
      // Sort hits to make sure decode(write(X)) keeps the IDs ordered
      // (ordering is relied upon and enforced in HltSelReportsWriter)
      std::sort( std::begin( hits ), std::end( hits ) );
      hos->setLhcbIDs( hits );

    } else {
      // pointers
      SmartRefVector<LHCb::HltObjectSummary> thisSubstructure;
      for ( const auto& jObj : sub.second ) {
        if ( jObj < nObj ) {
          thisSubstructure.push_back( &( *( objects[jObj] ) ) );
        } else {
          Error( " Substructure object index out of range ", StatusCode::SUCCESS, 10 ).ignore();
        }
      }
      hos->setSubstructureExtended( thisSubstructure );
      if ( hltselreportsRawBank0->version() < 3 ) { hos->setSubstructure( thisSubstructure ); }
    }

    // give ownership to output
    objectSummaries.push_back( hos );
  }

  // clean-up
  hltSelReportsBank.deleteBank();

  // fix intermix of substructure needed by TisTos tool and by Turbo stream
  // substructure() returns only things needed by TisTos, substructureExetended() all things needed by Turbo
  // logic below is somewhat messy and depends on what was done in HltSelReportsMaker; this is the best we
  //      can do without restructuring rawbanks that don't distinguish between the two types

  //     don't waste time on older version banks which did not have extended info
  if ( hltselreportsRawBank0->version() > 2 ) {
    for ( unsigned int iObj = 0; iObj != nObj; ++iObj ) {

      HltObjectSummary*& hos = objects[iObj];
      if ( hos->summarizedObjectCLID() != LHCb::CLID_Particle ) {
        hos->setSubstructure( hos->substructureExtended() );
      } else {
        // for TisTos need to delete calo clusters from a particle that has a track in substructure

        const auto& sub = hos->substructureExtended();
        // look for a track among substracture
        auto e = std::find_if( sub.begin(), sub.end(), [&]( const LHCb::HltObjectSummary* obj ) {
          return obj && obj->summarizedObjectCLID() == LHCb::CLID_Track;
        } );
        if ( e != sub.end() ) // trackFound
        {
          for ( const auto& elem : sub ) {
            if ( !( elem.target() ) ) continue;
            auto id = elem.target()->summarizedObjectCLID();
            // add only if not calo cluster
            if ( id == LHCb::CLID_CaloCluster || id == LHCb::CLID_CaloHypo ) continue;
            hos->addToSubstructure( elem.target() );
          }
        } else {
          // no track, no worry
          hos->setSubstructure( hos->substructureExtended() );
        }
      }
    }
  }

  // ---------------------------------------------------------
  // ------- special container for selections ----------------
  // ---------------------------------------------------------

  for ( unsigned int iObj = 0; iObj != nObj; ++iObj ) {

    HltObjectSummary*& hos = objects[iObj];
    if ( hos->summarizedObjectCLID() != 1 ) continue;

    auto selName = std::end( idmap );
    auto i = std::find_if( std::begin( hos->numericalInfo() ), std::end( hos->numericalInfo() ),
                           []( const std::pair<std::string, double>& info ) { return info.first == "0#SelectionID"; } );
    if ( i != std::end( hos->numericalInfo() ) ) {
      int id  = (int)( i->second + 0.1 );
      selName = idmap.find( id );
    }
    if ( selName != std::end( idmap ) ) {

      // skip reports of the wrong type
      if ( !selName->second ) continue;

      // clone hos
      HltObjectSummary selSumOut;
      selSumOut.setSummarizedObjectCLID( hos->summarizedObjectCLID() );
      selSumOut.setNumericalInfo( hos->numericalInfo() );
      selSumOut.setSubstructure( hos->substructure() );

      // insert selection into the container
      if ( output.insert( selName->second, selSumOut ) == StatusCode::FAILURE ) {
        Error( "  Failed to add Hlt selection name " + std::string{selName->second} + " to its container ",
               StatusCode::SUCCESS, 10 )
            .ignore();
      }
    } else {
      Error( " Did not find string key for trigger selection in storage", StatusCode::SUCCESS, 50 ).ignore();
    }
  }

  if ( msgLevel( MSG::VERBOSE ) ) {

    verbose() << " ======= HltSelReports size= " << output.size() << endmsg;
    verbose() << output << endmsg;

    verbose() << " ======= HltObjectSummary container size= " << objectSummaries.size() << endmsg;
    for ( const auto& pHos : objectSummaries ) {
      verbose() << " key " << pHos->index();
      auto selby = output.selectedAsCandidateBy( pHos );
      if ( !selby.empty() ) {
        verbose() << " selectedAsCandidateBy= ";
        for ( const auto& i : selby ) verbose() << i << " ";
        auto pvInfo = output.pvSelectionNameAndKey( pHos );
        if ( pvInfo.second > -1 ) {
          verbose() << " pvSelectionName= " << pvInfo.first << " pvKey= " << pvInfo.second << " ";
        }
      }
      verbose() << *pHos << endmsg;
    }
  }
  return outputs;
}

//=============================================================================
