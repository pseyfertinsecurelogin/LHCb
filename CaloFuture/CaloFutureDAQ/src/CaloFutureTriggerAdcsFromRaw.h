#ifndef CALOFUTURETRIGGERADCSFROMRAW_H 
#define CALOFUTURETRIGGERADCSFROMRAW_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "CaloFutureDAQ/ICaloFutureTriggerAdcsFromRaw.h"            // Interface
#include "CaloFutureReadoutTool.h"

/** @class CaloFutureTriggerAdcsFromRaw CaloFutureTriggerAdcsFromRaw.h
 *  
 *
 *  @author Olivier Callot
 *  @date   2005-01-05
 */
class CaloFutureTriggerAdcsFromRaw final 
: public extends<CaloFutureReadoutTool, ICaloFutureTriggerAdcsFromRaw> {
public: 
  /// Standard constructor
  CaloFutureTriggerAdcsFromRaw( const std::string& type, 
                      const std::string& name,
                      const IInterface* parent);

  StatusCode initialize() override;


  const std::vector<LHCb::L0CaloAdc>& adcs( ) override;
  const std::vector<LHCb::L0CaloAdc>& adcs( int source ) override;
  const std::vector<LHCb::L0CaloAdc>& adcs( const LHCb::RawBank& bank) override;
  const std::vector<LHCb::L0CaloAdc>& pinAdcs( ) override;
  void clear() override;
  void cleanData(int feb) override;

private:
  bool getData ( const LHCb::RawBank& bank );
  std::vector<LHCb::L0CaloAdc> m_data;
  std::vector<LHCb::L0CaloAdc> m_pinData;
};
#endif // CALOFUTURETRIGGERADCSFROMRAW_H
