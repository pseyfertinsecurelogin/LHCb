#ifndef CALOFUTUREDATAPROVIDER_H
#define CALOFUTUREDATAPROVIDER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "CaloFutureDAQ/ICaloFutureDataProvider.h"            // Interface
#include "CaloFutureReadoutTool.h"

/** @class CaloFutureDataProvider CaloFutureDataProvider.h
 *  Fast access to calorimeter data from raw
 *
 *  @author Olivier Deschamps
 *  @date   2007-02-27
 */
class CaloFutureDataProvider : public extends<CaloFutureReadoutTool, ICaloFutureDataProvider> {

public:
  /// Standard constructor
  using extends::extends;


  StatusCode initialize() override;
  void clear() override;
  void cleanData(int feb) override;
  int    adc(LHCb::CaloCellID id, int def = 0) override;
  double digit(LHCb::CaloCellID id, double def = 0.) override;
  unsigned int nTell1s() override {return m_tell1s;}
  const CaloVector<LHCb::CaloAdc>& adcs(int source=-1,bool clean=true) override;
  const CaloVector<LHCb::CaloAdc>& adcs(std::vector<int> source,bool clean=true) override;
  const CaloVector<LHCb::CaloDigit>& digits(int source=-1,bool clean=true) override;
  const CaloVector<LHCb::CaloDigit>& digits(std::vector<int> sources,bool clean=true) override;
  ICaloFutureDataProvider::CaloAdcPair adcRange() override {
    LHCb::CaloAdc min = (m_minADC.cellID() == LHCb::CaloCellID()) ? LHCb::CaloAdc(LHCb::CaloCellID(), 0) : m_minADC;
    LHCb::CaloAdc max = (m_maxADC.cellID() == LHCb::CaloCellID()) ? LHCb::CaloAdc(LHCb::CaloCellID(), 0) : m_maxADC;
    return std::make_pair(min,max) ;
  }
  ICaloFutureDataProvider::CaloAdcPair pinRange() override{
    LHCb::CaloAdc min = (m_minPinADC.cellID() == LHCb::CaloCellID()) ? LHCb::CaloAdc(LHCb::CaloCellID(), 0) : m_minPinADC;
    LHCb::CaloAdc max = (m_maxPinADC.cellID() == LHCb::CaloCellID()) ? LHCb::CaloAdc(LHCb::CaloCellID(), 0) : m_maxPinADC;
    return std::make_pair(min,max) ;
  }

protected:
  void adc2digit();
  bool decodeCell(LHCb::CaloCellID id);
  bool decodeTell1(int tell1);
  bool decodeBank(const LHCb::RawBank& bank);
  bool decodePrsTriggerBank(const LHCb::RawBank& bank);
private:
  LHCb::CaloAdc fillAdc(LHCb::CaloCellID id, int adc,int sourceID){
    LHCb::CaloAdc temp(id,adc);
    if( 0 >  m_adcs.index(id)){
      m_adcs.addEntry( temp, id);
    }else{
      counter("Duplicate ADC found")+=1;
      m_status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::DuplicateEntry);
      return temp;
    }

    if( id.area() != CaloCellCode::PinArea ){
      if( adc < m_minADC.adc()  )m_minADC = temp;
      if( adc > m_maxADC.adc()  )m_maxADC = temp;
    }else{
      if( adc < m_minPinADC.adc() )m_minPinADC = temp;
      if( adc > m_maxPinADC.adc() )m_maxPinADC = temp;
    }
    return temp;
  }
  CaloVector<LHCb::CaloAdc>   m_adcs;
  CaloVector<LHCb::CaloDigit> m_digits;
  unsigned int m_tell1s = 0;
  LHCb::CaloAdc m_minADC;
  LHCb::CaloAdc m_minPinADC;
  LHCb::CaloAdc m_maxADC;
  LHCb::CaloAdc m_maxPinADC;
};
#endif // CALOFUTUREDATAPROVIDER_H