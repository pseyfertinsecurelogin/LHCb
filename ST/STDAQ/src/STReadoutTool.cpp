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
#include "STReadoutTool.h"

#include "Event/STCluster.h"
#include "Kernel/STDAQDefinitions.h"
#include "Kernel/STTell1Board.h"
#include "Kernel/STTell1ID.h"

// IT
#include "Kernel/STChannelID.h"
#include "STDet/DeSTDetector.h"

#include <algorithm>

// STKernel
#include "Kernel/STXMLUtils.h"

/** @file DeSTSector.cpp
 *
 *  Implementation of class :  DeSTSector
 *
 *    @author Matthew Needham
 */

using namespace LHCb;

STReadoutTool::STReadoutTool( const std::string& type, const std::string& name, const IInterface* parent )
    : base_class( type, name, parent ) {
  m_boards.reserve( 100 ); // about correct
}

void STReadoutTool::clear() {
  // clear the boards
  m_boards.clear();
  m_nBoard = 0;
}

StatusCode STReadoutTool::initialize() {

  // initialization phase...
  StatusCode sc = base_class::initialize();
  if ( sc.isFailure() ) { return Error( "Failed to initialize", sc ); }

  // tracker
  m_tracker = getDet<DeSTDetector>( DeSTDetLocation::location( m_detType ) );

  return StatusCode::SUCCESS;
}

StatusCode STReadoutTool::finalize() {

  if ( m_writeXML ) writeMappingToXML();
  return base_class::finalize();
}

StatusCode STReadoutTool::writeMappingToXML() const {

  // load conditions
  Condition* rInfo = getDet<Condition>( m_conditionLocation );

  std::ofstream outputFile( m_outputFileName.value() );
  if ( outputFile.fail() ) { return Warning( "Failed to open output file", StatusCode::FAILURE ); }

  // write the xml headers
  outputFile << header( rInfo->toXml( "", true, m_precision ) ) << '\n';

  // add comments
  std::ostringstream comment;
  ST::XMLUtils::fullComment( comment, m_author, m_tag, m_desc );
  outputFile << comment.str() << '\n';

  std::string temp = strip( rInfo->toXml( "", false, m_precision ) );
  outputFile << temp << "\n\n";

  // footer
  outputFile << footer() << '\n';

  return StatusCode::SUCCESS;
}

unsigned int STReadoutTool::nBoard() const {
  // number of boards
  return m_nBoard;
}

unsigned int STReadoutTool::nServiceBox() const { return m_serviceBoxes.size(); }

std::string STReadoutTool::serviceBox( const LHCb::STChannelID& aChan ) const {

  // find the board

  static const std::string InValidBox = "Unknown";
  bool                     isFound    = false;
  unsigned int             waferIndex = 999u;
  unsigned int             iBoard     = m_firstBoardInRegion[region( aChan )];
  while ( ( iBoard != m_nBoard ) && ( isFound == false ) ) {
    if ( m_boards[iBoard]->isInside( aChan, waferIndex ) ) {
      isFound = true;
    } else {
      ++iBoard;
    }
  } // iBoard
  return ( isFound ? m_boards[iBoard]->serviceBoxes()[waferIndex] : InValidBox );
}

std::vector<STTell1ID> STReadoutTool::boardIDs() const {
  std::vector<STTell1ID> ids;
  ids.reserve( m_boards.size() );
  std::transform( m_boards.begin(), m_boards.end(), std::back_inserter( ids ),
                  []( const auto& b ) { return b->boardID(); } );
  return ids;
}

STDAQ::chanPair STReadoutTool::offlineChanToDAQ( const STChannelID aOfflineChan, double isf ) const {
  // look up region start.....
  unsigned int iBoard     = m_firstBoardInRegion[region( aOfflineChan )];
  unsigned int waferIndex = 999u;

  bool isFound = false;
  while ( ( iBoard != m_nBoard ) && !isFound ) {
    if ( m_boards[iBoard]->isInside( aOfflineChan, waferIndex ) ) {
      isFound = true;
    } else {
      ++iBoard;
    }
  } // iBoard

  if ( !isFound ) {
    return {STTell1ID( STTell1ID::nullBoard, false ), 0};
  } else {
    return {m_boards[iBoard]->boardID(), m_boards[iBoard]->offlineToDAQ( aOfflineChan, waferIndex, isf )};
  }
}

double STReadoutTool::interStripToDAQ( const STChannelID aOfflineChan, const STTell1ID aBoardID,
                                       const double isf ) const {
  unsigned int waferIndex = 999u;

  STTell1Board* aBoard = this->findByBoardID( aBoardID );
  double        newisf = 0;

  if ( aBoard->isInside( aOfflineChan, waferIndex ) ) {
    unsigned int orientation = aBoard->orientation()[waferIndex];
    if ( orientation == 0 && isf > 0.01 ) {
      newisf = 1 - isf;
    } else {
      newisf = isf;
    }
  } else { // Can not find board!
    newisf = -1;
  }

  return newisf;
}

