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
#ifndef _STReadoutTool_H
#define _STReadoutTool_H

// STL
#include <vector>
#include <fstream>
#include <string>

// Gaudi
#include "GaudiAlg/GaudiTool.h"

// STDAQ
#include "Kernel/ISTReadoutTool.h"
#include "Kernel/STTell1ID.h"
#include "Kernel/STDAQDefinitions.h"

// ST
#include "Kernel/STChannelID.h"


/** @class STReadoutTool STReadoutTool.h  STDAQ/STReadoutTool
 *
 *  Concret Class for things related to the Readout - Tell1 Board
 *
 *  @author M.Needham
 *  @date   13/3/2002
*/

class DeSTDetector;

class STReadoutTool: public extends<GaudiTool, ISTReadoutTool> {

public:

  /// Constructer
  STReadoutTool(const std::string& type,
                const std::string& name,
                const IInterface* parent);

  /// init
  StatusCode initialize() override;

  /// nBoard
  unsigned int nBoard() const override;

  ///return vector of Tell1IDs
  std::vector<STTell1ID> boardIDs() const override;

  /// convert ITChannelID to DAQ ChannelID
  STDAQ::chanPair offlineChanToDAQ(const LHCb::STChannelID aOfflineChan, double isf) const override;

/// convert offline interStripFraction to DAQ interStripFraction
  double interStripToDAQ(const LHCb::STChannelID aOfflineChan,
                                 const STTell1ID aBoardID,
                                 const double isf) const override;

  bool ADCOfflineToDAQ(const LHCb::STChannelID aOfflineChan,
                       const STTell1ID aBoardID,
                       LHCb::STCluster::ADCVector& adcs) const override;


  /// find the Tell1 board given a board ID
  STTell1Board* findByBoardID(const STTell1ID aBoardID) const override;

  /// find Tell1 board by storage order
  STTell1Board* findByOrder(const unsigned int aValue) const override;

  /// list of the readout sector ids on the board
  std::vector<LHCb::STChannelID> sectorIDs(const STTell1ID board) const override;

  /// list of the readout sectors
  std::vector<DeSTSector*> sectors(const STTell1ID board) const override;

  /// service box
  unsigned int nServiceBox() const override;

  /// service box number
  std::string serviceBox(const LHCb::STChannelID& aChan) const override;

  /// list of the readout sectors ids in a service box
  std::vector<LHCb::STChannelID> sectorIDsOnServiceBox(const std::string& serviceBox) const override;

  /// list of the readout sectors in a service box
  std::vector<DeSTSector*> sectorsOnServiceBox(const std::string& serviceBox) const override;

  /// list of service boxes
  const std::vector<std::string>& serviceBoxes() const override;

  /// Add the mapping of source ID to TELL1 board number
  unsigned int SourceIDToTELLNumber(unsigned int sourceID) const override;

  /// Add the mapping of TELL1 board number to source ID
  unsigned int TELLNumberToSourceID(unsigned int TELL) const override;

  /// print mapping
  void printMapping() const override;

  /// write out the mapping as xml
  StatusCode writeMappingToXML() const override;

  StatusCode validate() const;

  /// finalize
  StatusCode finalize() override;

protected:

  void clear();

  std::string footer() const;
  std::string header(const std::string& conString) const;
  std::string strip(const std::string& conString) const;


  unsigned int m_hybridsPerBoard;
  unsigned int m_nBoard = 0;
  unsigned int m_nServiceBox;
  std::vector<std::unique_ptr<STTell1Board>> m_boards;
  std::map<STTell1ID, STTell1Board*> m_boardsMap;
  std::vector<std::string> m_serviceBoxes;
  std::vector<unsigned int> m_firstBoardInRegion;

  Gaudi::Property<bool> m_printMapping { this, "printMapping", false };
  DeSTDetector* m_tracker = nullptr;
  std::string m_detType;
  std::string m_conditionLocation;

private:

  Gaudi::Property<bool> m_writeXML { this, "writeMappingToXML", false };
  Gaudi::Property<std::string> m_footer { this, "footer", "</DDDB>"};
  Gaudi::Property<std::string> m_startTag { this, "startTag", "<condition"};
  Gaudi::Property<std::string> m_outputFileName { this, "outputFile","ReadoutMap.xml"};
  std::ofstream m_outputFile;
  Gaudi::Property<std::string> m_author { this, "author", "Joe Bloggs"};
  Gaudi::Property<std::string> m_tag { this, "tag", "None"};
  Gaudi::Property<std::string> m_desc { this, "description", "BlahBlahBlah"};
  Gaudi::Property<bool> m_removeCondb { this, "removeCondb", false};
  Gaudi::Property<unsigned int> m_precision { this, "precision", 16u};
  Gaudi::Property<unsigned int> m_depth { this, "depths", 3u };

};



#endif // _STReadoutTool_H
