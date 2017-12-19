#ifndef L0DUMULTICONFIGPROVIDER_H
#define L0DUMULTICONFIGPROVIDER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
// from LHCb
#include "L0Interfaces/IL0DUConfigProvider.h"            // Interface
#include "L0DUConfigProvider.h" // single Config provider tool


/** @class L0DUMultiConfigProvider L0DUMultiConfigProvider.h
 *
 *  Provide  **ANY** predefined L0DU configuration according to
 *  the Trigger Configuration Key
 *  Configuration described in options file
 *
 *  @author Olivier Deschamps
 *  @date   2008-01-26
 */
class L0DUMultiConfigProvider : public extends<GaudiTool, IL0DUConfigProvider> {
public:
  /// Standard constructor
  L0DUMultiConfigProvider( const std::string& type,
                           const std::string& name,
                           const IInterface* parent);

  StatusCode initialize() override;
  StatusCode finalize() override;

  LHCb::L0DUConfig*  config(long tck = LHCb::L0DUTemplateConfig::TCKValue,std::string slot="T0") override;
  LHCb::L0DUConfigs* configs(std::string slot="T0") override {return m_configs[slot].get();}

private:
  // private method
  LHCb::L0DUConfig* loadConfig( const std::string& tck , std::string slot="T0");

  // attributes
  bool m_preload;
  std::vector<std::string> m_list;

  //
  struct ConfigsReleaser {
      void operator()(LHCb::L0DUConfigs* p) { if (p) p->release(); }
  };
  using L0DUConfigsPtr = std::unique_ptr<LHCb::L0DUConfigs,ConfigsReleaser>;
  std::map<std::string,L0DUConfigsPtr> m_configs;
  IL0DUConfigProvider* m_provider = nullptr;
  std::map<std::string,LHCb::L0DUConfig*> m_template;

};
#endif // L0DUMULTICONFIGPROVIDER_H
