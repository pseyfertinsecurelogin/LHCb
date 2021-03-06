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
#include <cctype>
#include <memory>
#include <numeric>

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
#include "HltSelReportsWriter.h"
#include "LHCbMath/bit_cast.h"

using namespace LHCb;

namespace {
  void sanityCheck( const HltSelRepRBStdInfo& stdInfo ) {
    auto sizeStored = ( stdInfo.location()[0] >> 16 );
    if ( sizeStored != 0xFFFFu && sizeStored != stdInfo.size() ) {
      throw GaudiException( "Inconsistent HltSelRepRBStdInfo bank", "HltSelReportsWriter", StatusCode::FAILURE );
    }
  }

  bool isStdInfo( const std::string& s ) {
    // check for [0-9]+# at the start, i.e. first non-numerical item is a '#', and it cannot be the first character
    auto i = std::find_if( std::begin( s ), std::end( s ), []( const char& c ) { return std::isdigit( c ) == 0; } );
    return i != std::begin( s ) && i != std::end( s ) && *i == '#';
  }

  static const Gaudi::StringKey InfoID{"InfoID"};

  template <typename Input1, typename Input2, typename Output1, typename Output2, typename Output3>
  void decompose( Input1 first1, Input1 last1, Input2 first2, Input2 last2, Output1 result1, Output2 result2,
                  Output3 result3 ) {
    while ( first1 != last1 && first2 != last2 ) {
      if ( *first1 < *first2 ) {
        *result1++ = *first1++;
      } else if ( *first2 < *first1 ) {
        *result2++ = *first2++;
      } else {
        *result3++ = *first1++;
        ++first2; // skip common value in set2
      }
    }
    std::copy( first1, last1, result1 );
    std::copy( first2, last2, result2 );
  }

  template <typename OrderedSequence>
  struct intersects {
    OrderedSequence s;

    template <typename OrderedSequence2>
    bool operator()( const OrderedSequence2& s2 ) const {
      auto first1 = std::begin( s );
      auto last1  = std::end( s );
      auto first2 = std::begin( s2 );
      auto last2  = std::end( s2 );
      while ( first1 != last1 && first2 != last2 ) {
        if ( *first1 < *first2 )
          ++first1;
        else if ( *first2 < *first1 )
          ++first2;
        else
          return true;
      }
      return false;
    }
  };

  template <typename OrderedSequence>
  intersects<OrderedSequence> intersects_with( OrderedSequence&& s ) {
    return {std::forward<OrderedSequence>( s )};
  }

  //=============================================================================
  // this function moves (the relevant parts of) set2 into sequences
  template <typename T>
  void addToSequences( typename T::value_type&& set2, T& sequences ) {
    typename T::difference_type offset{0};
    while ( !set2.empty() ) {
      auto iSet1 =
          std::find_if( std::next( std::begin( sequences ), offset ), std::end( sequences ), intersects_with( set2 ) );
      if ( iSet1 == std::end( sequences ) ) {
        // no overlap, add everything in one shot...
        sequences.emplace_back( std::move( set2 ) );
      } else {
        offset = std::distance( std::begin( sequences ), iSet1 ); // start of the next iteration
        typename T::value_type set1p, set2p, setin;
        decompose( std::begin( *iSet1 ), std::end( *iSet1 ), std::begin( set2 ), std::end( set2 ),
                   std::inserter( set1p, std::end( set1p ) ),               // subset of set1, not in set2
                   std::inserter( set2p, std::end( set2p ) ),               // subset of set2, not in set1
                   std::inserter( setin, std::end( setin ) ) );             // intersection of set1 and set2
        if ( iSet1->size() != setin.size() ) *iSet1 = std::move( setin );   // shrink set1 to its intersection with set2
        if ( !set1p.empty() ) sequences.emplace_back( std::move( set1p ) ); // add the non-overlapping part of set1
        set2 = std::move( set2p );                                          // shrink set2 to its non-overlapping part
      }
    }
  }

