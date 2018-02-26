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

