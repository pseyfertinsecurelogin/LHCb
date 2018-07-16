#ifndef _IUTReadoutTool_H
#define _IUTReadoutTool_H

#include "GaudiKernel/IAlgTool.h"

#include <string>
#include <vector>
#include "Kernel/STDAQDefinitions.h"
#include "Kernel/STTell1ID.h"
#include "Event/STCluster.h"

class STTell1Board;
class DeSTSector;

namespace LHCb{
 class STChannelID;
}

struct IUTReadoutTool: extend_interfaces<IAlgTool>{

  /// Static access to interface id
  DeclareInterfaceID(IUTReadoutTool, 1 , 0);

  /// number of boards
  virtual unsigned int nBoard() const = 0;


  ///return vector of Tell1IDs
  virtual std::vector<STTell1ID> boardIDs() const = 0;

  /// convert STChannelID to DAQ ChannelID
  virtual STDAQ::chanPair offlineChanToDAQ(const LHCb::STChannelID aOfflineChan,
                                           double isf) const = 0 ;

  /// convert offline interStripFraction to DAQ interStripFraction
  virtual double interStripToDAQ(const LHCb::STChannelID aOfflineChan, 
                                 const STTell1ID aBoardID,
                                 const double isf) const = 0 ;

  virtual bool ADCOfflineToDAQ(const LHCb::STChannelID aOfflineChan,
			       const STTell1ID aBoardID,
			       LHCb::STCluster::ADCVector& adcs) const = 0;
  
  /// find the Tell1 board given a board ID
  virtual STTell1Board* findByBoardID(const STTell1ID aBoardID) const = 0;

  /// find a Tell board by storage order
  virtual STTell1Board* findByOrder(const unsigned int aValue) const = 0;

  /// list of the readout sector ids on the board in a board
  virtual std::vector<LHCb::STChannelID> sectorIDs(const STTell1ID board) const = 0;

  /// list of the readout sectors in a board
  virtual std::vector<DeSTSector*> sectors(const STTell1ID board) const = 0;

  /// service box
  virtual unsigned int nServiceBox() const = 0;

  /// service box number
  virtual std::string serviceBox(const LHCb::STChannelID& aChan) const = 0;

  /// list of the readout sectors ids in a service box 
  virtual std::vector<LHCb::STChannelID> sectorIDsOnServiceBox(const std::string& serviceBox) const = 0;
  
  /// list of the readout sectors in a service box
  virtual std::vector<DeSTSector*> sectorsOnServiceBox(const std::string& serviceBox) const = 0;

  /// list of service boxes
  virtual const std::vector<std::string>& serviceBoxes() const = 0;
 
  /// Add the mapping of source ID to TELL1 board number
  virtual const  std::map<unsigned int, unsigned int>& SourceIDToTELLNumberMap() const = 0; 

  /// Add the mapping of source ID to TELL1 board number
  virtual unsigned int SourceIDToTELLNumber(unsigned int sourceID) const = 0; 

  /// Add the mapping of TELL1 board number to source ID
  virtual unsigned int TELLNumberToSourceID(unsigned int sourceID) const = 0;

  /// printout
  virtual void printMapping() const = 0;

  /// write an xml file
  virtual StatusCode writeMappingToXML() const = 0;
  
};


#endif // _IUTReadoutTool_H
