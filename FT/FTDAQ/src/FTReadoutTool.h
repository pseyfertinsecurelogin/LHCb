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

#include "DetDesc/Condition.h"
#include "DetDesc/ConditionInfo.h"


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

  /// init & finish
  StatusCode initialize() override;
  StatusCode finalize() override;

  /// validate
  //  StatusCode validate() const;

  //Build FTChannelID from information
  LHCb::FTChannelID station      (const unsigned int a) const override;
  LHCb::FTChannelID layer        (const unsigned int a) const override;
  LHCb::FTChannelID quarter      (const unsigned int a) const override;
  LHCb::FTChannelID uniqueQuarter(const unsigned int a) const override;
  LHCb::FTChannelID module       (const unsigned int a) const override;
  LHCb::FTChannelID mat          (const unsigned int a) const override;
  LHCb::FTChannelID sipm         (const unsigned int a) const override;

  
  unsigned int nBoard() const override;

  /// printout
  void printMapping() const override;
  
  /// write an xml file
  StatusCode writeMappingToXML() const override;
  std::string footer() const;
  std::string header(const std::string& conString) const;
  std::string strip(const std::string& conString) const;

  std::string m_conditionLocation;
  
 private:
  void clear();
  StatusCode createBoards();

  //  unsigned int m_nRegionA     = 512;
  unsigned int m_firstStation = 512;

  unsigned int m_passedHeaders;//LoH

  bool m_printMapping = false;
  
  unsigned int m_hybridsPerBoard;
  unsigned int m_nBoard;
  std::vector<std::unique_ptr<FTTell40ID> > m_boards;

  Gaudi::Property<std::string> m_outputFileName { this, "outputFile","ReadoutMap.xml"};
  Gaudi::Property<bool> m_writeXML { this, "writeMappingToXML", true };
  Gaudi::Property<std::string> m_footer { this, "footer", "</DDDB>"};
  Gaudi::Property<std::string> m_startTag { this, "startTag", "<condition"};
  std::ofstream m_outputFile;

  Gaudi::Property<std::string> m_author { this, "author", "Louis Henry"};
  Gaudi::Property<std::string> m_tag { this, "tag", "None"};
  Gaudi::Property<std::string> m_desc { this, "description", "BlahBlahBlah"};
  Gaudi::Property<bool> m_removeCondb { this, "removeCondb", false};
  Gaudi::Property<unsigned int> m_precision { this, "precision", 16u};
  Gaudi::Property<unsigned int> m_depth { this, "depths", 3u };


};

#endif // _FTReadoutTool_H
