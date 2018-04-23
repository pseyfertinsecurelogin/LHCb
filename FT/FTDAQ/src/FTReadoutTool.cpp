// FTDAQ
#include "FTReadoutTool.h"
#include "FTRawBankParams.h"

// FT
#include "Kernel/FTChannelID.h"
#include "FTXMLUtils.h"

#include <algorithm>

// Det Desc
#include "DetDesc/Condition.h"
#include "DetDesc/ConditionInfo.h"

using namespace LHCb;

DECLARE_COMPONENT( FTReadoutTool )

//    //Main initialisation/destruction methods
// Constructor
FTReadoutTool::FTReadoutTool(const std::string& type,
               const std::string& name,
               const IInterface* parent)
: base_class( type, name, parent )
{
  m_idTell40ByMatT1T2.reserve(30);//20 in reality
  m_idTell40ByMatT1T2.reserve(30);//24 in reality
  declareProperty( "conditionLocation",
                   m_conditionLocation  = "/dd/Conditions/ReadoutConf/FT/ReadoutMap" );
}

StatusCode FTReadoutTool::initialize(){
  StatusCode sc = base_class::initialize();
  if (sc.isFailure()){
    return Error("Failed to initialize", sc);
  }
  
  registerCondition(m_conditionLocation,
                    &FTReadoutTool::readFile);
  sc = runUpdate();
  if (sc.isFailure()) return Error ( "Failed first UMS update for readout tool", sc );
  return StatusCode::SUCCESS;
}

unsigned int FTReadoutTool::nStations() const {return m_nStations;}
unsigned int FTReadoutTool::nLayers()   const {return m_nLayers;}
unsigned int FTReadoutTool::nTell40PerQuarter() const {return m_nTell40PerQuarter;}
unsigned int FTReadoutTool::nTell40() const {return m_nTell40;}
unsigned int FTReadoutTool::nQuarters() const {return m_nQuarters;}


// Build a FTChannelID from info
LHCb::FTChannelID FTReadoutTool::station      (const unsigned int a) const{return LHCb::FTChannelID(a,0,0,0,0,0,0);}
LHCb::FTChannelID FTReadoutTool::layer        (const unsigned int a) const{return LHCb::FTChannelID(0,a,0,0,0,0,0);}
LHCb::FTChannelID FTReadoutTool::quarter      (const unsigned int a) const{return LHCb::FTChannelID(0,0,a,0,0,0,0);}
LHCb::FTChannelID FTReadoutTool::module       (const unsigned int a) const{return LHCb::FTChannelID(0,0,0,a,0,0,0);}
LHCb::FTChannelID FTReadoutTool::mat          (const unsigned int a) const{return LHCb::FTChannelID(0,0,0,0,a,0,0);}
LHCb::FTChannelID FTReadoutTool::sipm         (const unsigned int a) const{return LHCb::FTChannelID(0,0,0,0,0,a,0);}
LHCb::FTChannelID FTReadoutTool::channel      (const unsigned int a) const{return LHCb::FTChannelID(0,0,0,0,0,0,a);}

// Build a bank number from ID and conversely

LHCb::FTChannelID FTReadoutTool::channelIDShift(unsigned int bankNumber) const
{
  return LHCb::FTChannelID(m_FTTell40FirstChannel[bankNumber]);
}

unsigned int FTReadoutTool::bankNumber(LHCb::FTChannelID id) const
{
  unsigned int bankNumber = m_nTell40PerQuarter*(id.quarter() + 4*id.layer() + 16*(id.station()-1u));
  unsigned int shift = (id.station()!=3)?m_idTell40ByMatT1T2[id.module()*4+id.mat()]:m_idTell40ByMatT3[id.module()*4+id.mat()];
  return bankNumber + shift;
}

//    //Input/Output

/// write an xml file
std::string FTReadoutTool::footer() const
{
  std::string temp = m_footer;
  temp.insert(0, "</catalog>" );
  return temp;
}

std::string FTReadoutTool::header(const std::string& conString) const
{
  // get the header
  std::string::size_type startpos = conString.find(m_startTag);
  std::string temp = conString.substr(0,startpos);
  temp.insert(startpos, "<catalog name=\"ReadoutSectors\">" );

  // correct the location of the DTD
  if( m_removeCondb ) {
    FT::XMLUtils::replace(temp,"conddb:", "");
    std::string location;
    for (unsigned int i = 0;  i< m_depth; ++i) location += "../";
    std::string::size_type pos = temp.find("/DTD/");
    temp.insert(pos,location);
    FT::XMLUtils::replace(temp,"//", "/");
  }

  return temp;
}

