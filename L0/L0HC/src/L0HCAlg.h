#ifndef   L0HC_L0HCALG_H
#define   L0HC_L0HCALG_H  1

// from Gaudi
#include "L0Base/L0AlgBase.h"

// Local classes
#include "TriggerCard.h"
#include "L0Candidate.h"

/** @class L0HCAlg L0HCAlg.h
 *
 *  Algorithm responsible of producing the L0 Herschel information.
 *
 *  @author  Dan Johnson
 *  @date    2 May 2017
 */

class L0HCAlg : public L0AlgBase {
public:
  /// standard algorithm Constructor
  L0HCAlg( const std::string& name , ISvcLocator* pSvcLocator ) ;

  virtual ~L0HCAlg() ; ///< Standard destructor

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute   () override; ///< Algorithm execution
  StatusCode finalize  () override; ///< Algorithm finalization

protected:
  void addHCData(  );   ///< process the Herschel information

private:
  // Local variables
  Condition *    m_cond = nullptr         	; ///< Pointer to conditions DB
  
  unsigned int m_crateB					  	; /// Number of B-side crate
  unsigned int m_crateF					  	; /// Number of F-side crate
  
  std::vector<int> m_channelsB0			  	; /// Channel mappings
  std::vector<int> m_channelsB1			  	;
  std::vector<int> m_channelsB2			  	;
  std::vector<int> m_channelsF1			  	;
  std::vector<int> m_channelsF2			  	;

  LHCb::HCDigits* l0digits  			  	; /// Herschel trigger digits
  
  std::vector< int > m_HCMult_B, m_HCMult_F ; ///< Multiplicity of Herschel B- or F-side

  /// Cache database content  
  StatusCode cacheMapping();
  /// Setup the mapping for a given station.
  bool mapChannels(const std::vector<int>& channels,
                   const std::vector<int>& refs,
                   const std::vector<int>& masks,
                   const unsigned int station, const bool bwd);

};

#endif //   L0HC_L0HCALG_H
