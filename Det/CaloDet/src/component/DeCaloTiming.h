#ifndef COMPONENT_DECALOTIMING_H 
#define COMPONENT_DECALOTIMING_H 1

#include <array>
#include <string>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTupleAlg.h"
#include "GaudiKernel/StatusCode.h"

class DeCalorimeter;
class ISvcLocator;

/** @class DeCaloTiming DeCaloTiming.h component/DeCaloTiming.h
 *  
 *
 *  @author Olivier DESCHAMPS
 *  @date   2007-08-22
 */
class DeCaloTiming : public GaudiTupleAlg {
public: 
  /// Standard constructor
  DeCaloTiming( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization

private:
  double delta(long id ){
     auto i = m_deltas.find(std::to_string(id) ) ;
     if ( i == m_deltas.end() ) i = m_deltas.find( "Default" );
     return i->second;
  }

  std::string m_detectorName;
  std::string m_method;
  std::array<double,2> m_params;
  std::map<std::string , double > m_deltas;
  std::string m_key;
  DeCalorimeter* m_calo = nullptr;
};
#endif // COMPONENT_DECALOTIMING_H
