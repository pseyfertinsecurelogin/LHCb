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
#include "UTDAQ/UTReadoutTool.h"
#include "Kernel/UTTell1Board.h"
#include "Kernel/UTTell1ID.h"
#include "Kernel/UTBoardMapping.h"
#include "Kernel/UTChannelID.h"
#include <algorithm>
#include "DetDesc/Condition.h"
#include "Kernel/UTXMLUtils.h"

using namespace LHCb;

DECLARE_COMPONENT( UTReadoutTool )

UTReadoutTool::UTReadoutTool(const std::string& type,
                            const std::string& name,
                            const IInterface* parent)
: base_class( type, name, parent)
{
  // constructor
  m_boards.reserve(100); // about correct
}


void UTReadoutTool::clear() {
  // clear the boards
  m_boards.clear();
  m_nBoard = 0;
}

StatusCode UTReadoutTool::initialize() {
  // initialization phase...
  StatusCode sc = GaudiTool::initialize();
  if (sc.isFailure()){
    return Error("Failed to initialize", sc);
  }

  // tracker
  m_tracker = getDet<DeUTDetector>(DeUTDetLocation::location());

  registerCondition(m_conditionLocation,
                    &UTReadoutTool::createTell1Map);

  registerCondition(m_conditionLocation,
                    &UTReadoutTool::createBoards);

  sc = runUpdate(); // force update
  if (sc.isFailure()) return Error ( "Failed first UMS update for readout tool", sc );

  if (m_printMapping) printMapping();

  return StatusCode::SUCCESS;
}

StatusCode UTReadoutTool::finalize()  {

  if (m_writeXML) writeMappingToXML();
  return base_class::finalize();
}

StatusCode UTReadoutTool::writeMappingToXML() const {

  // load conditions
  Condition* rInfo = getDet<Condition>(m_conditionLocation);

  std::ofstream outputFile(m_outputFileName.value());
  if (outputFile.fail() ){
    return Warning("Failed to open output file",StatusCode::FAILURE);
  }

  // write the xml headers
  outputFile << header(rInfo->toXml("", true, m_precision))<< '\n';

  // add comments
  std::ostringstream comment;
  UT::XMLUtils::fullComment(comment, m_author, m_tag, m_desc);
  outputFile << comment.str() << '\n';

  std::string temp = strip(rInfo->toXml("", false, m_precision));
  outputFile << temp << "\n\n";

  // footer
  outputFile << footer() << '\n';

  return StatusCode::SUCCESS;
}

unsigned int UTReadoutTool::nBoard() const {
  // number of boards
  return m_nBoard;
}

unsigned int UTReadoutTool::nServiceBox() const{
  return m_serviceBoxes.size();
}

std::string UTReadoutTool::serviceBox(const LHCb::UTChannelID& aChan) const{

  // find the board

  static const std::string InValidBox = "Unknown";
  bool isFound = false;
  unsigned int waferIndex = 999u;
  unsigned int iBoard = m_firstBoardInRegion[region(aChan)];
  while ((iBoard != m_nBoard)&&(isFound == false)){
     if (m_boards[iBoard]->isInside(aChan,waferIndex)) {
      isFound = true;
    } else {
      ++iBoard;
    }
  } // iBoard
  return(  isFound ? m_boards[iBoard]->serviceBoxes()[waferIndex] : InValidBox);
}

std::vector<UTTell1ID> UTReadoutTool::boardIDs() const{
  std::vector<UTTell1ID> ids; ids.reserve(m_boards.size());
  std::transform( m_boards.begin(), m_boards.end(),
                  std::back_inserter(ids),
                  [](const auto& b) { return b->boardID(); } );
  return ids;
}

UTDAQ::chanPair UTReadoutTool::offlineChanToDAQ(const UTChannelID aOfflineChan,
                                                double isf) const
{
  // look up region start.....
  unsigned int iBoard = m_firstBoardInRegion[region(aOfflineChan)];
  unsigned int waferIndex = 999u;

  bool isFound = false;
  while ((iBoard != m_nBoard)&&!isFound){
     if (m_boards[iBoard]->isInside(aOfflineChan,waferIndex)) {
      isFound = true;
    } else {
      ++iBoard;
    }
  } // iBoard

  if (!isFound){
    return { UTTell1ID(UTTell1ID::nullBoard, false),0};
  } else {
    return { m_boards[iBoard]->boardID(),
             m_boards[iBoard]->offlineToDAQ(aOfflineChan,
                                            waferIndex,isf)};
  }
}


