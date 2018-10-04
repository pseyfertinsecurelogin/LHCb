#ifndef CALOFUTUREFILLPRSSPDRAWBUFFER_H 
#define CALOFUTUREFILLPRSSPDRAWBUFFER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "CaloDet/DeCalorimeter.h"

/** @class CaloFutureFillPrsSpdRawBuffer CaloFutureFillPrsSpdRawBuffer.h
 *  
 *
 *  @author Olivier Callot
 *  @date   2005-01-04
 */
class CaloFutureFillPrsSpdRawBuffer : public GaudiAlgorithm {
public: 
  /// Standard constructor
  CaloFutureFillPrsSpdRawBuffer( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

private:

  void fillDataBank ( );

  void fillDataBankShort ( );

  void fillTriggerBank ( );

  void fillTriggerBankShort ( );

  void fillPackedBank ( );

  std::string m_inputBank;
  std::string m_prsBank;
  std::string m_spdBank;
  LHCb::RawBank::BankType m_bankType;
  LHCb::RawBank::BankType m_triggerBankType;
  int    m_numberOfBanks;
  int    m_dataCodingType;

  DeCalorimeter* m_calo;

  // Statistics
  
  double m_totDataSize;
  double m_totTrigSize;
  std::vector<double> m_dataSize;
  int m_nbEvents;
  std::vector< std::vector<unsigned int> > m_banks;
  std::vector< std::vector<unsigned int> > m_trigBanks;
};
#endif // CALOFUTUREFILLPRSSPDRAWBUFFER_H
