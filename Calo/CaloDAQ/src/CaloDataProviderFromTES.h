#ifndef CALODATAPROVIDERFROMTES_H
#define CALODATAPROVIDERFROMTES_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "CaloDAQ/ICaloDataProvider.h"            // Interface
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiKernel/Incident.h"
#include "CaloDet/DeCalorimeter.h"

#include "details.h"

/** @class CaloDataProviderFromTES CaloDataProviderFromTES.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2008-08-22
 */
class CaloDataProviderFromTES : public extends< GaudiTool, ICaloDataProvider, IIncidentListener>
{

public:
  /// Standard constructor
  CaloDataProviderFromTES( const std::string& type,
                   const std::string& name,
                   const IInterface* parent);


  StatusCode initialize() override;
  StatusCode finalize() override;
  // =========================================================================
  /// Inform that a new incident has occurred
  void handle(const Incident& /* inc */ ) override { m_getRaw = true; }
  // =========================================================================

  int    adc(LHCb::CaloCellID id,int def=0) override;
  double digit(LHCb::CaloCellID id,double def=0.) override;
  const CaloVector<LHCb::CaloAdc>& adcs(int source=-1,bool clean=true) override;
  const CaloVector<LHCb::CaloDigit>& digits(int source=-1,bool clean=true) override;
  const CaloVector<LHCb::CaloAdc>& adcs(std::vector<int> sources,bool clean=true) override;
  const CaloVector<LHCb::CaloDigit>& digits(std::vector<int> sources,bool clean=true) override;
  void clear() override;
  void cleanData(int feb) override;
  bool getBanks() override;
  ICaloDataProvider::CaloAdcPair adcRange() override{
    LHCb::CaloAdc min = (m_minADC.cellID() == LHCb::CaloCellID()) ? LHCb::CaloAdc(LHCb::CaloCellID(), 0) : m_minADC;
    LHCb::CaloAdc max = (m_maxADC.cellID() == LHCb::CaloCellID()) ? LHCb::CaloAdc(LHCb::CaloCellID(), 0) : m_maxADC;
    return {min,max} ;
  }
  ICaloDataProvider::CaloAdcPair pinRange() override{
    LHCb::CaloAdc min = (m_minPinADC.cellID() == LHCb::CaloCellID()) ? LHCb::CaloAdc(LHCb::CaloCellID(), 0) : m_minPinADC;
    LHCb::CaloAdc max = (m_maxPinADC.cellID() == LHCb::CaloCellID()) ? LHCb::CaloAdc(LHCb::CaloCellID(), 0) : m_maxPinADC;
    return {min,max} ;
  }
  bool ok() override{
    if(m_getRaw)getBanks();
    return m_ok;
  }


  //
  StatusCode  _setProperty(const std::string& p,const std::string& v) override {return  setProperty(p,v);}
  //
  void setBanks(LHCb::span<const LHCb::RawBank*> /* bank */ ) override {} // harmless method
  LHCb::RawBankReadoutStatus& status() override {m_status=LHCb::RawBankReadoutStatus();return m_status; } // harmless method
  unsigned int nTell1s() override {return 0;} // harmless method
  void putStatusOnTES() override {return;} // harmless method
  DeCalorimeter* deCalo() override {return m_calo;}

private:
  bool checkSrc(int source);

  bool fromAdc() { return m_data == details::source_t::from_adc; }
  bool fromDigit() { return m_data == details::source_t::from_digit; }

  CaloVector<LHCb::CaloAdc>    m_adcs;
  CaloVector<LHCb::CaloDigit> m_digits;
  DeCalorimeter*   m_calo = nullptr;
  //
  const details::DetectorName_t  m_detectorName;
  std::string m_raw;
  Gaudi::Property<details::source_t> m_data { this, "InputDataType", details::source_t::from_digit };
  std::string m_adcLoc;
  std::string m_digLoc;
  Gaudi::Property<std::string> m_loc { this, "InputDataLocation" };
  std::vector<int> m_readSources;
  LHCb::CaloDigits* m_digCont = nullptr;
  LHCb::CaloAdcs* m_adcCont = nullptr;
  bool m_ok = false;
  LHCb::CaloAdc m_minADC;
  LHCb::CaloAdc m_minPinADC;
  LHCb::CaloAdc m_maxADC;
  LHCb::CaloAdc m_maxPinADC;
  bool m_getRaw = false;
  LHCb::RawBankReadoutStatus m_status;
};
#endif // CALODATAPROVIDERFROMTES_H
