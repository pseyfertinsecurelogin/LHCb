#ifndef   LLTCALO_L0CALOALG_H
#define   LLTCALO_L0CALOALG_H  1

#include <string>
#include <vector>

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"
#include "L0Candidate.h"
// Local classes
#include "LLTTriggerCard.h"

// forward declarations
class DeCalorimeter ;
class ISvcLocator;
struct ICaloTriggerAdcsFromRaw ;

/** @class LLTCaloAlg LLTCaloAlg.h
 *
 *  Algorithm responsible of producing the LLT Calorimeter information.
 *
 *  @author  Patrick Robbe
 *  @date    27 September 2013
 */

class LLTCaloAlg : public GaudiAlgorithm {
public:
  /// standard algorithm Constructor
  LLTCaloAlg( const std::string& name , ISvcLocator* pSvcLocator ) ;

  virtual ~LLTCaloAlg() ; ///< Standard destructor

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute   () override; ///< Algorithm execution
  StatusCode finalize  () override; ///< Algorithm finalization

protected:
  unsigned int sumEcalData( );  ///< process Ecal FE card.
  unsigned int sumHcalData( );  ///< process Hcal FE card.

private:
  // raw bank container
  std::vector< std::vector< unsigned int > > m_rawOutput ;

  unsigned int m_ECALThreshold ;  ///< Threshold to count multiplicity in ECAL
  unsigned int m_HCALThreshold ;  ///< Threshold to count multiplicity in HCAL

  // Local variables
  DeCalorimeter *    m_ecal = nullptr            ; ///< Pointer to Ecal detector element
  DeCalorimeter *    m_hcal = nullptr            ; ///< Pointer to Hcal detector element

  ICaloTriggerAdcsFromRaw * m_adcsEcal = nullptr ; ///< Tool to decode trigger adcs.
  ICaloTriggerAdcsFromRaw * m_adcsHcal = nullptr ; ///< Tool to decode trigger adcs.

  // Trigger cards
  std::vector< LLTTriggerCard > m_ecalFe ; ///< Ecal front-end card vector
  std::vector< LLTTriggerCard > m_hcalFe ; ///< Hcal front-end card vector

  // Name of the ADC tool
  std::string m_caloToolName ;

  double m_etScale ;
  bool   m_doubleScale ;
};
#endif //   L0CALO_LLTCALOALG_H
