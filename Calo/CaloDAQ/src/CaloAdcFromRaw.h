#ifndef CALOADCFROMRAW_H
#define CALOADCFROMRAW_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "CaloDet/DeCalorimeter.h"
#include "Event/CaloAdc.h"
#include "Event/L0CaloAdc.h"
#include "Event/L0PrsSpdHit.h"
#include "CaloDAQ/ICaloDataProvider.h"
#include "CaloDAQ/ICaloL0DataProvider.h"

/** @class CaloAdcFromRaw CaloAdcFromRaw.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2009-04-07
 */
class CaloAdcFromRaw final : public GaudiAlgorithm {
public:
  /// Standard constructor
  CaloAdcFromRaw( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  std::string m_detectorName;
  std::string m_caloName;
  Gaudi::Property<std::string> m_location { this, "AdcLocation" };
  Gaudi::Property<std::string> m_l0Location { this, "L0AdcLocation" };
  Gaudi::Property<std::string> m_l0BitLocation { this, "L0BitLocation" };
  Gaudi::Property<bool> m_calib { this, "DeCalibration"      , false }; // Expert usage
  int m_offset;
  DeCalorimeter* m_calo = nullptr;
  ICaloDataProvider*    m_data = nullptr;
  ICaloL0DataProvider*  m_l0data = nullptr;
};
#endif // CALOADCFROMRAW_H