double UTReadoutTool::interStripToDAQ(const UTChannelID aOfflineChan,
                                      const UTTell1ID aBoardID,
                                      const double isf) const
{
  unsigned int waferIndex = 999u;

  UTTell1Board* aBoard = this->findByBoardID(aBoardID);
  double newisf = 0;

  if(aBoard->isInside(aOfflineChan,waferIndex)){
    unsigned int orientation = aBoard->orientation()[waferIndex];
    if(orientation == 0 && isf > 0.01){
      newisf = 1 - isf;
    } else {
      newisf = isf;
    }
  } else { // Can not find board!
    newisf = -1;
  }

  return newisf;
}


bool UTReadoutTool::ADCOfflineToDAQ(const UTChannelID aOfflineChan,
                                    const UTTell1ID aBoardID,
                                    UTCluster::ADCVector& adcs) const
{
  unsigned int waferIndex = 999u;
  UTTell1Board* aBoard = this->findByBoardID(aBoardID);

  if( !aBoard->isInside(aOfflineChan,waferIndex) ) return false; // can not find board!

  if( aBoard->orientation()[waferIndex] == 0 ){
    std::reverse( std::begin(adcs), std::end(adcs) );
  }
  return true;
}


UTTell1Board* UTReadoutTool::findByBoardID(const UTTell1ID aBoardID) const{
  // find by board id
  try {
    return m_boardsMap.at(aBoardID);
  } catch (std::out_of_range &e) {
    return nullptr;
  }
}

UTTell1Board* UTReadoutTool::findByOrder(const unsigned int aValue) const{
  // find by order
  return aValue< m_nBoard ? m_boards[aValue].get() : nullptr;
}

void UTReadoutTool::printMapping() const{

  // dump out the readout mapping
  std::cout << "print mapping for: " << name() << " tool\n";
  std::cout << " Number of boards " << m_nBoard << '\n';
  for (const auto& b : m_boards ) std::cout << *b << '\n';
}

/// Add the mapping of source ID to TELL1 board number
unsigned int UTReadoutTool::SourceIDToTELLNumber(unsigned int sourceID) const {
  return (this->SourceIDToTELLNumberMap().find(sourceID))->second;
}

/// Add the mapping of TELL1 board number to source ID
unsigned int UTReadoutTool::TELLNumberToSourceID(unsigned int TELL) const {
  return (this->TELLNumberToSourceIDMap().find(TELL))->second;
}

StatusCode UTReadoutTool::validate() const{

  // validate the map - every sector must go somewhere !
  const DeUTDetector::Sectors& dSectors = m_tracker->sectors();
  return StatusCode{std::none_of(std::begin(dSectors), std::end(dSectors),
                      [this](const DeUTSector *s) {
                             UTChannelID chan = s->elementID();
                             auto chanPair = offlineChanToDAQ(chan,0.0);
                             return chanPair.first == UTTell1ID(UTTell1ID::nullBoard, false);
                       } )};
}

std::vector<LHCb::UTChannelID> UTReadoutTool::sectorIDs(const UTTell1ID board) const{

  std::vector<LHCb::UTChannelID> sectors; sectors.reserve(8);
  UTTell1Board* theBoard = findByBoardID(board);
  if (theBoard){
    sectors.insert(sectors.begin(), theBoard->sectorIDs().begin(), theBoard->sectorIDs().end());
  }
  else {
    Error("Failed to find Board", StatusCode::SUCCESS, 100);
  }
  return sectors;
}

std::vector<DeUTSector*> UTReadoutTool::sectors(const UTTell1ID board) const{

  return m_tracker->findSectors(sectorIDs(board));
}

std::vector<DeUTSector*> UTReadoutTool::sectorsOnServiceBox(const std::string& serviceBox) const{

  return m_tracker->findSectors(sectorIDsOnServiceBox(serviceBox));
}

std::vector<LHCb::UTChannelID> UTReadoutTool::sectorIDsOnServiceBox(const std::string& serviceBox) const{
  // loop over all boards
  std::vector<LHCb::UTChannelID> sectors; sectors.reserve(16);
  for (const auto& board : m_boards ) {
    const std::vector<LHCb::UTChannelID>& sectorVec = board->sectorIDs();
    const std::vector<std::string>& sBoxes = board->serviceBoxes();
    for (unsigned int iS = 0u ; iS < board->nSectors(); ++iS){
      if (sBoxes[iS] == serviceBox) sectors.push_back(sectorVec[iS]);
    } // iS
  } // iterB
  return sectors;
}

const std::vector<std::string>& UTReadoutTool::serviceBoxes() const{
  return m_serviceBoxes;
}

std::string UTReadoutTool::footer() const
{
  std::string temp = m_footer;
  temp.insert(0, "</catalog>" );
  return temp;
}

