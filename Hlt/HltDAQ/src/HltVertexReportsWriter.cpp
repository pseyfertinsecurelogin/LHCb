// Include files 

// from Gaudi
#include "GaudiKernel/AlgFactory.h" 

#include "Event/RawEvent.h"
#include "Event/HltVertexReports.h"

// local
#include "HltVertexReportsWriter.h"

using namespace LHCb;

namespace {
const Gaudi::StringKey Hlt1SelectionID{"Hlt1SelectionID"};
const Gaudi::StringKey Hlt2SelectionID{"Hlt2SelectionID"};
float floatFromInt(unsigned int i)
{
        union IntFloat { unsigned int mInt; float mFloat; };
        IntFloat a; a.mInt=i;
        return a.mFloat;
}

unsigned int doubleToInt(double d)
{
        union IntFloat { unsigned int mInt; float mFloat; };
        IntFloat a; a.mFloat = float(d);
        return a.mInt;
}
}

//-----------------------------------------------------------------------------
// Implementation file for class : HltVertexReportsWriter
//
// 2008-08-05 : Tomasz Skwarnicki
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( HltVertexReportsWriter )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
HltVertexReportsWriter::HltVertexReportsWriter( const std::string& name,
                                                      ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{

  declareProperty("InputHltVertexReportsLocation",
    m_inputHltVertexReportsLocation= LHCb::HltVertexReportsLocation::Default);  
  declareProperty("OutputRawEventLocation",
    m_outputRawEventLocation= LHCb::RawEventLocation::Default);
  declareProperty("SourceID",
    m_sourceID= kSourceID_Dummy );  

  m_hltANNSvc = 0;

}
//=============================================================================
// Destructor
//=============================================================================
HltVertexReportsWriter::~HltVertexReportsWriter() {} 

//=============================================================================
// Initialization
//=============================================================================
StatusCode HltVertexReportsWriter::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  m_hltANNSvc = svc<IANNSvc>("HltANNSvc");

  if( m_sourceID > kSourceID_Max ){
    m_sourceID = m_sourceID & kSourceID_Max;
    return Error("Illegal SourceID specified; maximal allowed value is 7" , StatusCode::FAILURE, 50 );
  }


  return StatusCode::SUCCESS;
}



//=============================================================================
// Main execution
//=============================================================================
StatusCode HltVertexReportsWriter::execute() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  // get input hlt vertex reports
  if( !exist<HltVertexReports>(m_inputHltVertexReportsLocation) ){    
    return Warning( " No HltVertexReports at " + m_inputHltVertexReportsLocation.value(),
                    StatusCode::SUCCESS, 20 );
  }  
  HltVertexReports* inputSummary = get<HltVertexReports>(m_inputHltVertexReportsLocation);

 // get output
  if( !exist<RawEvent>(m_outputRawEventLocation) ){    
    return Error(" No RawEvent at " + m_outputRawEventLocation.value(),StatusCode::SUCCESS, 20 );
  }  
  RawEvent* rawEvent = get<RawEvent>(m_outputRawEventLocation);

  // get string-to-int selection ID map
  auto selectionNameToIntMap = m_hltANNSvc->item_map(Hlt1SelectionID); // new style
  selectionNameToIntMap.merge( m_hltANNSvc->item_map(Hlt2SelectionID) );

  std::vector< unsigned int > hltVertexReportsRawBank;
  // first word will count number of vertex selections saved
  hltVertexReportsRawBank.push_back(0);
  
  // loop over vertex selections given in the input list
  for( const auto& s : *inputSummary) { 

     // save selection ---------------------------
     // find int selection id
     auto si = selectionNameToIntMap.find( s.first );
     if( si==std::end(selectionNameToIntMap) ) {
       Error(" selectionName=" +s.first+ " from HltVertexReports not found in HltANNSvc. Skipped. ", StatusCode::SUCCESS, 20 ); 
       continue;
     }
     int intSelID = si->second;

     ++hltVertexReportsRawBank[0];

     unsigned int size = s.second.size();
     size = std::min( size, 65535u );
     // first word for each selection contains number of vertices (low short) and selection ID (high short)
     hltVertexReportsRawBank.push_back(  (unsigned int)( size | (intSelID << 16) ) );
     
     for(const auto& vtx : s.second ) { 
       // now push vertex info
       hltVertexReportsRawBank.push_back( doubleToInt( vtx->position().x() ) );
       hltVertexReportsRawBank.push_back( doubleToInt( vtx->position().y() ) );
       hltVertexReportsRawBank.push_back( doubleToInt( vtx->position().z() ) );
       hltVertexReportsRawBank.push_back( doubleToInt( vtx->chi2() ) );
       hltVertexReportsRawBank.push_back( std::max( vtx->nDoF(), 0 ) ); 
       const Gaudi::SymMatrix3x3 & cov = vtx->covMatrix();
       hltVertexReportsRawBank.push_back( doubleToInt( cov[0][0] ) );
       hltVertexReportsRawBank.push_back( doubleToInt( cov[1][1] ) );
       hltVertexReportsRawBank.push_back( doubleToInt( cov[2][2] ) );
       hltVertexReportsRawBank.push_back( doubleToInt( cov[0][1] ) );
       hltVertexReportsRawBank.push_back( doubleToInt( cov[0][2] ) );
       hltVertexReportsRawBank.push_back( doubleToInt( cov[1][2] ) );
      
     }
  }

  // delete any previously inserted vtx reports
  const std::vector<RawBank*> hltvtxreportsRawBanks = rawEvent->banks( RawBank::HltVertexReports );
  for( const auto&  b : hltvtxreportsRawBanks ) {
    unsigned int sourceID=kSourceID_Hlt;
    if( b->version() > 1 ) sourceID = b->sourceID() >> kSourceID_BitShift;
    if( m_sourceID != sourceID ) continue;

    rawEvent->removeBank(b);
    if ( msgLevel(MSG::VERBOSE) ){ verbose() << " Deleted previosuly inserted HltVertexReports bank " << endmsg;
    }
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
                  << " x " << floatFromInt( hltVertexReportsRawBank[iWord] )
                  << " y " << floatFromInt( hltVertexReportsRawBank[iWord+1] )
                  << " z " << floatFromInt( hltVertexReportsRawBank[iWord+2] )
                  << " chi2 " << floatFromInt( hltVertexReportsRawBank[iWord+3] )
                  << " nDoF " << hltVertexReportsRawBank[iWord+4] 
                  << " cov(x,x) " << floatFromInt( hltVertexReportsRawBank[iWord+5] )
                  << " cov(y,y) " << floatFromInt( hltVertexReportsRawBank[iWord+6] )
                  << " cov(z,z) " << floatFromInt( hltVertexReportsRawBank[iWord+7] )
                  << " cov(x,y) " << floatFromInt( hltVertexReportsRawBank[iWord+8] )
                  << " cov(x,z) " << floatFromInt( hltVertexReportsRawBank[iWord+9] )
                  << " cov(y,z) " << floatFromInt( hltVertexReportsRawBank[iWord+10] )
                  << endmsg;
        iWord += 11;
  
      }
    }
  }
  return StatusCode::SUCCESS;
}

