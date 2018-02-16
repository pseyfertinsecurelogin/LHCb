#ifndef _IFTReadoutTool_H
#define _IFTReadoutTool_H 1

#include "GaudiKernel/IAlgTool.h"

#include <string>
#include <vector>
#include "Event/FTCluster.h"

//namespace LHCb{
//	class FTChannelID;
//}

/** @class IFTReadoutTool IFTReadoutTool.h  FTDAQ/IFTReadoutTool
*
*  Interface Class for things related to the Readout - Tell1 Board
*
*  @author L.Henry
*  @date   07/11/2017
*/

// Declaration of the interface ID ( interface id, major version, minor version)

struct IFTReadoutTool: extend_interfaces< IAlgTool >{

	/// Static access to interface id
	DeclareInterfaceID(IFTReadoutTool, 1 , 0);
	
	/// number of boards
	virtual unsigned int nBoard() const = 0;
	
	/// get station,layer,quarter
	//	virtual unsigned int region (const LHCb::FTChannelID aChan) const = 0;
	virtual unsigned int station(const LHCb::FTChannelID aChan) const = 0;
	virtual unsigned int layer  (const LHCb::FTChannelID aChan) const = 0;
	virtual unsigned int quarter(const LHCb::FTChannelID aChan) const = 0 ;
	virtual unsigned int uniqueQuarter(const LHCb::FTChannelID aChan) const = 0 ;
	virtual unsigned int mat    (const LHCb::FTChannelID aChan) const = 0;
	virtual unsigned int sipm   (const LHCb::FTChannelID aChan) const = 0;	
	
	/// convert FTChannelID to DAQ ChannelID
	//  virtual FTDAQ::chanPair offlineChanToDAQ(const LHCb::FTChannelID aOfflineChan,
	//                                           double isf) const = 0 ;
	
	/// convert offline interStripFraction to DAQ interStripFraction
	//  virtual double interStripToDAQ(const LHCb::FTChannelID aOfflineChan, 
	//                                 const FTTell1ID aBoardID,
	//                                 const double isf) const = 0 ;
	
	//  virtual bool ADCOfflineToDAQ(const LHCb::FTChannelID aOfflineChan,
	//			       const FTTell1ID aBoardID,
	//			       LHCb::FTCluster::ADCVector& adcs) const = 0;
	
	/// find the Tell1 board given a board ID
	//  virtual FTTell1Board* findByBoardID(const FTTell1ID aBoardID) const = 0;
	
	/// find a Tell board by storage order
	//  virtual FTTell1Board* findByOrder(const unsigned int aValue) const = 0;
	
	/// service box
	//  virtual unsigned int nServiceBox() const = 0;
	
	/// service box number
	//  virtual std::string serviceBox(const LHCb::FTChannelID& aChan) const = 0; 
	
	/// printout
	virtual void printMapping() const = 0;
	
	/// write an xml file
	virtual StatusCode writeMappingToXML() const = 0;
	
	/// list of the readout sector ids on the board in a board
	//  virtual std::vector<LHCb::FTChannelID> sectorIDs(const FTTell1ID board) const = 0;
	
	/// list of the readout sectors in a board
	//  virtual std::vector<DeFTSector*> sectors(const FTTell1ID board) const = 0;
	
	/// list of the readout sectors ids in a service box 
	//  virtual std::vector<LHCb::FTChannelID> sectorIDsOnServiceBox(const std::string& serviceBox) const = 0;
	
	/// list of the readout sectors in a service box
	//  virtual std::vector<DeFTSector*> sectorsOnServiceBox(const std::string& serviceBox) const = 0;
	
	/// list of service boxes
	//  virtual const std::vector<std::string>& serviceBoxes() const = 0;
	
	/// Add the mapping of source ID to TELL1 board number
	//  virtual const  std::map<unsigned int, unsigned int>& SourceIDToTELLNumberMap() const = 0; 
	
	/// Add the mapping of source ID to TELL1 board number
	//  virtual unsigned int SourceIDToTELLNumber(unsigned int sourceID) const = 0; 
	
	/// Add the mapping of TELL1 board number to source ID
	//  virtual const  std::map<unsigned int, unsigned int>& TELLNumberToSourceIDMap() const = 0; 
	
	/// Add the mapping of TELL1 board number to source ID
	//  virtual unsigned int TELLNumberToSourceID(unsigned int sourceID) const = 0; 
	
};


#endif // _IFTReadoutTool_H
