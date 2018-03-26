// FTDAQ
#include "FTReadoutTool.h"

// FT
#include "Kernel/FTChannelID.h"
#include "FTXMLUtils.h"

#include <algorithm>

// Det Desc
//#include "DetDesc/Condition.h"
//#include "DetDesc/ConditionInfo.h"

using namespace LHCb;

DECLARE_COMPONENT( FTReadoutTool )

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

StatusCode FTReadoutTool::initialize(){
  StatusCode sc = base_class::initialize();
  if (sc.isFailure()){
    return Error("Failed to initialize", sc);
  }

  return StatusCode::SUCCESS;
}

// Build a FTChannelID from info
LHCb::FTChannelID FTReadoutTool::station      (const unsigned int a) const{return LHCb::FTChannelID(a,0,0,0,0,0,0);}
LHCb::FTChannelID FTReadoutTool::layer        (const unsigned int a) const{return LHCb::FTChannelID(0,a,0,0,0,0,0);}
LHCb::FTChannelID FTReadoutTool::quarter      (const unsigned int a) const{return LHCb::FTChannelID(0,0,a,0,0,0,0);}
LHCb::FTChannelID FTReadoutTool::uniqueQuarter(const unsigned int a) const{return LHCb::FTChannelID(1,0,a,0,0,0,0);}
LHCb::FTChannelID FTReadoutTool::module       (const unsigned int a) const{return LHCb::FTChannelID(0,0,0,a,0,0,0);}
LHCb::FTChannelID FTReadoutTool::mat          (const unsigned int a) const{return LHCb::FTChannelID(0,0,0,0,a,0,0);}
LHCb::FTChannelID FTReadoutTool::sipm         (const unsigned int a) const{return LHCb::FTChannelID(0,0,0,0,0,a,0);}

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
  //  Condition* rInfo = getDet<Condition>(m_conditionLocation);
  
  std::ofstream outputFile(m_outputFileName.value());
  if (outputFile.fail() ){
    return Warning("Failed to open output file",StatusCode::FAILURE);
  }
  
  // write the xml headers
  //  outputFile << header(rInfo->toXml("", true, m_precision))<< '\n';
  
  // add comments
  std::ostringstream comment;
  FT::XMLUtils::fullComment(comment, m_author, m_tag, m_desc);
  outputFile << comment.str() << '\n';
  
  //  std::string temp = strip(rInfo->toXml("", false, m_precision));
  //  outputFile << temp << "\n\n";
  
  // footer
  outputFile << footer() << '\n';
  
  return StatusCode::SUCCESS;
}


unsigned int FTReadoutTool::nBoard() const{return 0;}
