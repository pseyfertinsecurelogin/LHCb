#ifndef _FTReadoutTool_H
#define _FTReadoutTool_H 1

// STL
#include <vector>
#include <fstream>
#include <string>

// Gaudi
#include "GaudiAlg/GaudiTool.h"

#include "FTTell40ID.h"
#include "Kernel/FTChannelID.h"
#include "IFTReadoutTool.h"

/** @class FTReadoutTool FTReadoutTool.h
 *
 *  Concrete class for things related to the Readout - Tell40 Board
 *
 *  @author L.Henry
 *  @date   20/10/2017
*/

class FTReadoutTool : public extends<GaudiTool, IFTReadoutTool>{
 public:
	using base_class::base_class;	

	//Ctor
	FTReadoutTool(const std::string& type,
				  const std::string& name,
				  const IInterface* parent);

	/// init
	StatusCode initialize() override;
	
	/// validate
	//	StatusCode validate() const;

	/// get station,layer,quarter
	//	unsigned int region (const LHCb::FTChannelID aChan) const override;
	unsigned int station(const LHCb::FTChannelID aChan) const override;
	unsigned int layer  (const LHCb::FTChannelID aChan) const override;
	unsigned int quarter(const LHCb::FTChannelID aChan) const override;
	unsigned int uniqueQuarter(const LHCb::FTChannelID aChan) const override;
	unsigned int mat    (const LHCb::FTChannelID aChan) const override;
	unsigned int sipm   (const LHCb::FTChannelID aChan) const override;
	
	unsigned int nBoard() const override;

	/** Add the mapping of source ID to board number for FT */
	//  const  std::map<unsigned int, unsigned int>& SourceIDToTELLNumberMap() const override;
	
	/** Add the mapping of board number to source ID for FT */
	//  const  std::map<unsigned int, unsigned int>& TELLNumberToSourceIDMap() const override;
	
	/// printout
	void printMapping() const override;
	
	/// write an xml file
	StatusCode writeMappingToXML() const override;

	std::string m_conditionLocation;
	
 private:
	void clear();
	StatusCode createBoards();

	//	unsigned int m_nRegionA     = 512;
	unsigned int m_firstStation = 512;

	unsigned int m_passedHeaders;//LoH

	bool m_printMapping = false;
	
	Gaudi::Property<std::string> m_outputFileName { this, "outputFile","ReadoutMap.xml"};
	unsigned int m_hybridsPerBoard;
	unsigned int m_nBoard;
	//	std::vector<std::unique_ptr<FTTell40Board> > m_boards;
	std::vector<std::unique_ptr<FTTell40ID> > m_boards;
};

#endif // _FTReadoutTool_H
