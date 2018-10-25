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
// ============================================================================
// Include files
// from Gaudi
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
DECLARE_COMPONENT( CaloDetTestAlgorithm )
// ============================================================================
/** Standard constructor
 *  @param   name   algorithm name 
 *  @param   svcloc pointer to service locator 
 */
// ============================================================================
CaloDetTestAlgorithm::CaloDetTestAlgorithm( const std::string& name   ,
                            ISvcLocator*       svcloc )
  : GaudiAlgorithm ( name , svcloc ) 
{
  declareProperty("DetDataLocation" , m_DetData ) ;
  
  int index = name.find_last_of(".") +1 ; // return 0 if '.' not found --> OK !!
  std::string detectorName = ( name.compare(index,3,"Prs") == 0 ?  "Prs"
                             : name.compare(index,3,"Spd") == 0 ?  "Spd"
                             : name.substr( index, 4 ) );
  if("Ecal" == detectorName){
    m_DetData = DeCalorimeterLocation::Ecal; 
  }else  if("Hcal" == detectorName){
    m_DetData = DeCalorimeterLocation::Hcal; 
  }else  if("Prs" == detectorName){
    m_DetData = DeCalorimeterLocation::Prs; 
  }else  if("Spd" == detectorName){
    m_DetData = DeCalorimeterLocation::Spd; 
  }
  
}


// ============================================================================
/** standard algorithm initialization 
 *  @return status code 
 */
// ============================================================================
StatusCode CaloDetTestAlgorithm::initialize() 
{
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Initialize" << endmsg;
  
  StatusCode sc = GaudiAlgorithm::initialize();
  if( sc.isFailure() ) 
    { return Error("Could not initialize the base class!",sc);}


  DeCalorimeter* calo = getDet<DeCalorimeter>( m_DetData ) ;
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << "Detector element found at " << calo << endmsg;


  // channel
  const CaloVector<CellParam>& cells = calo->cellParams();
  for(const auto & cell : cells){
    
    LHCb::CaloCellID id = (cell).cellID();
    int card = (cell).cardNumber();

    info()    << " | " 
              << id << " | "
              << id.all() << " | "
              << format("0x%04X",id.all()) << " | "
              << (cell).cardColumn() <<"/"<< (cell).cardRow() << " | "
              << (cell).cardNumber() << " ( " << calo->cardCode(card)      << ") | "
              << calo->cardCrate(card)     << " | "
              << calo->cardSlot(card)      << " | "
              << calo->cardToTell1(card)   << " | " 
              << endmsg;
  }
  


  
  return StatusCode::SUCCESS;
}

// ============================================================================
// The End 
// ============================================================================
