#ifndef L0HC_L0HCALG_H
#define L0HC_L0HCALG_H 1

#include "L0Base/L0AlgBase.h"
#include "DetDesc/Condition.h"

/** @class L0HCAlg L0HCAlg.h
 *
 *  Algorithm responsible of producing the L0 Herschel information.
 *
 *  @author  Dan Johnson
 *  @date    2 May 2017
 */

class L0HCAlg : public L0AlgBase {
 public:
  L0HCAlg(const std::string& name, ISvcLocator* pSvcLocator);
  virtual ~L0HCAlg();

  StatusCode initialize() override;  ///< Algorithm initialization
  StatusCode execute() override;     ///< Algorithm execution

 private:
  /// Pointer to conditions DB
  Condition* m_cond = nullptr;
  /// Herschel digits location
  std::string m_l0digitLocation;
  std::string m_digitLocation;
  
  /// Flag for emulation of Herschel trigger bit construction, using raw Herschel data and a given threshold
  bool m_emulateHCFETrigPGA ;
  /// Flag for creation of fake L0HCDigits, all counters over threshold
  bool m_fakeHCL0Digits ;

  /// Number of B-side crate
  unsigned int m_crateB;
  /// Number of F-side crate
  unsigned int m_crateF;

  /// Channel mappings
  std::vector<int> m_channelsB0;
  std::vector<int> m_channelsB1;
  std::vector<int> m_channelsB2;
  std::vector<int> m_channelsF1;
  std::vector<int> m_channelsF2;

  /// Channel numbers for each quadrant
  std::vector<std::vector<unsigned int> > m_channels;

  /// Cache database content
  StatusCode cacheMapping();
  /// Setup the mapping for a given station.
  bool mapChannels(const std::vector<int>& channels, const unsigned int station,
                   const bool bwd);
};

#endif  // L0HC_L0HCALG_H
