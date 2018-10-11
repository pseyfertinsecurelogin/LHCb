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
// STDAQ
#include "ITReadoutTool.h"
#include "Kernel/STTell1Board.h"
#include "Kernel/STTell1ID.h"
#include "Kernel/STBoardMapping.h"

// IT
#include "Kernel/STChannelID.h"

// Det Desc
#include "DetDesc/Condition.h"

#include <algorithm>


using namespace LHCb;

DECLARE_COMPONENT( ITReadoutTool )

ITReadoutTool::ITReadoutTool(const std::string& type,
                            const std::string& name,
                            const IInterface* parent)
  : STReadoutTool( type, name, parent )
{
  declareProperty( "conditionLocation",
                   m_conditionLocation  = "/dd/Conditions/ReadoutConf/IT/ReadoutMap" );

  m_detType = "IT";

}

StatusCode ITReadoutTool::initialize() {

  // initialization phase...
  StatusCode sc = STReadoutTool::initialize();
  if (sc.isFailure()) return Error("Failed to initialize", sc);

  registerCondition(m_conditionLocation, &ITReadoutTool::createBoards );
  sc = runUpdate();
  if (sc.isFailure()) return Error ( "Failed first UMS update for ITReadoutTool", sc );

  if (m_printMapping) printMapping();

  return StatusCode::SUCCESS;
}

unsigned int ITReadoutTool::region(const STChannelID aChan) const{
  // convert channel to region
  return aChan.station()-m_firstStation;
}

// Add the mapping of source ID to board number
const  std::map<unsigned int, unsigned int>& ITReadoutTool::SourceIDToTELLNumberMap() const {
  return STBoardMapping::ITSourceIDToNumberMap();
}

// Add the mapping of TELL1 board number to source ID
const  std::map<unsigned int, unsigned int>& ITReadoutTool::TELLNumberToSourceIDMap() const {
  return STBoardMapping::ITNumberToSourceIDMap();
}

StatusCode ITReadoutTool::createBoards() {

  clear();

  // load conditions
  Condition* rInfo = getDet<Condition>(m_conditionLocation);

  // vector of layer types
  const std::vector<std::string>& stations =
    rInfo->param<std::vector<std::string> >("stations");

  m_hybridsPerBoard = rInfo->param<int>("hybridsPerBoard");
  unsigned int nBoardPerStation = rInfo->param<int>("nBoard");
  unsigned int nStripsPerHybrid =  STDAQ::nStripsPerBoard/m_hybridsPerBoard;
  m_nBoard = nBoardPerStation*stations.size();


  unsigned int iReg = 0;

  for (auto iterS = stations.begin(); iterS != stations.end(); ++iterS, ++iReg){
   m_firstBoardInRegion.push_back(m_boards.size());

   const std::vector<int>& tMap = rInfo->param<std::vector<int> >(*iterS);
   const std::vector<int>& orientation = rInfo->param<std::vector<int> >(*iterS+"HybridOrientation");
   const std::vector<std::string>& serviceBoxes = rInfo->param<std::vector<std::string> >(*iterS+"ServiceBox");

   unsigned int vecLoc = 0;

   if ( iterS == stations.begin()){
     STChannelID firstChan = STChannelID(tMap[0]);
     m_firstStation = firstChan.station() ;
   }

   for (unsigned int iBoard = 0; iBoard < nBoardPerStation; ++iBoard){

     // make new board
     STTell1ID anID = STTell1ID(iReg,iBoard);
     auto aBoard = std::make_unique<STTell1Board>(anID,nStripsPerHybrid, m_detType );

     for (unsigned iH = 0 ; iH < m_hybridsPerBoard; ++iH, ++vecLoc){
       STChannelID sectorID((unsigned int)tMap[vecLoc]);
       aBoard->addSector(sectorID, (unsigned int)orientation[vecLoc], serviceBoxes[vecLoc]);


       // add to the list of service boxs if not already there
       if (std::find(m_serviceBoxes.begin(), m_serviceBoxes.end(),
                     serviceBoxes[vecLoc]) ==  m_serviceBoxes.end()) {
	 m_serviceBoxes.push_back(serviceBoxes[vecLoc]);
       }

     } // iH

     m_boards.push_back(std::move(aBoard));

     if (m_boardsMap.find( anID ) == m_boardsMap.end()) {
       m_boardsMap[anID] = m_boards.back().get();
     }

   } // boards per region
  } // iterS


  // validate the mapping --> all sectors should go somewhere !
  StatusCode sc = validate();
  if (sc.isFailure() ){
    return Error("Failed to validate mapping",StatusCode::FAILURE);
  }

  return StatusCode::SUCCESS;
}