std::string FTReadoutTool::strip(const std::string& conString) const
{
  std::string::size_type startpos = conString.find(m_startTag);
  std::string::size_type endpos = conString.find(m_footer);
  return conString.substr(startpos, endpos - startpos);
}

StatusCode FTReadoutTool::finalize()
{
  if (m_writeXML) writeMappingToXML();
  return base_class::finalize();
}

StatusCode FTReadoutTool::writeMappingToXML() const
{
  // load conditions  
  Condition* rInfo = getDet<Condition>(m_conditionLocation);

  std::ofstream outputFile(m_outputFileName.value());
  if (outputFile.fail() ){
    return Warning("Failed to open output file",StatusCode::FAILURE);
  }
  
  // write the xml headers
  outputFile << header(rInfo->toXml("", true, m_precision))<< '\n';
  
  // add comments
  std::ostringstream comment;
  FT::XMLUtils::fullComment(comment, m_author, m_tag, m_desc);
  outputFile << comment.str() << '\n';
  
  std::string temp = strip(rInfo->toXml("", false, m_precision));
  outputFile << temp << "\n\n";
  
  // footer
  outputFile << footer() << '\n';
  
  return StatusCode::SUCCESS;
}

void FTReadoutTool::clear()
{

}

StatusCode FTReadoutTool::validate() const
{
  //Check the accordance of the nStations/nLayers/nQuarter with the hardcoded values)
  if (m_nStations != FTRawBank::nStations)
    return Error("Number of stations is not the same in SIMCOND and in FTRawBankParams",StatusCode::FAILURE);
  if (m_nLayers != FTRawBank::nLayers)
    return Error("Number of layers is not the same in SIMCOND and in FTRawBankParams",StatusCode::FAILURE);
  if (m_nQuarters != FTRawBank::nQuarters)
    return Error("Number of quarters is not the same in SIMCOND and in FTRawBankParams",StatusCode::FAILURE);
  if (m_nTell40PerQuarter != FTRawBank::nTell40PerQuarter)
    return Error("Number of TELL40 per quarter is not the same in SIMCOND and in FTRawBankParams",StatusCode::FAILURE);
  return StatusCode::SUCCESS;
}

StatusCode FTReadoutTool::readFile()
{
  clear();

  Condition* rInfo = getDet<Condition>(m_conditionLocation);
  //Global params
  m_nStations         = rInfo->param<int>("nStations");
  m_nLayers           = rInfo->param<int>("nLayers");
  m_nQuarters         = rInfo->param<int>("nQuarters");
  m_nTell40PerQuarter = rInfo->param<int>("nTell40PerQuarter");
  m_nTell40           = rInfo->param<int>("nTell40");

  //Get the readout map  
  m_idTell40ByMatT1T2 = rInfo->param<std::vector<int> >("idTell40ByMatT1T2");
  m_idTell40ByMatT3   = rInfo->param<std::vector<int> >("idTell40ByMatT3");

  m_FTTell40Station      = rInfo->param<std::vector<int> >("FTTell40Station");
  m_FTTell40Layer        = rInfo->param<std::vector<int> >("FTTell40Layer");
  m_FTTell40Quarter      = rInfo->param<std::vector<int> >("FTTell40Quarter");
  m_FTTell40FirstModule  = rInfo->param<std::vector<int> >("FTTell40FirstModule");
  m_FTTell40FirstMat     = rInfo->param<std::vector<int> >("FTTell40FirstMat");
  
  //Construct the first channel attribute  
  m_FTTell40FirstChannel.reserve(m_nTell40);
  for (unsigned int i = 0 ; i < m_nTell40 ; i++)
    {
      m_FTTell40FirstChannel.push_back(LHCb::FTChannelID(m_FTTell40Station[i],
                                                         m_FTTell40Layer  [i],
                                                         m_FTTell40Quarter[i],
                                                         m_FTTell40FirstModule[i],
                                                         m_FTTell40FirstMat[i],
                                                         0,
                                                         0
                                                         ));
      warning() << "LoH: " << m_FTTell40FirstChannel[i] << endmsg;
    }
  StatusCode sc = validate();
  if (sc.isFailure() ){
    return Error("Failed to validate mapping",StatusCode::FAILURE);
  }

  return StatusCode::SUCCESS;
}
