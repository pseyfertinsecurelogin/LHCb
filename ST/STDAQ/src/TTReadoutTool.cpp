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
// STDAQ
#include "TTReadoutTool.h"
#include "Kernel/STBoardMapping.h"
#include "Kernel/STTell1Board.h"
#include "Kernel/STTell1ID.h"

// IT
#include "Kernel/STChannelID.h"

#include <algorithm>

// Det Desc
#include "DetDesc/Condition.h"

using namespace LHCb;

DECLARE_COMPONENT( TTReadoutTool )

TTReadoutTool::TTReadoutTool( const std::string& type, const std::string& name, const IInterface* parent )
    : STReadoutTool( type, name, parent ) {
  // constructor

  declareProperty( "conditionLocation", m_conditionLocation = "/dd/Conditions/ReadoutConf/TT/ReadoutMap" );

  m_detType = "TT";
}

StatusCode TTReadoutTool::initialize() {

  // initialization phase...
  StatusCode sc = STReadoutTool::initialize();
  if ( sc.isFailure() ) { return Error( "Failed to initialize", sc ); }

  registerCondition( m_conditionLocation, &TTReadoutTool::createBoards );
  sc = runUpdate(); // force update
  if ( sc.isFailure() ) return Error( "Failed first UMS update for readout tool", sc );

  if ( m_printMapping == true ) { printMapping(); }

  return StatusCode::SUCCESS;
}

unsigned int TTReadoutTool::region( const STChannelID aChan ) const {
  // convert channel to region
  return ( aChan.station() == 1 ? aChan.layer() - 1 : m_nRegionA + aChan.layer() - 1 );
}

// Add the mapping of source ID to TELL1 board number
const std::map<unsigned int, unsigned int>& TTReadoutTool::SourceIDToTELLNumberMap() const {
  return STBoardMapping::TTSourceIDToNumberMap();
}

// Add the mapping of TELL1 board number to source ID
const std::map<unsigned int, unsigned int>& TTReadoutTool::TELLNumberToSourceIDMap() const {
  return STBoardMapping::TTNumberToSourceIDMap();
}

StatusCode TTReadoutTool::createBoards() {

  clear();

  // load conditions
  Condition* rInfo = getDet<Condition>( m_conditionLocation );

  // vector of layer types
  // const std::vector<std::string>& layers = rInfo->paramAsStringVect("layers");
  const std::vector<std::string> layers  = rInfo->param<std::vector<std::string>>( "layers" );
  const std::vector<int>&        nBoards = rInfo->paramAsIntVect( "nBoardsPerLayer" );

  m_hybridsPerBoard             = rInfo->param<int>( "hybridsPerBoard" );
  m_nRegionA                    = rInfo->param<int>( "nRegionsInTTa" );
  unsigned int nStripsPerHybrid = STDAQ::nStripsPerBoard / m_hybridsPerBoard;

  for ( unsigned int iReg = 0; iReg < layers.size(); ++iReg ) {

    m_firstBoardInRegion.push_back( m_boards.size() );
    m_nBoard += nBoards[iReg];

    const std::vector<int>&         tMap        = rInfo->param<std::vector<int>>( layers[iReg] );
    std::string                     orLoc       = layers[iReg] + "HybridOrientation";
    const std::vector<int>&         orientation = rInfo->param<std::vector<int>>( orLoc );
    const std::vector<std::string>& serviceBoxes =
        rInfo->param<std::vector<std::string>>( layers[iReg] + "ServiceBox" );

    unsigned int vecLoc = 0;
    if ( iReg == 0 ) {
      STChannelID firstChan = STChannelID( tMap[0] );
      m_firstStation        = firstChan.station();
    }

    for ( unsigned int iBoard = 0; iBoard < (unsigned int)nBoards[iReg]; ++iBoard ) {

      // make new board
      STTell1ID anID   = STTell1ID( iReg, iBoard );
      auto      aBoard = std::make_unique<STTell1Board>( anID, nStripsPerHybrid, m_detType );

      for ( unsigned iH = 0; iH < m_hybridsPerBoard; ++iH, ++vecLoc ) {
        STChannelID sectorID( (unsigned int)tMap[vecLoc] );
        aBoard->addSector( sectorID, (unsigned int)orientation[vecLoc], serviceBoxes[vecLoc] );

        // add to the list of service boxs if not already there
        if ( std::find( m_serviceBoxes.begin(), m_serviceBoxes.end(), serviceBoxes[vecLoc] ) == m_serviceBoxes.end() ) {
          m_serviceBoxes.push_back( serviceBoxes[vecLoc] );
        }

      } // iH

      m_boards.push_back( std::move( aBoard ) );

      if ( m_boardsMap.find( anID ) == m_boardsMap.end() ) { m_boardsMap[anID] = m_boards.back().get(); }

    } // boards per region
  }   // iterS

  // validate the mapping --> all sectors should go somewhere !
  StatusCode sc = validate();
  if ( sc.isFailure() ) { return Error( "Failed to validate mapping", StatusCode::FAILURE ); }

  return StatusCode::SUCCESS;
}