  /// for sorting ranked selections
  // static bool sortByCLID( const LHCb::HltObjectSummary*  elem1, const LHCb::HltObjectSummary*  elem2);
  class sortByCLID {
  public:
    bool operator()( const LHCb::HltObjectSummary* elem1, const LHCb::HltObjectSummary* elem2 ) const {
      if ( ( elem1 == nullptr ) || ( elem2 == nullptr ) || elem1 == elem2 ) return false;
      return elem1->summarizedObjectCLID() > elem2->summarizedObjectCLID();
    }
  };

  constexpr sortByCLID sortByCLID_{};
} // namespace

//-----------------------------------------------------------------------------
// Implementation file for class : HltSelReportsWriter
//
// 2008-07-25 : Tomasz Skwarnicki
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( HltSelReportsWriter )

//=============================================================================
// Initialization
//=============================================================================
StatusCode HltSelReportsWriter::initialize() {
  StatusCode sc = HltSelReportsWriterBase_t::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;                         // error printed already by GaudiAlgorithm

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  if ( !m_useTCK ) {
    m_hltANNSvc = service( "HltANNSvc" );
  } else {
    m_tckANNSvc = service( "TCKANNSvc" );
  }

  if ( m_sourceID > kSourceID_Max || m_sourceID < 0 ) {
    return Error( "Illegal SourceID specified; maximal allowed value is 7", StatusCode::FAILURE );
  }
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
void HltSelReportsWriter::operator()( LHCb::HltDecReports const& decreps, LHCb::HltSelReports const& selreps,
                                      LHCb::HltObjectSummary::Container const& objectSummaries,
                                      LHCb::RawEvent const&                    rawevt ) const {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // const HltSelReportsWriter::NameToNumberMap* infoIDMap = nullptr;
  unsigned int const tck = m_useTCK ? decreps.configuredTCK() : 0;

  const auto infoIDMap = m_useTCK ? &tckANNSvcMap( tck, InfoID ) : nullptr;

  // protection against too many objectSummaries to store
  if ( objectSummaries.size() > 0xFFFFL ) {
    error() << "Too many HltObjectSummaries to store " << objectSummaries.size()
            << " HltSelReports RawBank cannot be created " << endmsg;
    return;
  }

  // --------------------------------------------------------------------------------------
  // ------------ create hit sequence bank -------------------------------------------------
  // --------------------------------------------------------------------------------------

  // lhcbid sequences - recognize common hit sequences
  using LhcbidSequences = std::vector<std::vector<unsigned int>>;
  LhcbidSequences lhcbidSequences;
  for ( const auto& hos : objectSummaries ) {
    const auto&                 ids = hos->lhcbIDs();
    LhcbidSequences::value_type thisIDset;
    thisIDset.reserve( ids.size() );
    std::transform( std::begin( ids ), std::end( ids ), std::inserter( thisIDset, std::end( thisIDset ) ),
                    []( const LHCb::LHCbID& id ) { return id.lhcbID(); } );
    // Make sure the LHCbIDs are sorted or else the binary_search called
    // later does not work! (when connecting substructures with ids)
    // TODO check if its needed
    std::sort( std::begin( thisIDset ), std::end( thisIDset ) );

    addToSequences( std::move( thisIDset ), lhcbidSequences );
  }

  auto nHits = std::accumulate( std::begin( lhcbidSequences ), std::end( lhcbidSequences ), 0u,
                                []( unsigned int n, LhcbidSequences::const_reference s ) { return n + s.size(); } );

  if ( lhcbidSequences.size() / 2 + 1 + nHits > 0xFFFFL ) {
    error() << "Too many hits or hit-sequences to store hits=" << std::to_string( nHits )
            << " seq=" << std::to_string( lhcbidSequences.size() ) << " HltSelReports RawBank cannot be created "
            << endmsg;
    return;
  }

  HltSelRepRBHits hitsSubBank;
  hitsSubBank.initialize( lhcbidSequences.size(), nHits );
  std::copy( std::begin( lhcbidSequences ), std::end( lhcbidSequences ), std::back_inserter( hitsSubBank ) );

  // --------------------------------------------------------------------------------------
  //  ---------------- in storage banks are ordered by summarizedClassCLID ----------------
  // -------------------------------- sort them -------------------------------------------

  std::vector<const HltObjectSummary*> sortedHosPtrs( objectSummaries.begin(), objectSummaries.end() );
  std::sort( std::begin( sortedHosPtrs ), std::end( sortedHosPtrs ), sortByCLID_ );
  // inverse mapping
  std::vector<unsigned int> fromIndexToNewIndex( sortedHosPtrs.size() );
  for ( size_t i = 0; i != sortedHosPtrs.size(); ++i ) { fromIndexToNewIndex[sortedHosPtrs[i]->index()] = i; }

  // --------------------------------------------------------------------------------------
  //   create substructure bank -------------------------------------------------
  //   and object type bank
  //   and extra  info bank
  //   and std    info bank
  // --------------------------------------------------------------------------------------

  HltSelRepRBSubstr    substrSubBank;
  HltSelRepRBObjTyp    objTypSubBank;
  HltSelRepRBExtraInfo extraInfoSubBank;
  HltSelRepRBStdInfo   stdInfoSubBank;

  objTypSubBank.initialize();
  substrSubBank.initialize();
  // some banks require initialization with size info
  unsigned int nExtraInfo = 0;
  unsigned int nStdInfo   = 0;
  for ( const auto& hos : sortedHosPtrs ) {
    for ( const auto& i : hos->numericalInfo() ) {
      if ( isStdInfo( i.first ) ) {
        ++nStdInfo;
      } else {
        ++nExtraInfo;
      }
    }
  }
  bool saveExtraInfo = extraInfoSubBank.initialize( sortedHosPtrs.size(), nExtraInfo );
  if ( !saveExtraInfo ) {
    error() << "ExtraInfoSubBank too large to store nObj=" + std::to_string( sortedHosPtrs.size() ) +
                   " nInfo=" + std::to_string( nExtraInfo ) + " No Extra Info will be saved!"
            << endmsg;
    if ( !extraInfoSubBank.initialize( sortedHosPtrs.size(), 0 ) ) {
      error() << "Cannot save even empty ExtraInfoSubBank  - expect a fatal error" << endmsg;
    }
  }
  bool saveStdInfo = stdInfoSubBank.initialize( sortedHosPtrs.size(), nStdInfo );
  if ( !saveStdInfo ) {
    error() << "StdInfoSubBank too large to store nObj=" + std::to_string( sortedHosPtrs.size() ) +
                   " nInfo=" + std::to_string( nStdInfo ) + " No Std Info will be saved!"
            << endmsg;
    // save only selection IDs
    nStdInfo = std::accumulate(
        std::begin( sortedHosPtrs ), std::end( sortedHosPtrs ), 0, []( int n, const HltObjectSummary* hos ) {
          if ( hos->summarizedObjectCLID() == 1 ) {
            const auto& ni = hos->numericalInfo();
            n += std::count_if( std::begin( ni ), std::end( ni ),
                                []( HltObjectSummary::Info::const_reference i ) { return isStdInfo( i.first ); } );
          }
          return n;
        } );
    if ( !stdInfoSubBank.initialize( sortedHosPtrs.size(), nStdInfo ) ) {
      error() << "Cannot save even selectionIDs - expect a fatal error" << endmsg;
    }
  }

  // associate objects with hit sequences, or do simple substructure
  // do info banks, and object type bank
  for ( const auto& hos : sortedHosPtrs ) {
    objTypSubBank.push_back( hos->summarizedObjectCLID() );

    HltSelRepRBExtraInfo::ExtraInfo extraInfo;
    HltSelRepRBStdInfo::StdInfo     stdInfo;

    for ( const auto& i : hos->numericalInfo() ) {

      if ( isStdInfo( i.first ) ) {

        if ( saveStdInfo || ( hos->summarizedObjectCLID() == 1 ) ) {
          // push floats as ints (allows for possible compression in future versions)
          stdInfo.push_back( bit_cast<unsigned int>( i.second ) );
        }

      } else if ( saveExtraInfo ) {
        // convert string-id to a short
        auto j = ( !m_useTCK ) ? optionalValue( InfoID, i.first ) : optionalFind( *infoIDMap, i.first );
        if ( j ) {
          extraInfo.emplace_back( *j, i.second );
        } else {
          // this is very unexpected but shouldn't be fatal
          error() << "Int key for string info key=" + i.first + " not found " << endmsg;
        }
      }
    }

    stdInfoSubBank.push_back( stdInfo );
    extraInfoSubBank.push_back( extraInfo );

    // substructure or hit sequence link vector
    HltSelRepRBSubstr::Substrv svect;
    unsigned int               sHitType = 0;

    const auto& subs    = hos->substructure();
    const auto& subsExt = hos->substructureExtended();
    if ( !subs.empty() || !subsExt.empty() ) {
      std::transform( std::begin( subs ), std::end( subs ), std::back_inserter( svect ),
                      [&]( const SmartRef<LHCb::HltObjectSummary>& i ) { return fromIndexToNewIndex[i->index()]; } );
      if ( !subsExt.empty() ) { // add Extended items if not in already
        for ( const auto& i : subsExt ) {
          if ( subs.end() == std::find( subs.begin(), subs.end(), i ) ) {
            svect.push_back( fromIndexToNewIndex[i->index()] );
          }
        }
      }
    } else if ( !hos->lhcbIDs().empty() ) {
      // hits
      sHitType = 1;
      unsigned int iSeqID( 0 );
      for ( const auto& hitset : lhcbidSequences ) {
        if ( std::binary_search( std::begin( hos->lhcbIDs() ), std::end( hos->lhcbIDs() ), LHCbID{hitset.front()} ) )
          svect.push_back( iSeqID );
        ++iSeqID;
      }
    }

    if ( !substrSubBank.push_back( {sHitType, std::move( svect )} ) ) {
      // Exceeded maximal size of substructure-subbank, return debugging bank.

      hitsSubBank.deleteBank();
      objTypSubBank.deleteBank();
      substrSubBank.deleteBank();
      stdInfoSubBank.deleteBank();
      extraInfoSubBank.deleteBank();

      Gaudi::StringKey HltID;
      if ( m_sourceID == 1 )
        HltID = "Hlt1SelectionID";
      else
        HltID = "Hlt2SelectionID";

      const auto hltIDMap = m_useTCK ? &tckANNSvcMap( tck, HltID ) : nullptr;

      auto SelNames = selreps.selectionNames();

      std::vector<unsigned int> vect;
      for ( auto n : SelNames ) {
        auto j = ( !m_useTCK ) ? optionalValue( HltID, n ) : optionalFind( *hltIDMap, n );
        vect.push_back( *j );
        vect.push_back( selreps.selReport( n )->substructure().size() );
      }
      HltSelRepRBHits hitsSubBank_99;
      hitsSubBank_99.initialize( 1, vect.size() );
      hitsSubBank_99.push_back( vect );

      HltSelRepRawBank hltSelReportsBank_99;
      hltSelReportsBank_99.push_back( HltSelRepRBEnums::SubBankIDs::kHitsID, hitsSubBank_99.location(),
                                      hitsSubBank_99.size() );
      hitsSubBank_99.deleteBank();

      std::vector<unsigned int> bankBody_99( &( hltSelReportsBank_99.location()[0] ),
                                             &( hltSelReportsBank_99.location()[hltSelReportsBank_99.size()] ) );
      // TODO handle the construction of the extended raw event differently, we would like to have a const tes in the
      // end. see https://its.cern.ch/jira/browse/LBCORE-1742
      const_cast<LHCb::RawEvent&>( rawevt ).addBank( m_sourceID, RawBank::HltSelReports, 99, bankBody_99 );
      hltSelReportsBank_99.deleteBank();

      error() << "Exceeded maximal size of substructure-subbank. HltSelReports RawBank cannot be created, instead "
                 "returning debugging bank"
              << endmsg;
    }
  }

  // ----------- create HltSelReports bank body and insert subbanks to it
  HltSelRepRawBank hltSelReportsBank;

  // order of sub-banks in the main bank is arbitrary

  //    lhcbids
  hltSelReportsBank.push_back( HltSelRepRBEnums::SubBankIDs::kHitsID, hitsSubBank.location(), hitsSubBank.size() );
  hitsSubBank.deleteBank();

  //    object types
  objTypSubBank.saveSize();
  hltSelReportsBank.push_back( HltSelRepRBEnums::SubBankIDs::kObjTypID, objTypSubBank.location(),
                               objTypSubBank.size() );
  objTypSubBank.deleteBank();

  // their substructure
  substrSubBank.saveSize();
  hltSelReportsBank.push_back( HltSelRepRBEnums::SubBankIDs::kSubstrID, substrSubBank.location(),
                               substrSubBank.size() );
  substrSubBank.deleteBank();

  sanityCheck( stdInfoSubBank );
  // std info
  stdInfoSubBank.saveSize();
  hltSelReportsBank.push_back( HltSelRepRBEnums::SubBankIDs::kStdInfoID, stdInfoSubBank.location(),
                               stdInfoSubBank.size() );
  stdInfoSubBank.deleteBank();

  // extra info
  hltSelReportsBank.push_back( HltSelRepRBEnums::SubBankIDs::kExtraInfoID, extraInfoSubBank.location(),
                               extraInfoSubBank.size() );
  extraInfoSubBank.deleteBank();

  // insert the bank into the RawEvent
  hltSelReportsBank.saveSize();

  // delete any previously inserted sel reports with the same major sourceID
  const auto& hltselreportsRawBanks = rawevt.banks( RawBank::HltSelReports );
  for ( const auto& b : LHCb::RawBank::ConstVector{hltselreportsRawBanks.begin(), hltselreportsRawBanks.end()} ) {
    auto sourceID = b->version() > 1 ? ( b->sourceID() >> kSourceID_BitShift ) : kSourceID_Hlt;
    if ( m_sourceID != sourceID ) continue;
    // TODO handle the construction of the extended raw event differently, we would like to have a const tes in the end.
    // see https://its.cern.ch/jira/browse/LBCORE-1742
    const_cast<LHCb::RawEvent&>( rawevt ).removeBank( b );
    warning() << " Deleted previously inserted HltSelReports bank " << endmsg;
  }

  // RawBank is limited in size to 65535 bytes i.e. 16383 words; be conservative cut it off at a smaller limit.
  // Save in chunks if exceed this size.
  int nBank = 1 + ( hltSelReportsBank.size() == 0 ? 0 : ( hltSelReportsBank.size() - 1 ) / 16300 );
  if ( nBank > kSourceID_MinorMask ) {
    // delete the main bank
    hltSelReportsBank.deleteBank();
    error() << "HltSelReports too long to save" << endmsg;
  }
  for ( int iBank = 0; iBank < nBank; ++iBank ) {
    int ioff  = iBank * 16300;
    int isize = hltSelReportsBank.size() - ioff;
    if ( isize > 16300 ) isize = 16300;
    // TODO: can we avoid making a copy into bankBody??? ( call adoptBank( createBank( ... ) ) ? )
    std::vector<unsigned int> bankBody( &( hltSelReportsBank.location()[ioff] ),
                                        &( hltSelReportsBank.location()[ioff + isize] ) );
    int                       sourceID = iBank | ( m_sourceID << kSourceID_BitShift );
    // TODO handle the construction of the extended raw event differently, we would like to have a const tes in the end.
    // see https://its.cern.ch/jira/browse/LBCORE-1742
    const_cast<LHCb::RawEvent&>( rawevt ).addBank( sourceID, RawBank::HltSelReports, kVersionNumber, bankBody );
  }
  if ( nBank > 1 ) {
    warning() << "HltSelReports is huge. Saved in " + std::to_string( nBank ) + " separate RawBanks " << endmsg;
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

  // delete the main bank
  hltSelReportsBank.deleteBank();
}

const HltSelReportsWriter::NameToNumberMap& HltSelReportsWriter::tckANNSvcMap( unsigned int            tck,
                                                                               const Gaudi::StringKey& major ) const {
  const auto key = std::pair{tck, major};

  auto itbl = m_infoTable.find( key );
  if ( LIKELY( itbl != std::end( m_infoTable ) ) ) { return itbl->second; }

  auto& map = m_infoTable[key];
  for ( auto p : m_tckANNSvc->i2s( tck, InfoID ) ) { map.insert( {p.second, p.first} ); }
  return map;
}
