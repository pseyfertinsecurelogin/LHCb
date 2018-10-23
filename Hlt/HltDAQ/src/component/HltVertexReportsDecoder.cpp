/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files
#include <memory>

#include "Event/HltVertexReports.h"
#include "Event/RawEvent.h"

// local
#include "HltVertexReportsDecoder.h"
#include "HltVertexReportsWriter.h"
#include "pun.h"

namespace {
    static const std::vector<std::string> DefaultLabels = { "PV3D", "ProtoPV3D" };

    template <typename StringContainer>
    std::vector<std::string> prefix(const std::string& prefix, const StringContainer& strs ) {
        std::vector<std::string> vs; vs.reserve(strs.size());
        std::transform( strs.begin(), strs.end(),
                        std::back_inserter(vs),
                        [&](const std::string& s) {  return prefix + "/" + s; } );
        return vs;
    }

    template <typename Container>
    int index_(const Container& c, typename Container::const_reference v ) {
      auto i = std::find(c.begin(),c.end(),v);
      return i!=c.end() ? std::distance( c.begin(), i) : -1 ;
    }

}

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : HltVertexReportsDecoder
//
// 2008-08-05 : Tomasz Skwarnicki
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( HltVertexReportsDecoder )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
HltVertexReportsDecoder::HltVertexReportsDecoder( const std::string& name,
                                                  ISvcLocator* pSvcLocator )
: HltRawBankSplittingDecoder<LHCb::VertexBase::Container>( name , pSvcLocator,
                            KeyValue{ "RawEventLocations", Gaudi::Functional::concat_alternatives(LHCb::RawEventLocation::Trigger,
                                                                                                    LHCb::RawEventLocation::Copied,
                                                                                                    LHCb::RawEventLocation::Default )},
                            KeyValues{ "OutputHltVertexLocations", prefix( LHCb::HltVertexReportsLocation::Default, DefaultLabels ) }
                            )
{
  declareProperty("Decode", m_decode = DefaultLabels, "List of containers to decode" );
  // TODO: verify/guarantee the match between 'Decode' and 'OutputHltVertexReportsLocation'...
}

//=============================================================================
// Main execution
//=============================================================================
Gaudi::Functional::vector_of_optional_<VertexBase::Container> HltVertexReportsDecoder::operator()(const LHCb::RawEvent& rawEvent) const
{
  Gaudi::Functional::vector_of_optional_<VertexBase::Container> output( m_decode.size() );

  auto hltvertexreportsRawBanks = selectRawBanks( rawEvent.banks(RawBank::HltVertexReports) );

  if( hltvertexreportsRawBanks.empty() ){
    throw GaudiException( " No HltVertexReports RawBank for requested SourceID in RawEvent. Quiting. ",
                          name(),
                          StatusCode::SUCCESS );
  }
  if( hltvertexreportsRawBanks.size() != 1 ){
    Warning(" More then one HltVertexReports RawBanks for requested SourceID in RawEvent. Will only process the first one. " ,StatusCode::SUCCESS, 20 ).ignore();
  }

  const RawBank* hltvertexreportsRawBank = hltvertexreportsRawBanks.front();

  if( hltvertexreportsRawBank->magic() != RawBank::MagicPattern ){
    throw GaudiException(" HltVertexReports RawBank has wrong magic number. Return without decoding.",
                          name(),
                          StatusCode::FAILURE );
  }
  const unsigned int bankVersionNumber = hltvertexreportsRawBank->version();
  if( bankVersionNumber > kVersionNumber ){
    Warning( " HltVertexReports Raw Bank version number "
           + std::to_string( bankVersionNumber)
           + " higher than the one of the decoder " + std::to_string(kVersionNumber),
             StatusCode::SUCCESS, 20 ).ignore();
  }
  auto nWordsPerVert = ( bankVersionNumber == 0 ? 5 : 11 );

  const auto& tbl = id2string( tck(rawEvent) );

  const unsigned int *i   = hltvertexreportsRawBank->begin<unsigned int>();
  const unsigned int *end = hltvertexreportsRawBank->end<unsigned int>();
  int nSel = *i++;
  while ( i < end ) { // avoid infinite loop with corrupt/incompatible data... (i.e. do NOT use i!=end)
    --nSel;
    unsigned nVert    = ( ( *i ) & 0xFFFFL );
    unsigned intSelID = ( *i++ >> 16 );

    auto  value = tbl.find( intSelID );
    if (value == std::end(tbl)) {
      Error( std::string{ " did not find name for id = "} + std::to_string(intSelID) + "; skipping this selection",
             StatusCode::SUCCESS, 50 ).ignore();
      i += nVert * ( bankVersionNumber == 0 ? 5 : 11 ); // would have been nice to have a size / vtx in the bank...
      continue;
    }
    // skip reports if of wrong type, or not requested to decode
    auto indx = ( value->second ? index_( m_decode,  value->second.str() ) : -1 );
    if ( indx < 0 ) { //TODO: use a 'compact optional' instead of int with explicit special value -1
	  i += nWordsPerVert * nVert;
	  continue;
    }

    auto& verticesOutput = output[indx];

    // create output container for vertices
    if (verticesOutput) {// protect against duplicates...
        error()<< "Duplicate selection! -- skipping second one" << endmsg;
	    i += nWordsPerVert * nVert;
	    continue;
    }
    verticesOutput.emplace(); // activate the optional, and make it refer to an empty container!!

    for( unsigned int j=0; j!=nVert; ++j ){
      auto  pVtx = std::make_unique<VertexBase>();
      double x = pun_to<float>( *i++ );
      double y = pun_to<float>( *i++ );
      double z = pun_to<float>( *i++ );
      pVtx->setPosition( { x,y,z } );
      pVtx->setChi2( pun_to<float>( *i++ ) );
      pVtx->setNDoF( *i++ ) ;
      if( bankVersionNumber>0 ){
        Gaudi::SymMatrix3x3 cov;
        cov[0][0] = pun_to<float>( *i++ ) ;
        cov[1][1] = pun_to<float>( *i++ ) ;
        cov[2][2] = pun_to<float>( *i++ ) ;
        cov[0][1] = pun_to<float>( *i++ ) ;
        cov[0][2] = pun_to<float>( *i++ ) ;
        cov[1][2] = pun_to<float>( *i++ ) ;
        pVtx->setCovMatrix(cov);
      }

      verticesOutput->insert(pVtx.release()); //transfer ownership 
    }

  }
  if (nSel!=0) {
     error()  << "Unexpected banksize while decoding (case 1).... " << endmsg;
  }
  if (i!=end) {
     error()  << "Unexpected banksize while decoding (case 2).... " << endmsg;
  }

  return output;
}
