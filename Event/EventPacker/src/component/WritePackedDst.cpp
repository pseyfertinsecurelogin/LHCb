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
#include "Event/ODIN.h"
#include "Event/PackedCaloCluster.h"
#include "Event/PackedCaloHypo.h"
#include "Event/PackedMuonPID.h"
#include "Event/PackedParticle.h"
#include "Event/PackedProtoParticle.h"
#include "Event/PackedRecVertex.h"
#include "Event/PackedRichPID.h"
#include "Event/PackedTrack.h"
#include "Event/PackedTwoProngVertex.h"
#include "Event/PackedVertex.h"
#include "Event/PackedWeightsVector.h"
#include "Event/ProcStatus.h"
#include "Event/RawEvent.h"
#include "Event/RecHeader.h"
#include "Event/RecSummary.h"
#include "Event/StandardPacker.h"
#include "GaudiAlg/MergingTransformer.h"
#include "GaudiKernel/IOpaqueAddress.h"
#include "GaudiKernel/compose.h"
#include "MDF/MDFHeader.h"
#include "MDF/RawEventHelpers.h"
#include "PackedBank.h"

//-----------------------------------------------------------------------------
// Implementation file for class : WritePackedDst
//
// 2008-12-01 : Olivier Callot
//-----------------------------------------------------------------------------

/** @class WritePackedDst WritePackedDst.h
 *  Write the packed DST as a MDF file
 *
 *  @author Olivier Callot
 *  @date   2008-12-01
 */

using DataObjectRefs = Gaudi::Functional::vector_of_const_<DataObject>;
class WritePackedDst : public Gaudi::Functional::MergingTransformer<LHCb::RawEvent( const DataObjectRefs& )> {

public:
  /// Standard constructor
  WritePackedDst( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode     initialize() override;                              ///< Algorithm initialization
  LHCb::RawEvent operator()( const DataObjectRefs& ) const override; ///< Algorithm execution

private:
  DataObjectReadHandle<LHCb::ODIN>     m_odin{LHCb::ODINLocation::Default, this};
  DataObjectReadHandle<LHCb::RawEvent> m_rawevent{LHCb::RawEventLocation::Default, this};
};

namespace {

#if defined( __GNUC__ ) && __GNUC__ < 10
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wparentheses"
#endif
  // gcc has a false positive warning -- see  https://godbolt.org/z/cyjtrr
  template <typename... Ts, typename F>
  bool match_and_invoke( const DataObject& obj, F&& f ) {
    const auto id = obj.clID();
    return ( ( id == Ts::classID() &&
               ( (void)std::invoke( std::forward<F>( f ), dynamic_cast<Ts const&>( obj ) ), true ) ) ||
             ... );
  }
#if defined( __GNUC__ ) && __GNUC__ < 10
#  pragma GCC diagnostic pop
#endif

  class PackedDst {
    LHCb::RawEvent* m_dst = nullptr;
    unsigned int    m_blobNumber{0};
    unsigned int    m_bankNb{0};

    void storeInBlob( PackedBank& pBnk, const void* data, unsigned int nb, unsigned int bSize ) {
      constexpr unsigned int MAXBANK = 65000;
      //== First, put the data in column order: x for all tracks, then y, then ...
      unsigned int     iSize   = bSize / sizeof( unsigned int );
      int              totSize = nb * bSize;
      const int*       start   = reinterpret_cast<const int*>( data );
      std::vector<int> temp;
      temp.reserve( totSize / sizeof( unsigned int ) );
      for ( unsigned int k = 0; k != iSize; ++k ) {
        for ( unsigned int l = 0; l != nb; ++l ) { temp.push_back( *( start + iSize * l + k ) ); }
      }
      start            = temp.data();
      unsigned int len = totSize;
      pBnk.addEntry( len, nb, m_blobNumber );
      while ( len > MAXBANK ) {
        LHCb::RawBank* bank = LHCb::RawEvent::createBank( m_blobNumber++, LHCb::RawBank::DstData, 0, MAXBANK, start );
        m_dst->adoptBank( bank, true );
        start += MAXBANK / sizeof( unsigned int );
        len -= MAXBANK;
      }
      if ( len > 0 ) {
        LHCb::RawBank* bank = LHCb::RawEvent::createBank( m_blobNumber++, LHCb::RawBank::DstData, 0, len, start );
        m_dst->adoptBank( bank, true );
      }
    }
    template <typename ValueType>
    void storeInBlob( PackedBank& pBnk, std::vector<ValueType> const& data ) {
      storeInBlob( pBnk, data.data(), data.size(), sizeof( ValueType ) );
    }
    void add( int version, PackedBank& bank ) {
      m_dst->addBank( m_bankNb++, LHCb::RawBank::DstBank, version, bank.data() );
    }

