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
#ifndef _UTREADOUTTOOL_H
#define _UTREADOUTTOOL_H

#include <vector>
#include <fstream>
#include <string>

#include "Kernel/IUTReadoutTool.h"
#include "Kernel/UTChannelID.h"
#include "GaudiAlg/GaudiTool.h"
#include "Kernel/UTTell1ID.h"
#include "Kernel/UTDAQDefinitions.h"
#include "Kernel/UTChannelID.h"
#include "Kernel/UTTell1Board.h"
#include "Event/UTCluster.h"
#include "UTDet/DeUTDetector.h"

/**
 *  Concret Class for things related to the Readout of the UT Tell1 Boards
 */

class UTReadoutTool: public extends<GaudiTool, IUTReadoutTool> {

public:

  /// Constructer
  UTReadoutTool(const std::string& type,
                const std::string& name,
                const IInterface* parent);

  /// nBoard
  unsigned int nBoard() const override;

  ///return vector of Tell1IDs
  std::vector<UTTell1ID> boardIDs() const override;

  /// convert ITChannelID to DAQ ChannelID
  UTDAQ::chanPair offlineChanToDAQ(const LHCb::UTChannelID aOfflineChan, double isf) const override;

  /// convert offline interStripFraction to DAQ interStripFraction
  double interStripToDAQ(const LHCb::UTChannelID aOfflineChan,
                                 const UTTell1ID aBoardID,
                                 const double isf) const override;

  bool ADCOfflineToDAQ(const LHCb::UTChannelID aOfflineChan,
                       const UTTell1ID aBoardID,
                       LHCb::UTCluster::ADCVector& adcs) const override;


  /// find the Tell1 board given a board ID
  UTTell1Board* findByBoardID(const UTTell1ID aBoardID) const override;

  /// find Tell1 board by storage order
  UTTell1Board* findByOrder(const unsigned int aValue) const override;

  /// Add the mapping of source ID to TELL1 board number
  unsigned int SourceIDToTELLNumber(unsigned int sourceID) const override;

  /** Add the mapping of source ID to board number for UT */
  const  std::map<unsigned int, unsigned int>& SourceIDToTELLNumberMap() const override;



  
  
  /// list of the readout sector ids on the board
  std::vector<LHCb::UTChannelID> sectorIDs(const UTTell1ID board) const override;

  /// list of the readout sectors
  std::vector<DeUTSector*> sectors(const UTTell1ID board) const override;

  /// service box
  unsigned int nServiceBox() const override;

  /// service box number
  std::string serviceBox(const LHCb::UTChannelID& aChan) const override;

  /// list of the readout sectors ids in a service box
  std::vector<LHCb::UTChannelID> sectorIDsOnServiceBox(const std::string& serviceBox) const override;

  /// list of the readout sectors in a service box
  std::vector<DeUTSector*> sectorsOnServiceBox(const std::string& serviceBox) const override;

  /// list of service boxes
  const std::vector<std::string>& serviceBoxes() const override;

  /// Add the mapping of TELL1 board number to source ID
  unsigned int TELLNumberToSourceID(unsigned int TELL) const override;

  /// print mapping
  void printMapping() const override;

  /// write out the mapping as xml
  StatusCode writeMappingToXML() const override;

  StatusCode validate() const;

  /// finalize
  StatusCode finalize() override;

  /// init
  StatusCode initialize() override;

  /// get region
  unsigned int region(const LHCb::UTChannelID aChan) const override;

  /** Add the mapping of board number to source ID for UT */
  const  std::map<unsigned int, unsigned int>& TELLNumberToSourceIDMap() const override;

protected:

  void clear();

  std::string footer() const;
  std::string header(const std::string& conString) const;
  std::string strip(const std::string& conString) const;


  unsigned int m_hybridsPerBoard;
  unsigned int m_nBoard{0};
  unsigned int m_nServiceBox;
  std::vector<std::unique_ptr<UTTell1Board>> m_boards;
  std::map<UTTell1ID, UTTell1Board*> m_boardsMap;
  std::vector<std::string> m_serviceBoxes;
  std::vector<unsigned int> m_firstBoardInRegion;

  Gaudi::Property<bool> m_printMapping { this, "printMapping", false };
  DeUTDetector* m_tracker = nullptr;
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

private:

  StatusCode createBoards();
  StatusCode createTell1Map();


  unsigned int m_nRegionA = 512;
  unsigned int m_firstStation = 512;

};






#endif // _UTREADOUTTOOL_H
