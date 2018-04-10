#ifndef _UTREADOUTTOOL_H
#define _UTREADOUTTOOL_H

#include <vector>
#include <fstream>
#include <string>

#include "Kernel/STChannelID.h"
#include "GaudiAlg/GaudiTool.h"
#include "Kernel/STTell1ID.h"
#include "Kernel/STDAQDefinitions.h"
#include "Kernel/STChannelID.h"
#include "Kernel/STTell1Board.h"
#include "Event/STCluster.h"
#include "STDet/DeSTDetector.h"

/**
 *  Concret Class for things related to the Readout of the UT Tell1 Boards
 */

class UTReadoutTool: public GaudiTool {

public:

  /// Constructer
  UTReadoutTool(const std::string& type,
                const std::string& name,
                const IInterface* parent);

  /// nBoard
  unsigned int nBoard() const;

  ///return vector of Tell1IDs
  std::vector<STTell1ID> boardIDs() const;

  /// convert ITChannelID to DAQ ChannelID
  STDAQ::chanPair offlineChanToDAQ(const LHCb::STChannelID aOfflineChan, double isf) const;

  /// convert offline interStripFraction to DAQ interStripFraction
  double interStripToDAQ(const LHCb::STChannelID aOfflineChan,
                                 const STTell1ID aBoardID,
                                 const double isf) const;

  bool ADCOfflineToDAQ(const LHCb::STChannelID aOfflineChan,
                       const STTell1ID aBoardID,
                       LHCb::STCluster::ADCVector& adcs) const;


  /// find the Tell1 board given a board ID
  STTell1Board* findByBoardID(const STTell1ID aBoardID) const;

  /// find Tell1 board by storage order
  STTell1Board* findByOrder(const unsigned int aValue) const;

  /// Add the mapping of source ID to TELL1 board number
  unsigned int SourceIDToTELLNumber(unsigned int sourceID) const;

  /** Add the mapping of source ID to board number for IT / TT / UT */
  const  std::map<unsigned int, unsigned int>& SourceIDToTELLNumberMap() const;



  
  
  /// list of the readout sector ids on the board
  std::vector<LHCb::STChannelID> sectorIDs(const STTell1ID board) const;

  /// list of the readout sectors
  std::vector<DeSTSector*> sectors(const STTell1ID board) const;

  /// service box
  unsigned int nServiceBox() const;

  /// service box number
  std::string serviceBox(const LHCb::STChannelID& aChan) const;

  /// list of the readout sectors ids in a service box
  std::vector<LHCb::STChannelID> sectorIDsOnServiceBox(const std::string& serviceBox) const;

  /// list of the readout sectors in a service box
  std::vector<DeSTSector*> sectorsOnServiceBox(const std::string& serviceBox) const;

  /// list of service boxes
  const std::vector<std::string>& serviceBoxes() const;

  /// Add the mapping of TELL1 board number to source ID
  unsigned int TELLNumberToSourceID(unsigned int TELL) const;

  /// print mapping
  void printMapping() const;

  /// write out the mapping as xml
  StatusCode writeMappingToXML() const;

  StatusCode validate() const;

  /// finalize
  StatusCode finalize() override;

  /// init
  StatusCode initialize() override;

  /// get region
  unsigned int region(const LHCb::STChannelID aChan) const;

  /** Add the mapping of board number to source ID for IT / TT / UT */
  const  std::map<unsigned int, unsigned int>& TELLNumberToSourceIDMap() const;

protected:

  void clear();

  std::string footer() const;
  std::string header(const std::string& conString) const;
  std::string strip(const std::string& conString) const;


  unsigned int m_hybridsPerBoard;
  unsigned int m_nBoard{0};
  unsigned int m_nServiceBox;
  std::vector<std::unique_ptr<STTell1Board>> m_boards;
  std::map<STTell1ID, STTell1Board*> m_boardsMap;
  std::vector<std::string> m_serviceBoxes;
  std::vector<unsigned int> m_firstBoardInRegion;

  Gaudi::Property<bool> m_printMapping { this, "printMapping", false };
  DeSTDetector* m_tracker = nullptr;
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
