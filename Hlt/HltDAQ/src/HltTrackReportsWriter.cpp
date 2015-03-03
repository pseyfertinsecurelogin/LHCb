// $Id: HltTrackReportsWriter.cpp,v 1.1.1.1 2009-06-24 15:38:52 tskwarni Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/AlgFactory.h" 

#include "Event/RawEvent.h"
#include "Event/Track.h"
#include "HltTrackingCoder.h"
#include <functional>
#include <algorithm>

// local
#include "HltTrackReportsWriter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : HltTrackReportsWriter
//
// 2014-1-10 : Sebastian Neubert
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( HltTrackReportsWriter )


using namespace LHCb;



//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
HltTrackReportsWriter::HltTrackReportsWriter( const std::string& name,
                                          ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{

  declareProperty("InputHltTrackLocation",
    m_inputHltTrackLocation = "Hlt/Track/Velo");  
  declareProperty("OutputRawEventLocation",
    m_outputRawEventLocation= LHCb::RawEventLocation::Default);  
  declareProperty("SourceID",
    m_sourceID= kSourceID_Dummy );  

}
//=============================================================================
// Destructor
//=============================================================================
HltTrackReportsWriter::~HltTrackReportsWriter() {} 

//=============================================================================
// Initialization
//=============================================================================
StatusCode HltTrackReportsWriter::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;


  if( m_sourceID > kSourceID_Max ){
    m_sourceID = m_sourceID & kSourceID_Max;
    return Error("Illegal SourceID specified; maximal allowed value is 7" , StatusCode::FAILURE, 50 );
  }


  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode HltTrackReportsWriter::execute() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  // get input
  if( !exist<LHCb::Tracks>(m_inputHltTrackLocation) ){    
    return Warning( " No tracks at " + m_inputHltTrackLocation.value(), StatusCode::SUCCESS, 20 );
  }  
  const LHCb::Tracks* inputTracks   = get<LHCb::Tracks>( m_inputHltTrackLocation ); 

  // get output
  if( !exist<RawEvent>(m_outputRawEventLocation) ){    
    return Error(" No RawEvent at " + m_outputRawEventLocation.value(), StatusCode::SUCCESS, 20 );
  }  
  RawEvent* rawEvent = get<RawEvent>(m_outputRawEventLocation);


  if( msgLevel(MSG::VERBOSE) )
  {
    verbose() << "----------------------------------------\n";
    verbose() << " Input tracks: \n";  
    // collect tracks pointers in local vector
    LHCb::Tracks::const_iterator pItr;
    for(pItr = inputTracks->begin(); inputTracks->end() != pItr; ++pItr){
      LHCb::Track* Tr = (*pItr);
      verbose()  << *Tr << endmsg ;
      // also dump IDs
      verbose() << "LHCbIDs: [\n"; 
      unsigned int nhits= Tr->nLHCbIDs();
      for(unsigned int i=0;i<nhits;++i){
	verbose() << Tr->lhcbIDs()[i] << ",\n";
	  }
      verbose() << "]" << endmsg;

    }
    verbose() << "\n----------------------------------------";
    verbose() << endmsg;
  }
  
  // compose the bank body
  std::vector<unsigned int> bankBody;

  // calling core encoder function see src/core/HltTrackingCoder.cxx
  encodeTracks(inputTracks,bankBody);

  // shift bits in sourceID for the same convention as in HltSelReports
  rawEvent->addBank(  int(m_sourceID<<kSourceID_BitShift), RawBank::HltTrackReports, kVersionNumber, bankBody );
  

  if ( msgLevel(MSG::VERBOSE) ){
    verbose() << " Output:  ";  
    verbose() << " VersionNumber= " << kVersionNumber;  
    verbose() << " SourceID= " << m_sourceID;
    //std::vector<unsigned int>::const_iterator i=bankBody.begin();
    //verbose() << " configuredTCK = " << *i++ << " " ;
    //verbose() << " taskID = " << *i++ << " " ;
    // for( ; i!=bankBody.end();++i) {
    //   verbose() << HltDecReport(*i).intDecisionID() << "-" << HltDecReport(*i).decision() << " ";
    // }
    verbose() << endmsg;
  }

  return StatusCode::SUCCESS;
}