std::string UTReadoutTool::header(const std::string& conString) const
{
  // get the header
  std::string::size_type startpos = conString.find(m_startTag);
  std::string temp = conString.substr(0,startpos);
  temp.insert(startpos, "<catalog name=\"ReadoutSectors\">" );

  // correct the location of the DTD
  if( m_removeCondb ) {
    UT::XMLUtils::replace(temp,"conddb:", "");
    std::string location;
    for (unsigned int i = 0;  i< m_depth; ++i) location += "../";
    std::string::size_type pos = temp.find("/DTD/");
    temp.insert(pos,location);
    UT::XMLUtils::replace(temp,"//", "/");
  }

  return temp;
}

std::string UTReadoutTool::strip(const std::string& conString) const
{
  std::string::size_type startpos = conString.find(m_startTag);
  std::string::size_type endpos = conString.find(m_footer);
  return conString.substr(startpos, endpos - startpos);
}

unsigned int UTReadoutTool::region(const UTChannelID aChan) const{
  // convert channel to region
  return aChan.station() == 1 ?  aChan.layer() - 1 : m_nRegionA + aChan.layer() - 1;
}

// Add the mapping of source ID to TELL1 board number
const  std::map<unsigned int, unsigned int>& UTReadoutTool::SourceIDToTELLNumberMap() const {
  return UTBoardMapping::UTSourceIDToNumberMap();
}

// Add the mapping of TELL1 board number to source ID
const  std::map<unsigned int, unsigned int>& UTReadoutTool::TELLNumberToSourceIDMap() const {
  return UTBoardMapping::UTNumberToSourceIDMap();
}

StatusCode UTReadoutTool::createTell1Map()
{
  Condition* rInfo = getDet<Condition>(m_conditionLocation);
  const std::vector<std::string> layers =  rInfo->param<std::vector<std::string> >("layers");

  UTBoardMapping::ClearUTMap();

  unsigned int sourceIDBase = 0;
  for (unsigned int iReg = 0; iReg < layers.size(); ++iReg){
    std::string tell1Loc = layers[iReg]+"TELL1";
    if ( rInfo->exists(tell1Loc) ) {
      //      printf("Extracting TELL1 map from %s\n", tell1Loc.c_str());

      const std::vector<int>& tell1 = rInfo->param<std::vector<int> >(tell1Loc);
      for ( unsigned int i=0; i<tell1.size(); i++ ) {
        UTBoardMapping::AddUTMapEntry(sourceIDBase+i, tell1.at(i));
      }
    }
    sourceIDBase += 64;
  }

  return StatusCode::SUCCESS;
}



StatusCode UTReadoutTool::createBoards() {

  bool isUT = true;
  clear();

  // load conditions
  Condition* rInfo = getDet<Condition>(m_conditionLocation);

  // vector of layer types
  // const std::vector<std::string>& layers = rInfo->paramAsStringVect("layers");
  const std::vector<std::string> layers =  rInfo->param<std::vector<std::string> >("layers");
  const std::vector<int>& nBoards = rInfo->paramAsIntVect("nBoardsPerLayer");

  m_hybridsPerBoard = rInfo->param<int>("hybridsPerBoard");
  m_nRegionA = rInfo->param<int>("nRegionsInUTa");
  unsigned int nStripsPerHybrid =  UTDAQ::nStripsPerBoard/m_hybridsPerBoard;

  for (unsigned int iReg = 0; iReg < layers.size(); ++iReg){

   m_firstBoardInRegion.push_back(m_boards.size());
   m_nBoard += nBoards[iReg];

   const std::vector<int>& tMap = rInfo->param<std::vector<int> >(layers[iReg]);
   std::string orLoc = layers[iReg]+"HybridOrientation";
   const std::vector<int>& orientation = rInfo->param<std::vector<int> >(orLoc);
   const std::vector<std::string>& serviceBoxes = rInfo->param<std::vector<std::string> >(layers[iReg]+"ServiceBox");


   unsigned int vecLoc = 0;
   if ( iReg == 0){
     UTChannelID firstChan = UTChannelID(tMap[0]);
     m_firstStation = firstChan.station() ;
   }

   for (unsigned int iBoard = 0; iBoard < (unsigned int)nBoards[iReg]; ++iBoard){

     // make new board
     UTTell1ID anID = UTTell1ID(iReg,iBoard, isUT);
     auto aBoard = std::make_unique<UTTell1Board>(anID, nStripsPerHybrid, "UT");

     for (unsigned iH = 0 ; iH < m_hybridsPerBoard; ++iH, ++vecLoc){
       UTChannelID sectorID((unsigned int)tMap[vecLoc]);
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


