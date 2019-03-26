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
#include "Kernel/UTTell1Board.h"
#include "Kernel/LHCbConstants.h"
#include "Kernel/UTDAQDefinitions.h"

#include "Event/UTCluster.h"
#include "Kernel/UTNames.h"

using namespace LHCb;

UTTell1Board::UTTell1Board( const UTTell1ID aBoard, const unsigned int nStripsPerHybrid, const std::string& detType )
    : m_boardID( aBoard ), m_nStripsPerHybrid( nStripsPerHybrid ), m_detType( detType ) {
  // constructer
  m_sectorsVector.reserve( 8 );
  m_sectorsVectorOpt.reserve( 8 );
}

void UTTell1Board::addSector( UTChannelID aOfflineChan, unsigned int orientation, const std::string& serviceBox ) {
  // add sector to vector
  m_orientation.push_back( orientation );
  m_sectorsVector.push_back( aOfflineChan );
  m_sectorsVectorOpt.push_back( {aOfflineChan.station(), aOfflineChan.layer(), aOfflineChan.detRegion(),
                                 aOfflineChan.sector(), aOfflineChan.uniqueSector(), (unsigned int)aOfflineChan} );
  m_serviceBoxVector.push_back( serviceBox );
}

bool UTTell1Board::isInside( const UTChannelID aOfflineChan, unsigned int& sectorIndex ) const {
  // look-up the board
  unsigned int iSector = 0u;

  while ( ( iSector < m_sectorsVector.size() ) &&
          ( aOfflineChan.uniqueSector() != m_sectorsVector[iSector].uniqueSector() ) ) {
    ++iSector;
  } // iSector
  sectorIndex = iSector;

  return iSector != m_sectorsVector.size();
}

UTTell1Board::chanPair UTTell1Board::DAQToOffline( const unsigned int fracStrip, const UTDAQ::version& version,
                                                   const UTDAQ::UTStripRepresentation aDAQChan ) const {
  auto  full       = DAQToOfflineFull( fracStrip, version, aDAQChan );
  auto& fullChan   = std::get<0>( full );
  auto& strip      = std::get<1>( full );
  auto& interStrip = std::get<2>( full );

  return {UTChannelID( UTChannelID{(int)fullChan.chanID}.type(), fullChan.station, fullChan.layer, fullChan.detRegion,
                       fullChan.sector, strip ),
          interStrip};
}

std::tuple<UTTell1Board::ExpandedChannelID, unsigned int, int>
UTTell1Board::DAQToOfflineFull( const unsigned int fracStrip, const UTDAQ::version& version,
                                unsigned int aDAQChan ) const {

  // convert a DAQ channel to offline !
  const unsigned int index = aDAQChan / m_nStripsPerHybrid;
  unsigned int       strip = aDAQChan - ( index * m_nStripsPerHybrid );

  int interstrip = fracStrip;

  if ( m_orientation[index] == 0 ) {
    // reverse direction of strip numbering
    strip = m_nStripsPerHybrid - strip;
    // shift channel by one, because interstrip fraction cannot be negative
    if ( fracStrip != 0 && version >= UTDAQ::v4 ) {
      interstrip = 4 - fracStrip;
      --strip;
    }
  } else { // Add one because offline strips start at one.
    ++strip;
  }

  return {m_sectorsVectorOpt[index], strip, interstrip};
}

void UTTell1Board::ADCToOffline( const unsigned int aDAQChan, UTCluster::ADCVector& adcs, const int version,
                                 const unsigned int offset, const unsigned int fracStrip ) const {
  const unsigned int index     = aDAQChan / m_nStripsPerHybrid;
  const unsigned int size      = adcs.size();
  int                newoffset = 0;

  // flip sequence of adc vector
  if ( m_orientation[index] == 0 && version >= UTDAQ::v4 ) {
    // calculate the new offset
    newoffset = size - 1 - offset;
    // Correct for interstrip fraction when not equal to zero
    if ( fracStrip != 0u ) { --newoffset; }
    if ( newoffset < 0 ) newoffset = 0;

    // Do the actual flipping
    UTCluster::ADCVector adcsflip = adcs;
    for ( unsigned int i = 0u; i < size; ++i ) {
      adcsflip[size - i - 1]       = adcs[i];
      adcsflip[size - i - 1].first = size - i - 1 - newoffset;
    }

    adcs = adcsflip;
  }
}

unsigned int UTTell1Board::offlineToDAQ( const UTChannelID aOfflineChan, const unsigned int waferIndex,
                                         double isf ) const {
  // convert an offline channel to DAQ channel
  unsigned int strip = aOfflineChan.strip();
  if ( m_orientation[waferIndex] == 0 ) {
    // change the direction of numbering
    strip = m_nStripsPerHybrid - strip;
    // shift channel by one, because interstrip fraction cannot be negative
    if ( isf > 0.01 ) --strip;
  } else {
    // subtract one because offline strips start at one.
    --strip;
  }

  return ( waferIndex * m_nStripsPerHybrid ) + strip;
}

std::string UTTell1Board::serviceBox( const UTDAQ::UTStripRepresentation& tell1Chan ) const {

  const unsigned int index = tell1Chan.value() / m_nStripsPerHybrid;
  return serviceBox( index );
}

std::ostream& UTTell1Board::fillStream( std::ostream& os ) const {

  // print function
  os << m_detType + "Tell1: " << flatTell1Number() << std::endl;
  os << " Source ID Board: " << m_boardID.id() << " region " << m_boardID.region() << " subID" << m_boardID.subID()
     << std::endl;
  os << "# readout sectors " << m_sectorsVector.size() << std::endl;

  unsigned int wafer = 0u;
  for ( auto iterW = m_sectorsVector.begin(); iterW != m_sectorsVector.end(); ++iterW, ++wafer ) {

    if ( *iterW == LHCb::UTChannelID( 0 ) ) {
      // link is not loaded
      os << "Unloaded Link" << std::endl;
      continue;
    }

    os << UTNames().UniqueSectorToString( *iterW ) << " " << serviceBox( wafer ) << '\n';

  } // iW
  return os << " -----------\n";
}

#include "Kernel/UTBoardMapping.h"

unsigned int UTTell1Board::flatTell1Number() const {
  return UTBoardMapping::find( boardID().id(), UTBoardMapping::UTSourceIDToNumberMap() );
}