  public:
    PackedDst( LHCb::RawEvent* raw ) : m_dst{raw} {}

    template <typename... Args>
    decltype( auto ) addBank( Args&&... args ) {
      return m_dst->addBank( std::forward<Args>( args )... );
    }
    template <typename... Args>
    decltype( auto ) banks( Args&&... args ) {
      return m_dst->banks( std::forward<Args>( args )... );
    }
    template <typename... Args>
    decltype( auto ) adoptBank( Args&&... args ) {
      return m_dst->adoptBank( std::forward<Args>( args )... );
    }
    void add( const LHCb::PackedTracks& in ) {
      PackedBank bank( &in );
      storeInBlob( bank, in.tracks() );
      storeInBlob( bank, in.states() );
      storeInBlob( bank, in.ids() );
      storeInBlob( bank, in.extras() );
      add( in.version(), bank );
    }
    void add( const LHCb::PackedRichPIDs& in ) {
      PackedBank bank( &in );
      storeInBlob( bank, in.data() );
      add( in.version(), bank );
    }
    void add( const LHCb::PackedMuonPIDs& in ) {
      PackedBank bank( &in );
      storeInBlob( bank, in.data() );
      add( in.version(), bank );
    }
    void add( const LHCb::PackedCaloHypos& in ) {
      PackedBank bank( &in );
      storeInBlob( bank, in.hypos() );
      storeInBlob( bank, in.refs() );
      add( in.version(), bank );
    }
    void add( const LHCb::PackedProtoParticles& in ) {
      PackedBank bank( &in );
      storeInBlob( bank, in.protos() );
      storeInBlob( bank, in.refs() );
      storeInBlob( bank, in.extras() );
      add( in.version(), bank );
    }
    void add( const LHCb::PackedRecVertices& in ) {
      PackedBank bank( &in );
      storeInBlob( bank, in.vertices() );
      storeInBlob( bank, in.refs() );
      storeInBlob( bank, in.extras() );
      add( in.version(), bank );
    }
    void add( const LHCb::PackedTwoProngVertices& in ) {
      PackedBank bank( &in );
      storeInBlob( bank, in.vertices() );
      storeInBlob( bank, in.refs() );
      storeInBlob( bank, in.extras() );
      add( in.version(), bank );
    }
    void add( const LHCb::RecSummary& in ) {
      PackedBank bank( &in );
      bank.storeInt( in.summaryData().size() );
      for ( auto const& iS : in.summaryData() ) {
        bank.storeInt( iS.first );
        bank.storeInt( iS.second );
      }
      add( in.version(), bank );
    }
    void add( const LHCb::RecHeader& in ) {
      PackedBank   bank( &in );
      unsigned int evHigh = ( ( in.evtNumber() ) >> 32 ) & 0xFFFFFFFF;
      unsigned int evLow  = ( in.evtNumber() & 0xFFFFFFFF );
      bank.addEntry( evHigh, evLow, 0 ); // 0 is size of random seeds vector, for backward compatibility
      bank.storeString( in.applicationName() );
      bank.storeString( in.applicationVersion() );
      bank.storeInt( in.runNumber() );
      bank.storeInt( in.condDBTags().size() );
      for ( unsigned int kk = 0; in.condDBTags().size() != kk; ++kk ) {
        bank.storeString( in.condDBTags()[kk].first );
        bank.storeString( in.condDBTags()[kk].second );
      }
      add( in.version(), bank );
    }
    void add( const LHCb::PackedParticles& in ) {
      PackedBank bank( &in );
      storeInBlob( bank, in.data() );
      storeInBlob( bank, in.extra() );
      storeInBlob( bank, in.daughters() );
      add( in.version(), bank );
    }
    void add( const LHCb::PackedVertices& in ) {
      PackedBank bank( &in );
      storeInBlob( bank, in.data() );
      storeInBlob( bank, in.outgoingParticles() );
      add( in.version(), bank );
    }
    void add( const LHCb::PackedWeightsVector& in ) {
      PackedBank bank( &in );
      storeInBlob( bank, in.data() );
      storeInBlob( bank, in.weights() );
      add( in.version(), bank );
    }
    void add( const LHCb::PackedCaloClusters& in ) {
      PackedBank bank( &in );
      storeInBlob( bank, in.data() );
      storeInBlob( bank, in.entries() );
      add( in.version(), bank );
    }
    void add( const LHCb::ProcStatus& in ) {
      PackedBank bank( &in );
      bank.storeInt( in.aborted() );
      bank.storeInt( in.algs().size() );
      for ( unsigned int kk = 0; in.algs().size() != kk; ++kk ) {
        bank.storeString( in.algs()[kk].first );
        bank.storeInt( in.algs()[kk].second );
      }
      add( in.version(), bank );
    }
    void add( const LHCb::RawEvent& in ) {
      //== For RawEvent data, copy all banks to the output RawEvent, keep track of the banks
      PackedBank bank( &in );
      for ( unsigned int bnkTyp = LHCb::RawBank::L0Calo; LHCb::RawBank::LastType != bnkTyp; ++bnkTyp ) {
        for ( const LHCb::RawBank* bnk : in.banks( (LHCb::RawBank::BankType)bnkTyp ) ) {
          bank.storeInt( bnkTyp );
          bank.storeInt( bnk->sourceID() );
          addBank( bnk );
        }
      }
      add( in.version(), bank );
    }
  };
} // namespace

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
WritePackedDst::WritePackedDst( const std::string& name, ISvcLocator* pSvcLocator )
    : MergingTransformer{name, pSvcLocator, {"Containers", {}}, {"Output", "/Event/DAQ/DstEvent"}} {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode WritePackedDst::initialize() {
  return GaudiAlgorithm::initialize().andThen( [&] {
    info() << "Write containers ";
    for ( unsigned int i = 0; i != inputLocationSize(); ++i ) info() << " '" << inputLocation( i ) << "',";
    info() << endmsg;
  } );
}

//=============================================================================
// Main execution
//=============================================================================
LHCb::RawEvent WritePackedDst::operator()( const DataObjectRefs& inputs ) const {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  LHCb::RawEvent rawEvent;
  PackedDst      dst{&rawEvent};

  //== Write the opaque address of the Raw Event according to Markus specifications
  const LHCb::RawEvent* evt = m_rawevent.get();
  const IRegistry*      reg = evt->registry();
  if ( !reg ) {
    throw GaudiException( "No opaque address for " + m_rawevent.objKey() + " !!!", "WritePackedDst",
                          StatusCode::FAILURE );
  }
  const IOpaqueAddress* padd = reg->address();

  std::vector<unsigned int> data( 200 );
  unsigned int*             dataPt = data.data();
  *dataPt++                        = evt->classID();
  *dataPt++                        = padd->ipar()[0];
  *dataPt++                        = padd->ipar()[1];
  *dataPt++                        = padd->svcType();
  char* charPt                     = (char*)dataPt;
  strcpy( charPt, padd->par()[0].c_str() );
  charPt = charPt + strlen( padd->par()[0].c_str() ) + 1;
  strcpy( charPt, padd->par()[1].c_str() );
  charPt = charPt + strlen( padd->par()[1].c_str() ) + 1;
  strcpy( charPt, reg->identifier().c_str() );
  charPt   = charPt + strlen( reg->identifier().c_str() + 7 ) + 1; // Omit '/Event' !
  int bLen = charPt - (char*)data.data();
  data.resize( ( bLen + 3 ) / 4 );
  dst.addBank( 0, LHCb::RawBank::DstAddress, 0, data );

  for ( const DataObject& myObj : inputs ) {
    if ( !match_and_invoke<LHCb::PackedTracks, LHCb::PackedRichPIDs, LHCb::PackedMuonPIDs, LHCb::PackedCaloHypos,
                           LHCb::PackedProtoParticles, LHCb::PackedRecVertices, LHCb::PackedTwoProngVertices,
                           LHCb::RecSummary, LHCb::RecHeader, LHCb::PackedParticles, LHCb::PackedVertices,
                           LHCb::PackedWeightsVector, LHCb::PackedCaloClusters, LHCb::ProcStatus, LHCb::ODIN,
                           LHCb::RawEvent>( myObj, Gaudi::overload( [&dst]( const auto& obj ) { dst.add( obj ); },
                                                                    [&]( const LHCb::ODIN& ) {
                                                                      //== Get the ODIN from raw data, and copy it
                                                                      // instead!
                                                                      const auto& odinBanks =
                                                                          evt->banks( LHCb::RawBank::ODIN );
                                                                      if ( !odinBanks.empty() )
                                                                        dst.addBank( *odinBanks.begin() );
                                                                    } ) ) )
      throw GaudiException( "--- Unknown class ID " + std::to_string( myObj.clID() ) + " for container " +
                                myObj.name() + " --",
                            __FUNCTION__, StatusCode::FAILURE );
  }

  //== Write the DAQ status bank
  int    len = 0;
  size_t i;
  for ( len = 0, i = LHCb::RawBank::L0Calo; i < LHCb::RawBank::LastType; ++i ) {
    const auto& banks = dst.banks( LHCb::RawBank::BankType( i ) );
    len               = std::accumulate( banks.begin(), banks.end(), len,
                           []( int l, const LHCb::RawBank* bnk ) { return l + bnk->totalSize(); } );
  }
  LHCb::RawBank* hdrBank = LHCb::RawEvent::createBank(
      0, LHCb::RawBank::DAQ, DAQ_STATUS_BANK, sizeof( LHCb::MDFHeader ) + sizeof( LHCb::MDFHeader::Header1 ), 0 );
  LHCb::MDFHeader* hdr = (LHCb::MDFHeader*)hdrBank->data();
  hdr->setChecksum( 0 );
  hdr->setCompression( 0 );
  hdr->setHeaderVersion( 3 );
  hdr->setSpare( 0 );
  hdr->setDataType( LHCb::MDFHeader::BODY_TYPE_BANKS );
  hdr->setSubheaderLength( sizeof( LHCb::MDFHeader::Header1 ) );
  hdr->setSize( len );
  LHCb::MDFHeader::SubHeader h = hdr->subHeader();

  unsigned int      trMask[] = {~0u, ~0u, ~0u, ~0u};
  const LHCb::ODIN* odin     = m_odin.get();
  h.H1->setTriggerMask( trMask );
  h.H1->setRunNumber( odin->runNumber() );
  h.H1->setOrbitNumber( odin->orbitNumber() );
  h.H1->setBunchID( odin->bunchId() );
  dst.adoptBank( hdrBank, true );

  return rawEvent;
}

//=============================================================================

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( WritePackedDst )

//=============================================================================
