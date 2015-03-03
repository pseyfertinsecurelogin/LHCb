// $Id: OTTimeCreator.cpp,v 1.19 2007-09-07 13:19:21 wouter Exp $
// Include files
// Gaudi
#include "GaudiKernel/AlgFactory.h"

// from Detector
#include "OTDet/DeOTDetector.h"
#include "OTDet/DeOTModule.h"

// Event
#include "Event/OTTime.h"

// local
#include "OTTimeCreator.h"


//-----------------------------------------------------------------------------
// Implementation file for class : OTTimeCreator
//
// 2004-06-06 : Jacopo Nardulli
//-----------------------------------------------------------------------------

using namespace LHCb;
 
// Declaration of the tool Factory
DECLARE_ALGORITHM_FACTORY( OTTimeCreator );


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
OTTimeCreator::OTTimeCreator( const std::string& name,
				    ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator ),
    m_rawbankdecoder("OTRawBankDecoder")
{
  declareProperty( "OutputLocation", m_timeLocation = OTTimeLocation::Default );
  declareProperty("ToFCorrection", m_tofCorrection = true);
}

//=============================================================================
// Destructor
//=============================================================================
OTTimeCreator::~OTTimeCreator() {}; 

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode OTTimeCreator::initialize() {

  StatusCode sc = GaudiAlgorithm::initialize();
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm
  
  // Loading OT Geometry from XML
  m_tracker = getDet<DeOTDetector>( DeOTDetectorLocation::Default );

  // tool handle to the ot raw bank decoder
  sc = m_rawbankdecoder.retrieve() ;

  return sc ;
};

//=============================================================================
// Main execution
//=============================================================================
StatusCode OTTimeCreator::execute() 
{
  // make OTTime container 
  OTTimes* outputTimes = new OTTimes();
  outputTimes->reserve(10000);
  // register output buffer to TES
  put(outputTimes, m_timeLocation);

  // Load all modules
  for(DeOTDetector::Modules::const_iterator imod = m_tracker->modules().begin() ;
      imod != m_tracker->modules().end(); ++imod) {
    LHCb::OTLiteTimeRange ottimes = m_rawbankdecoder->decodeModule( (*imod)->elementID() ) ;
    
    for( LHCb::OTLiteTimeRange::const_iterator it = ottimes.begin() ; it != ottimes.end(); ++it) {
      double unCorrectedTime = it->rawTime() ;
      double t0 = m_tofCorrection ? (*imod)->strawT0( it->channel().straw()) : 0 ;
      outputTimes->insert(new OTTime(it->channel(),unCorrectedTime - t0 ));
    }
  }
  
  return StatusCode::SUCCESS;
}
