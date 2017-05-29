#ifndef CALOTRIGGERADCSFROMRAW_H 
#define CALOTRIGGERADCSFROMRAW_H 1

#include <string>
#include <vector>

#include "CaloDAQ/ICaloTriggerAdcsFromRaw.h"            // Interface
#include "CaloReadoutTool.h"
#include "Event/L0CaloAdc.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/extends.h"

class IInterface;
namespace LHCb {
class RawBank;
}  // namespace LHCb
struct ICaloTriggerAdcsFromRaw;

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
#endif // CALOTRIGGERADCSFROMRAW_H
