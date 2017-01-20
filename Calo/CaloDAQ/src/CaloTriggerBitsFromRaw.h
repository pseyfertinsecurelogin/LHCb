#ifndef CALOTRIGGERBITSFROMRAW_H 
#define CALOTRIGGERBITSFROMRAW_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "CaloDAQ/ICaloTriggerBitsFromRaw.h"            // Interface
#include "CaloReadoutTool.h"


/** @class CaloTriggerBitsFromRaw CaloTriggerBitsFromRaw.h
 *  Decode the PRS bits. 
 *
 *  @author Olivier Callot
 *  @date   2005-01-06
 */
class CaloTriggerBitsFromRaw : public CaloReadoutTool , virtual public ICaloTriggerBitsFromRaw {
public: 
  /// Standard constructor
  CaloTriggerBitsFromRaw( const std::string& type, 
                          const std::string& name,
                          const IInterface* parent);

  StatusCode initialize() override;

  const LHCb::Calo::FiredCells& prsCells( ) override; // get prs FiredCells
  const LHCb::Calo::FiredCells& spdCells( ) override; // get spd FiredCells
  const LHCb::Calo::PrsSpdFiredCells& prsSpdCells( ) override; // get all FiredCells
  const LHCb::Calo::PrsSpdFiredCells& prsSpdCells(int source ) override; // get FiredCells for a single bank
  const LHCb::Calo::PrsSpdFiredCells& prsSpdCells( const LHCb::RawBank& bank ) override; // get FiredCells for a single bank
  void clear() override;
  void cleanData(int feb) override;
  
private:
  bool getData(const LHCb::RawBank& bank );

  LHCb::Calo::PrsSpdFiredCells m_data;  
};
#endif // CALOTRIGGERBITSFROMRAW_H