bool STReadoutTool::ADCOfflineToDAQ( const STChannelID aOfflineChan, const STTell1ID aBoardID,
                                     STCluster::ADCVector& adcs ) const {
  unsigned int  waferIndex = 999u;
  STTell1Board* aBoard     = this->findByBoardID( aBoardID );

  if ( !aBoard->isInside( aOfflineChan, waferIndex ) ) return false; // can not find board!

  if ( aBoard->orientation()[waferIndex] == 0 ) { std::reverse( std::begin( adcs ), std::end( adcs ) ); }
  return true;
}

STTell1Board* STReadoutTool::findByBoardID( const STTell1ID aBoardID ) const {
  // find by board id
  try {
    return m_boardsMap.at( aBoardID );
  } catch ( std::out_of_range& e ) { return nullptr; }
}

STTell1Board* STReadoutTool::findByOrder( const unsigned int aValue ) const {
  // find by order
  return aValue < m_nBoard ? m_boards[aValue].get() : nullptr;
}

void STReadoutTool::printMapping() const {

  // dump out the readout mapping
  std::cout << "print mapping for: " << name() << " tool\n";
  std::cout << " Number of boards " << m_nBoard << '\n';
  for ( const auto& b : m_boards ) std::cout << *b << '\n';
}

/// Add the mapping of source ID to TELL1 board number
unsigned int STReadoutTool::SourceIDToTELLNumber( unsigned int sourceID ) const {
  return ( this->SourceIDToTELLNumberMap().find( sourceID ) )->second;
}

/// Add the mapping of TELL1 board number to source ID
unsigned int STReadoutTool::TELLNumberToSourceID( unsigned int TELL ) const {
  return ( this->TELLNumberToSourceIDMap().find( TELL ) )->second;
}

StatusCode STReadoutTool::validate() const {

  // validate the map - every sector must go somewhere !
  const DeSTDetector::Sectors& dSectors = m_tracker->sectors();
  return StatusCode{std::none_of( std::begin( dSectors ), std::end( dSectors ), [this]( const DeSTSector* s ) {
    STChannelID chan     = s->elementID();
    auto        chanPair = offlineChanToDAQ( chan, 0.0 );
    return chanPair.first == STTell1ID( STTell1ID::nullBoard, false );
  } )};
}

std::vector<LHCb::STChannelID> STReadoutTool::sectorIDs( const STTell1ID board ) const {

  std::vector<LHCb::STChannelID> sectors;
  sectors.reserve( 8 );
  STTell1Board* theBoard = findByBoardID( board );
  if ( theBoard ) {
    sectors.insert( sectors.begin(), theBoard->sectorIDs().begin(), theBoard->sectorIDs().end() );
  } else {
    Error( "Failed to find Board", StatusCode::SUCCESS, 100 );
  }
  return sectors;
}

std::vector<DeSTSector*> STReadoutTool::sectors( const STTell1ID board ) const {

  return m_tracker->findSectors( sectorIDs( board ) );
}

std::vector<DeSTSector*> STReadoutTool::sectorsOnServiceBox( const std::string& serviceBox ) const {

  return m_tracker->findSectors( sectorIDsOnServiceBox( serviceBox ) );
}

std::vector<LHCb::STChannelID> STReadoutTool::sectorIDsOnServiceBox( const std::string& serviceBox ) const {
  // loop over all boards
  std::vector<LHCb::STChannelID> sectors;
  sectors.reserve( 16 );
  for ( const auto& board : m_boards ) {
    const std::vector<LHCb::STChannelID>& sectorVec = board->sectorIDs();
    const std::vector<std::string>&       sBoxes    = board->serviceBoxes();
    for ( unsigned int iS = 0u; iS < board->nSectors(); ++iS ) {
      if ( sBoxes[iS] == serviceBox ) sectors.push_back( sectorVec[iS] );
    } // iS
  }   // iterB
  return sectors;
}

const std::vector<std::string>& STReadoutTool::serviceBoxes() const { return m_serviceBoxes; }

std::string STReadoutTool::footer() const {
  std::string temp = m_footer;
  temp.insert( 0, "</catalog>" );
  return temp;
}

std::string STReadoutTool::header( const std::string& conString ) const {
  // get the header
  std::string::size_type startpos = conString.find( m_startTag );
  std::string            temp     = conString.substr( 0, startpos );
  temp.insert( startpos, "<catalog name=\"ReadoutSectors\">" );

  // correct the location of the DTD
  if ( m_removeCondb ) {
    ST::XMLUtils::replace( temp, "conddb:", "" );
    std::string location;
    for ( unsigned int i = 0; i < m_depth; ++i ) location += "../";
    std::string::size_type pos = temp.find( "/DTD/" );
    temp.insert( pos, location );
    ST::XMLUtils::replace( temp, "//", "/" );
  }

  return temp;
}

std::string STReadoutTool::strip( const std::string& conString ) const {
  std::string::size_type startpos = conString.find( m_startTag );
  std::string::size_type endpos   = conString.find( m_footer );
  return conString.substr( startpos, endpos - startpos );
}
