#ifndef CALOADCFROMRAW_H
#define CALOADCFROMRAW_H 1

#include <string>

#include "CaloDAQ/ICaloDataProvider.h"
#include "CaloDAQ/ICaloL0DataProvider.h"
#include "CaloDet/DeCalorimeter.h"
#include "Event/CaloAdc.h"
#include "Event/L0CaloAdc.h"
#include "Event/L0PrsSpdHit.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class DeCalorimeter;
class ISvcLocator;
struct ICaloDataProvider;
struct ICaloL0DataProvider;

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
  std::string m_location;
  std::string m_l0Location;
  std::string m_l0BitLocation;
  int m_offset;
  bool m_calib = false;
  DeCalorimeter* m_calo = nullptr;
  ICaloDataProvider*    m_data = nullptr;
  ICaloL0DataProvider*  m_l0data = nullptr;
};
#endif // CALOADCFROMRAW_H
