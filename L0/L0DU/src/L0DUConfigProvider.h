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
#ifndef L0DUCONFIGPROVIDER_H
#define L0DUCONFIGPROVIDER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
// Interfaces
#include "L0Interfaces/IL0DUConfigProvider.h"
#include "L0Interfaces/IL0CondDBProvider.h"
#include "Event/L0DUBase.h"

/** @class L0DUConfigProvider L0DUConfigProvider.h
 *
 *  Provide a **SINGLE** L0DU configuration according to
 *  the Trigger Configuration Key
 *  Configuration described in options file
 *
 *  @author Olivier Deschamps

 *  @date   2007-09-19
 */
class L0DUConfigProvider : public extends<GaudiTool, IL0DUConfigProvider> {
public:
  /// Standard constructor
  L0DUConfigProvider( const std::string& type,
                const std::string& name,
                const IInterface* parent);

  StatusCode initialize() override;
  StatusCode finalize() override;

  LHCb::L0DUConfig*  config(long tck = LHCb::L0DUTemplateConfig::TCKValue ,std::string slot="T0") override {
    if (!m_uptodate) { reset();update(); }
    if( slot.empty() ) slot = "T0";
    // first : handle TEMPLATE configuration
    if( m_template && tck == LHCb::L0DUTemplateConfig::TCKValue ){
        if ( msgLevel(MSG::DEBUG) )
          debug()<<"loading the TEMPLATE Configuration" << endmsg;
        return m_config;
    }

    if( tck != m_tckopts)warning() << "The requested TCK " << tck
                                   <<" is not consistent with the options definition "
                                   << m_tckopts << " CHECK your options ! " << endmsg;
    if(tck < 0 || tck > 0xFFFF){
      warning() << "requested TCK is not a 16 bit word" << endmsg;
      return nullptr;
    }
    auto it = m_configs.find( slot );
    if ( it == m_configs.end() ) createConfig(slot);

    it = m_configs.find( slot );
    if ( it == m_configs.end() ) { // if re-creating failed (paranoia test)
      warning() << " no configs container found for slot " << slot << endmsg;
      return nullptr;
    }

    // it exists
    return it->second->object(tck);
  }

  LHCb::L0DUConfigs* configs(std::string slot="T0") override {
    if ( !m_uptodate ) { reset(); update(); }
    return m_configs[slot.empty() ? "T0" : slot].get();
  }

private:
  void handler(Property&);
  StatusCode update();
  void reset();

  void createConfig(std::string slot="T0");
  std::vector<std::string> Parse(const std::string& flag, const std::vector<std::string>& config );
  typedef std::vector<std::vector<std::string> > ConfigType;

  std::map<std::string,std::string> m_sepMap;
  std::map<std::string,int> m_constData;
  ConfigType m_data;
  ConfigType m_conditions;
  ConfigType m_channels;
  ConfigType m_triggers;

  std::string m_tck;
  std::string m_def;
  std::string m_recipe;
  bool m_detail;


  void printConfig(const LHCb::L0DUConfig& config,std::string slot) const;
  void constantData();
  void predefinedData();
  StatusCode createData();
  StatusCode createConditions();
  StatusCode createChannels();
  StatusCode createTriggers();
  void predefinedTriggers();
  bool getDataList(const std::string&, std::vector<std::string>& );
  bool configChecker();
  bool conditionCheck(LHCb::L0DUElementaryCondition* condition);
  bool conditionOrdering();

  //options-defined data/conditions/channels
  LHCb::L0DUElementaryData::Map m_dataMap;
  LHCb::L0DUElementaryCondition::Map m_conditionsMap;
  LHCb::L0DUChannel::Map m_channelsMap;
  LHCb::L0DUTrigger::Map m_triggersMap;

  struct ConfigsReleaser {
      void operator()(LHCb::L0DUConfigs* p) { if (p) p->release(); }
  };
  using L0DUConfigsPtr = std::unique_ptr<LHCb::L0DUConfigs,ConfigsReleaser>;
  std::map<std::string,L0DUConfigsPtr> m_configs;

  LHCb::L0DUConfig*  m_config = nullptr;

  IL0CondDBProvider* m_condDB = nullptr;

  unsigned long m_cData =0;
  int m_pData       = 0;
  long m_tckopts    = 0;
  bool m_template   = false;
  bool m_uptodate   = false;
  bool m_forceOrder = false;
  std::vector<std::vector<LHCb::L0DUElementaryCondition*> > m_condOrder;
  std::vector<int> m_condMax;
  bool m_reOrder;
  bool m_check;
  unsigned int m_reported = 0;
  std::vector<int> m_FOIx;
  std::vector<int> m_FOIy;
  std::vector<int> m_knownBXs = { 0,-1 }; // The BXs the firmware can use in the algorithm
};
#endif // L0DUCONFIGPROVIDER_H
