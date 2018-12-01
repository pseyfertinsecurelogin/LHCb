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

#include "Event/RawEvent.h"
#include "Event/HltVertexReports.h"

// local
#include "HltVertexReportsWriter.h"
#include "pun.h"

using namespace LHCb;

namespace {

static const Gaudi::StringKey Hlt1SelectionID{"Hlt1SelectionID"};
static const Gaudi::StringKey Hlt2SelectionID{"Hlt2SelectionID"};

}

//-----------------------------------------------------------------------------
// Implementation file for class : HltVertexReportsWriter
//
// 2008-08-05 : Tomasz Skwarnicki
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( HltVertexReportsWriter )

//=============================================================================
// Initialization
//=============================================================================
StatusCode HltVertexReportsWriter::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  m_hltANNSvc = service("HltANNSvc");

  if( m_sourceID > kSourceID_Max || m_sourceID < 0 ){
    return Error("Illegal SourceID specified; maximal allowed value is 7" , StatusCode::FAILURE );
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode HltVertexReportsWriter::execute() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  // get input hlt vertex reports
  HltVertexReports* inputSummary = getIfExists<HltVertexReports>(m_inputHltVertexReportsLocation);
  if( !inputSummary) {
    return Warning( " No HltVertexReports at " + m_inputHltVertexReportsLocation.value(),
                    StatusCode::SUCCESS, 20 );
  }

 // get output
  RawEvent* rawEvent = getIfExists<RawEvent>(m_outputRawEventLocation);
  if( !rawEvent ) {
    return Error(" No RawEvent at " + m_outputRawEventLocation.value(),StatusCode::SUCCESS, 20 );
  }

  std::vector< unsigned int > hltVertexReportsRawBank;
  // first word will count number of vertex selections saved
  hltVertexReportsRawBank.push_back(0);

  // loop over vertex selections given in the input list
  for( const auto& s : *inputSummary) {

     // save selection ---------------------------
     // find int selection id
     auto si = selectionNameToInt(*m_hltANNSvc, s.first );
     if( !si ) {
       Error(" selectionName=" +s.first+ " from HltVertexReports not found in HltANNSvc. Skipped. ", StatusCode::SUCCESS, 20 ).ignore();
       continue;
     }

     ++hltVertexReportsRawBank[0];

     hltVertexReportsRawBank.reserve( hltVertexReportsRawBank.size() + 1 + s.second.size()*11 );
     auto out = std::back_inserter( hltVertexReportsRawBank );

     // first word for each selection contains number of vertices (low short) and selection ID (high short)
     *out++ =  (unsigned int)( std::min( s.second.size(), 65535ul ) | (*si << 16) ) ;

     for(const auto& vtx : s.second ) {
       // now push vertex info
       *out++ = pun_to<unsigned int>( vtx->position().x() );
       *out++ = pun_to<unsigned int>( vtx->position().y() );
       *out++ = pun_to<unsigned int>( vtx->position().z() );
       *out++ = pun_to<unsigned int>( vtx->chi2() );
       *out++ = std::max( vtx->nDoF(), 0 );
       const auto& cov = vtx->covMatrix();
       *out++ = pun_to<unsigned int>( cov[0][0] );
       *out++ = pun_to<unsigned int>( cov[1][1] );
       *out++ = pun_to<unsigned int>( cov[2][2] );
       *out++ = pun_to<unsigned int>( cov[0][1] );
       *out++ = pun_to<unsigned int>( cov[0][2] );
       *out++ = pun_to<unsigned int>( cov[1][2] );
     }
  }

  // delete any previously inserted vtx reports
  const auto& bnks = rawEvent->banks( RawBank::HltVertexReports );
  for( const auto&  b :  LHCb::RawBank::ConstVector{ bnks.begin(), bnks.end() }  ) {
    auto sourceID =  b->version() > 1 ? ( b->sourceID() >> kSourceID_BitShift ) : kSourceID_Hlt;
    if( m_sourceID != sourceID ) continue;
    rawEvent->removeBank(b);
    warning() << " Deleted previously inserted HltVertexReports bank " << endmsg;
  }
  // shift bits in sourceID for the same convention as in HltSelReports
  rawEvent->addBank(  int(m_sourceID<<kSourceID_BitShift), RawBank::HltVertexReports, kVersionNumber, hltVertexReportsRawBank );
  if ( msgLevel(MSG::VERBOSE) ){
    verbose() << " ======= HltVertexReports RawBank size= " << hltVertexReportsRawBank.size() << endmsg;
    verbose() << " VersionNumber= " << kVersionNumber;
    verbose() << " SourceID= " << m_sourceID;
    verbose() << " number of selections stored = " << hltVertexReportsRawBank[0] << endmsg;
    unsigned int iWord = 1;
    for(unsigned int i=0; i!=hltVertexReportsRawBank[0]; ++i){
      unsigned int n = hltVertexReportsRawBank[iWord] & 0xFFFFL;
      verbose() << " selection id " << (unsigned int)( hltVertexReportsRawBank[iWord] >> 16 )
                << " number of vertices " << n << endmsg;
      ++iWord;
      for( unsigned int j=0; j!=n; ++j ){
        verbose() << " " << j
                  << " x " << pun_to<float>( hltVertexReportsRawBank[iWord] )
                  << " y " << pun_to<float>( hltVertexReportsRawBank[iWord+1] )
                  << " z " << pun_to<float>( hltVertexReportsRawBank[iWord+2] )
                  << " chi2 " << pun_to<float>( hltVertexReportsRawBank[iWord+3] )
                  << " nDoF " << hltVertexReportsRawBank[iWord+4]
                  << " cov(x,x) " << pun_to<float>( hltVertexReportsRawBank[iWord+5] )
                  << " cov(y,y) " << pun_to<float>( hltVertexReportsRawBank[iWord+6] )
                  << " cov(z,z) " << pun_to<float>( hltVertexReportsRawBank[iWord+7] )
                  << " cov(x,y) " << pun_to<float>( hltVertexReportsRawBank[iWord+8] )
                  << " cov(x,z) " << pun_to<float>( hltVertexReportsRawBank[iWord+9] )
                  << " cov(y,z) " << pun_to<float>( hltVertexReportsRawBank[iWord+10] )
                  << endmsg;
        iWord += 11;

      }
    }
  }
  return StatusCode::SUCCESS;
}

//=============================================================================
std::optional<int> HltVertexReportsWriter::selectionNameToInt
(const IANNSvc& ann, const std::string& name) const
{
   std::optional<IANNSvc::minor_value_type> v{};
   switch (m_sourceID) {
       case kSourceID_Hlt1: v = ann.value(Hlt1SelectionID,name); break;
       case kSourceID_Hlt2: v = ann.value(Hlt2SelectionID,name); break;
       case kSourceID_Hlt : v = ann.value(Hlt1SelectionID,name);
                            if (!v) v = ann.value(Hlt2SelectionID,name);
                            break;
   }
   return v ? std::optional<int>{ v->second } : std::optional<int>{ } ;
}
