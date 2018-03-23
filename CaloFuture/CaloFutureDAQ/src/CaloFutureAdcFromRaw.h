#ifndef CALOFUTUREADCFROMRAW_H
#define CALOFUTUREADCFROMRAW_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "CaloDet/DeCalorimeter.h"
#include "Event/CaloAdc.h"
#include "Event/L0CaloAdc.h"
#include "Event/L0PrsSpdHit.h"
#include "CaloFutureDAQ/ICaloFutureDataProvider.h"
#include "CaloFutureDAQ/ICaloFutureL0DataProvider.h"

#include "futuredetails.h"

/** @class CaloFutureAdcFromRaw CaloFutureAdcFromRaw.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2009-04-07
 */

class CaloFutureAdcFromRaw final : public GaudiAlgorithm {
public:
  /// Standard constructor
  CaloFutureAdcFromRaw( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  futuredetails::DetectorName_t m_detectorName;
  std::string m_caloName;
  Gaudi::Property<std::string> m_location { this, "AdcLocation" };
  Gaudi::Property<std::string> m_l0Location { this, "L0AdcLocation" };
  Gaudi::Property<std::string> m_l0BitLocation { this, "L0BitLocation" };
  Gaudi::Property<bool> m_calib { this, "DeCalibration"      , false }; // Expert usage
  int m_offset;
  DeCalorimeter* m_calo = nullptr;
  ICaloFutureDataProvider*    m_data = nullptr;
  ICaloFutureL0DataProvider*  m_l0data = nullptr;
};

#endif // CALOFUTUREADCFROMRAW_H