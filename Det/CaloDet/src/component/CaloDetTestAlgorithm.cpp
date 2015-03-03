// $Id: CaloDetTestAlgorithm.cpp,v 1.8 2009-04-17 13:41:04 cattanem Exp $
// ============================================================================
// Include files
// from Gaudi
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/MsgStream.h" 
#include "GaudiKernel/SmartDataPtr.h" 
// local
#include "CaloDetTestAlgorithm.h"

// ============================================================================
/** @file CaloDetTestAlgorithm.cpp
 * 
 *  Implementation file for class : CaloDetTestAlgorithm
 * 
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date 02/11/2001
 */
// ============================================================================

// ============================================================================
/** Declaration of the Algorithm Factory
 */
// ============================================================================
DECLARE_ALGORITHM_FACTORY(CaloDetTestAlgorithm)
// ============================================================================
/** Standard constructor
 *  @param   name   algorithm name 
 *  @param   svcloc pointer to service locator 
 */
// ============================================================================
CaloDetTestAlgorithm::CaloDetTestAlgorithm( const std::string& name   ,
                            ISvcLocator*       svcloc )
  : GaudiAlgorithm ( name , svcloc ) 
  , m_DetData(  )
{
  declareProperty("DetDataLocation" , m_DetData ) ;
  
  int index = name.find_last_of(".") +1 ; // return 0 if '.' not found --> OK !!
  std::string detectorName = name.substr( index, 4 ); 
  if ( name.substr(index,3) == "Prs" ) detectorName = "Prs";
  if ( name.substr(index,3) == "Spd" ) detectorName = "Spd";
  if("Ecal" == detectorName){
    m_DetData = DeCalorimeterLocation::Ecal; 
  }else  if("Hcal" == detectorName){
    m_DetData = DeCalorimeterLocation::Hcal; 
  }else  if("Prs" == detectorName){
    m_DetData = DeCalorimeterLocation::Prs; 
  }else  if("Spd" == detectorName){
    m_DetData = DeCalorimeterLocation::Spd; 
  }
  
};

// ============================================================================
/** destructor
 */
// ============================================================================
CaloDetTestAlgorithm::~CaloDetTestAlgorithm() {};

// ============================================================================
/** standard algorithm initialization 
 *  @return status code 
 */
// ============================================================================
StatusCode CaloDetTestAlgorithm::initialize() 
{
  
  MsgStream log(msgSvc(), name());
  log << MSG::DEBUG << "==> Initialise" << endmsg;
  
  StatusCode sc = GaudiAlgorithm::initialize();
  if( sc.isFailure() ) 
    { return Error("Could not initialize the base class!",sc);}


  DeCalorimeter* calo = getDet<DeCalorimeter>( m_DetData ) ;
  debug() << "Detector element found at " << calo << endmsg;


  // channel
  const CaloVector<CellParam>& cells = calo->cellParams();
  for(CaloVector<CellParam>::const_iterator icel = cells.begin();icel != cells.end();++icel){
    
    LHCb::CaloCellID id = (*icel).cellID();
    int card = (*icel).cardNumber();

    info()    << " | " 
              << (*icel).cellID() << " | "
              << (*icel).cellID().all() << " | "
              << format("0x%04X",(*icel).cellID().all()) << " | "
              << (*icel).cardColumn() <<"/"<< (*icel).cardRow() << " | "
              << (*icel).cardNumber() << " ( " << calo->cardCode(card)      << ") | "
              << calo->cardCrate(card)     << " | "
              << calo->cardSlot(card)      << " | "
              << calo->cardToTell1(card)   << " | " 
              << endmsg;
  }
  


  
  return StatusCode::SUCCESS;
};

// ============================================================================
/** standard algorithm finalization 
 *  @return status code 
 */
// ============================================================================
StatusCode CaloDetTestAlgorithm::finalize() 
{
  
  MsgStream log(msgSvc(), name());
  log << MSG::DEBUG << "==> Finalize" << endmsg;
  
  /// finalize the base class 
  return GaudiAlgorithm::finalize();
};

// ============================================================================
/** standard algorithm execution 
 *  @return status code 
 */
// ============================================================================
StatusCode CaloDetTestAlgorithm::execute() 
{  

  return StatusCode::SUCCESS;

};

// ============================================================================
// The End 
// ============================================================================
