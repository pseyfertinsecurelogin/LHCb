#ifndef COMPONENT_DECALOCALIB_H
#define COMPONENT_DECALOCALIB_H 1

#include <map>
#include <ostream>
#include <string>
#include <vector>

#include "CaloDet/DeCalorimeter.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiTupleAlg.h"
#include "GaudiKernel/IRndmGenSvc.h"
#include "GaudiKernel/RndmGenerators.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"

class DeCalorimeter;
class ISvcLocator;



/** @class DeCaloCalib DeCaloCalib.h component/DeCaloCalib.h
 *
 *
 *  @author Olivier DESCHAMPS
 *  @date   2007-08-22
 */
class DeCaloCalib : public GaudiTupleAlg {
public:
  /// Standard constructor
  DeCaloCalib( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution



protected:
  IRndmGenSvc* rndmSvc() const  { return m_rndmSvc.get() ; }
private:
  double delta(long id ){
     std::stringstream sid("");
     sid << id ;
     return (m_deltas.find( sid.str() ) != m_deltas.end() ) ? m_deltas[sid.str()] : m_deltas["Default"];
  }
  void update();
  bool isDead(int channel);

  Rndm::Numbers m_shoot;

  std::string m_detectorName;
  std::string m_method;
  std::vector<double> m_params;
  std::map<std::string , double > m_deltas;
  std::string m_key;
  DeCalorimeter* m_calo = nullptr;
  mutable SmartIF<IRndmGenSvc>   m_rndmSvc;        ///< random number service
  bool m_update;
  bool m_ntup;
  std::vector<int> m_dead;
};
#endif // COMPONENT_DECALOCALIB_H
