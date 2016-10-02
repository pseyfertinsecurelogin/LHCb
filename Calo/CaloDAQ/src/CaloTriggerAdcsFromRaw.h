#ifndef CALOTRIGGERADCSFROMRAW_H 
#define CALOTRIGGERADCSFROMRAW_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "CaloDAQ/ICaloTriggerAdcsFromRaw.h"            // Interface
#include "CaloReadoutTool.h"

/** @class CaloTriggerAdcsFromRaw CaloTriggerAdcsFromRaw.h
 *  
 *
 *  @author Olivier Callot
 *  @date   2005-01-05
 */
class CaloTriggerAdcsFromRaw final 
: public extends<CaloReadoutTool, ICaloTriggerAdcsFromRaw> {
public: 
  /// Standard constructor
  CaloTriggerAdcsFromRaw( const std::string& type, 
                      const std::string& name,
                      const IInterface* parent);

  StatusCode initialize() override;


  const std::vector<LHCb::L0CaloAdc>& adcs( );
  const std::vector<LHCb::L0CaloAdc>& adcs( int source );
  const std::vector<LHCb::L0CaloAdc>& adcs( LHCb::RawBank* bank);
  const std::vector<LHCb::L0CaloAdc>& pinAdcs( );
  void clear();
  void cleanData(int feb);

protected:
  bool getData ( const LHCb::RawBank& bank );
private:
  std::vector<LHCb::L0CaloAdc> m_data;
  std::vector<LHCb::L0CaloAdc> m_pinData;
};
#endif // CALOTRIGGERADCSFROMRAW_H
