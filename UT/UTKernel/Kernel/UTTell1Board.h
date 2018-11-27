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
#ifndef _UTTell1Board_H
#define _UTTell1Board_H 1

#include "Kernel/UTTell1ID.h"
#include "Kernel/UTChannelID.h"
#include "Kernel/UTDAQDefinitions.h"
#include "Kernel/UTStripRepresentation.h"
#include <vector>
#include <iostream>

#include "Event/UTCluster.h"

/** @class UTTell1Board UTTell1Board.h "UTDAQ/UTTell1Board.h"
 *
 *  Class for converting from UTChannelID to daq channel/UTTell1ID
 *
 *  @author A. Beiter (based on code by M.Needham)
 *  @date   2018-09-04
 */

class UTTell1Board final {

public:

  struct ExpandedChannelID {
    unsigned int station;
    unsigned int layer;
    unsigned int detRegion;
    unsigned int sector;
    unsigned int uniqueSector;
    unsigned int chanID;
  };
  typedef std::pair<LHCb::UTChannelID,int> chanPair;

  /// constructer
  UTTell1Board(const UTTell1ID aBoard,
               const unsigned int stripsPerHybrid,
               const std::string& type);

  /// add wafer
  void addSector(LHCb::UTChannelID aOfflineChan, unsigned int orientation, const std::string& serviceBox);

  /// board indentifer
  UTTell1ID boardID() const;

  /// same board id
  bool sameID(const UTTell1ID& id) const;

  /// flat tell1 number as in itell1XXX etc
  unsigned int flatTell1Number() const;

  /// channel is in this board
  bool isInside(const LHCb::UTChannelID aOfflineChan,
                unsigned int& sectorIndex) const;

  /// construct LHCb::UTChannelID from DAQ Channel
  chanPair DAQToOffline(const unsigned int fracStrip,
			const UTDAQ::version& version,
                        const UTDAQ::UTStripRepresentation aDAQChan) const;

  /// construct LHCb::UTChannelID from DAQ Channel
  std::tuple<UTTell1Board::ExpandedChannelID, unsigned int, int>
  DAQToOfflineFull(const unsigned int fracStrip,
                   const UTDAQ::version& version,
                   unsigned int aDAQChan) const;

  /// fill adc values offline
  void ADCToOffline(const unsigned int aDAQChan,
		      LHCb::UTCluster::ADCVector& adcs,
		      const int version,
		      const unsigned int offset,
		      const unsigned int fracStrip) const;


  /// construct DAQChannel from LHCb::UTChannelID
  unsigned int offlineToDAQ(const LHCb::UTChannelID aOfflineChan,
                            const unsigned int sectorIndex,
			    double isf) const;

  /// check channel is valid
  bool validChannel(const unsigned int daqChan) const;

  /// vector of sectors on the board
  const std::vector<LHCb::UTChannelID>& sectorIDs() const;

  /// vector of hybrid orientations
  const std::vector<int>& orientation() const;

  /// vector of service boxes
  const std::vector<std::string>& serviceBoxes() const;

  /// service box
  std::string serviceBox(const LHCb::UTChannelID& chan) const;

  /// service box
  std::string serviceBox(const UTDAQ::UTStripRepresentation& tell1Chan) const;

  /// number of readout sectors
  unsigned int nSectors() const;

  /// Operator overloading for stringoutput
  friend std::ostream& operator<< (std::ostream& s, const UTTell1Board& obj)
  {
    return obj.fillStream(s);
  }

  // Fill the ASCII output stream
  std::ostream& fillStream(std::ostream& s) const;

  /** print method for python Not needed in C++ */
  std::string toString() const;

private:

  /// service box
  std::string serviceBox(const unsigned int& waferIndex) const;

  UTTell1ID m_boardID;
  unsigned int m_nStripsPerHybrid;
  std::vector<LHCb::UTChannelID> m_sectorsVector;
  std::vector<ExpandedChannelID> m_sectorsVectorOpt;
  std::vector<int> m_orientation;
  std::vector<std::string> m_serviceBoxVector;
  std::string m_detType;

};

#include <sstream>
#include <string>

inline std::string UTTell1Board::toString() const{
  std::ostringstream o;
  fillStream(o);
  return o.str();
}

inline UTTell1ID UTTell1Board::boardID() const{
  return m_boardID;
}

inline bool UTTell1Board::sameID(const UTTell1ID& id) const{
  return m_boardID == id ? true : false;
}

inline const std::vector<int>& UTTell1Board::orientation() const{
  return m_orientation;
}

inline const std::vector<std::string>& UTTell1Board::serviceBoxes() const{
  return m_serviceBoxVector;
}

inline const std::vector<LHCb::UTChannelID>& UTTell1Board::sectorIDs() const{
  return m_sectorsVector;
}

inline std::string UTTell1Board::serviceBox(const unsigned int& waferIndex) const {
  return (waferIndex < m_serviceBoxVector.size() ? m_serviceBoxVector[waferIndex] : std::string("Unknown"));
}

inline std::string UTTell1Board::serviceBox(const LHCb::UTChannelID& chan) const{
  unsigned int waferIndex;
  isInside(chan, waferIndex);
  return serviceBox(waferIndex);
}

inline bool UTTell1Board::validChannel(const unsigned int daqChan) const{
  if (daqChan > m_nStripsPerHybrid*m_sectorsVector.size()) return false;
  const int index = daqChan/m_nStripsPerHybrid;
  return m_sectorsVector[index].sector() != 0;
}

inline unsigned int UTTell1Board::nSectors() const{
  return m_sectorsVector.size();
}

#endif // _UTTell1Board_H
