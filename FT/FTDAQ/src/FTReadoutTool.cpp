// FTDAQ
#include "FTReadoutTool.h"

// FT
#include "Kernel/FTChannelID.h"

#include <algorithm>

// Det Desc
#include "DetDesc/Condition.h"

using namespace LHCb;

DECLARE_TOOL_FACTORY( FTReadoutTool )

//    //Main initialisation/destruction methods
// Constructor
FTReadoutTool::FTReadoutTool(const std::string& type,
							 const std::string& name,
							 const IInterface* parent)
: base_class( type, name, parent )
{
	m_boards.reserve(100);
	declareProperty( "conditionLocation",
					 m_conditionLocation  = "/dd/Conditions/ReadoutConf/FT/ReadoutMap" );
}

//Initialisation
StatusCode FTReadoutTool::initialize() {	
	// initialization phase...
	StatusCode sc = FTReadoutTool::initialize();
	if (sc.isFailure()){
		return Error("Failed to initialize", sc);
	}
	
	registerCondition(m_conditionLocation,
					  &FTReadoutTool::createBoards);
	sc = runUpdate(); // force update
	if (sc.isFailure()) return Error ( "Failed first UMS update for readout tool", sc );
	
	if (m_printMapping == true){
		printMapping();
	}

	m_passedHeaders = 0;

	return StatusCode::SUCCESS;
}

//Building the detector as described in conditions
StatusCode FTReadoutTool::createBoards() {	
	clear();	
	// load conditions
	//	Condition* rInfo = getDet<Condition>(m_conditionLocation);
	
	// vector of layer types
	//	const std::vector<std::string>& stations =
	//		rInfo->param<std::vector<std::string> >("stations");
	
	// vector of layer types
	
	//	m_hybridsPerBoard = rInfo->param<int>("hybridsPerBoard");
	//	unsigned int nBoardPerStation = rInfo->param<int>("nBoard");
	//	unsigned int nStripsPerHybrid =  FTDAQ::nStripsPerBoard/m_hybridsPerBoard;
	//	m_nBoard = nBoardPerStation*stations.size();
	
	
	//	unsigned int iReg = 0;
	/*
	for (auto iterS = stations.begin(); iterS != stations.end(); ++iterS, ++iReg){
		m_firstBoardInRegion.push_back(m_boards.size());
		
		const std::vector<int>& tMap = rInfo->param<std::vector<int> >(*iterS);
		const std::vector<int>& orientation = rInfo->param<std::vector<int> >(*iterS+"HybridOrientation");
		const std::vector<std::string>& serviceBoxes = rInfo->param<std::vector<std::string> >(*iterS+"ServiceBox");
		
		unsigned int vecLoc = 0;
		
		if ( iterS == stations.begin()){
			STChannelID firstChan = STChannelID(tMap[0]);
			m_firstStation = firstChan.station() ;
		}
		
		for (unsigned int iBoard = 0; iBoard < nBoardPerStation; ++iBoard){
			
			// make new board
			STTell1ID anID = STTell1ID(iReg,iBoard);
			auto aBoard = std::make_unique<STTell1Board>(anID,nStripsPerHybrid, m_detType );
			
			for (unsigned iH = 0 ; iH < m_hybridsPerBoard; ++iH, ++vecLoc){
				STChannelID sectorID((unsigned int)tMap[vecLoc]);
				aBoard->addSector(sectorID, (unsigned int)orientation[vecLoc], serviceBoxes[vecLoc]);
				
				
				// add to the list of service boxs if not already there
				if (std::find(m_serviceBoxes.begin(), m_serviceBoxes.end(),
							  serviceBoxes[vecLoc]) ==  m_serviceBoxes.end()) {
					m_serviceBoxes.push_back(serviceBoxes[vecLoc]);
				}
				
			} // iH
			
			m_boards.push_back(std::move(aBoard));
			
		} // boards per region
	} // iterS
	*/
	
	// validate the mapping --> all sectors should go somewhere !
	//	StatusCode sc = validate();
	//	if (sc.isFailure() ){
	//		return Error("Failed to validate mapping",StatusCode::FAILURE);
	//	}	
	return StatusCode::SUCCESS;
}

//Reinitialise the ReadoutTool
void FTReadoutTool::clear() {
	m_boards.clear();
	m_nBoard = 0;
	
	return;
}

//    //Main readout functions
// Propagate methods from FTChannelID
//unsigned int FTReadoutTool::station(const LHCb::FTChannelID aChan) const{return aChan.station();}
//unsigned int FTReadoutTool::layer  (const LHCb::FTChannelID aChan) const{return aChan.layer  ();}
//unsigned int FTReadoutTool::quarter(const LHCb::FTChannelID aChan) const{return aChan.quarter();}
//unsigned int FTReadoutTool::uniqueQuarter(const LHCb::FTChannelID aChan) const {return aChan.uniqueQuarter();}
//unsigned int FTReadoutTool::module (const LHCb::FTChannelID aChan) const {return aChan.module ();}//LoHenry: temp. Depends on the module ID
//unsigned int FTReadoutTool::mat    (const LHCb::FTChannelID aChan) const {return aChan.mat ();}//LoHenry: temp. Depends on the module ID
//unsigned int FTReadoutTool::sipm   (const LHCb::FTChannelID aChan) const {return aChan.sipm();}//LoHenry: temp. Depends on the module ID

// Build a FTChannelID from info
LHCb::FTChannelID FTReadoutTool::station      (const unsigned int aChan) const{return LHCb::FTChannelID(aChan,0,0,0,0,0,0);}
LHCb::FTChannelID FTReadoutTool::layer        (const unsigned int aChan) const{return LHCb::FTChannelID(0,aChan,0,0,0,0,0);}
LHCb::FTChannelID FTReadoutTool::quarter      (const unsigned int aChan) const{return LHCb::FTChannelID(0,0,aChan,0,0,0,0);}
LHCb::FTChannelID FTReadoutTool::uniqueQuarter(const unsigned int aChan) const{return LHCb::FTChannelID(0,0,aChan,0,0,0,0);}
LHCb::FTChannelID FTReadoutTool::module       (const unsigned int aChan) const{return LHCb::FTChannelID(0,0,0,aChan,0,0,0);}
LHCb::FTChannelID FTReadoutTool::mat          (const unsigned int aChan) const{return LHCb::FTChannelID(0,0,0,0,aChan,0,0);}
LHCb::FTChannelID FTReadoutTool::sipm         (const unsigned int aChan) const{return LHCb::FTChannelID(0,0,0,0,0,aChan,0);}

unsigned int FTReadoutTool::nBoard() const{return 0;}

//    //Input/Output
/// printout

void FTReadoutTool::printMapping() const
{
	// dump out the readout mapping
	std::cout << "print mapping for: " << name() << " tool\n";
	//	std::cout << "print mapping for: dummy tool\n";//LoHenry: dummy -> name()
	std::cout << " Number of boards " << m_nBoard << '\n';
	for (const auto& b : m_boards ) std::cout << *b << '\n';
}

/// write an xml file
StatusCode FTReadoutTool::writeMappingToXML() const
{
  // load conditions
	//  Condition* rInfo = getDet<Condition>(m_conditionLocation);
  
  std::ofstream outputFile(m_outputFileName.value());
  if (outputFile.fail() ){
    return Warning("Failed to open output file",StatusCode::FAILURE);
  }
  return StatusCode::SUCCESS;
}

