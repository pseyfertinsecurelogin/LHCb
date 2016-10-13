#ifndef STDECODINGBASEALG_H 
#define STDECODINGBASEALG_H 1

#include "GaudiKernel/DataObjectHandle.h"
#include "Kernel/STAlgBase.h"
#include "Event/RawBank.h"
#include "Kernel/STDAQDefinitions.h"

#include "Event/STCluster.h"
#include "Event/ODIN.h"

#include <vector>
#include <string>

/** @class STDecodingBaseAlg STDecodingBaseAlg.h
 *  
 *  Algorithm to create STClusters from RawEvent object
 * 
 *  @author M. Needham
 *  @author S. Ponce
 */

#include "Event/RawEvent.h"

#include "Kernel/STClusterWord.h"
#include "Kernel/STDecoder.h"
#include "Event/STTELL1BoardErrorBank.h"

#include <string>


class STTell1Board;

class STDecodingBaseAlg : public ST::AlgBase {

public:

  /// Standard constructor
  STDecodingBaseAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
   

protected:


 LHCb::RawBank::BankType bankType() const;

 bool forceVersion() const;  

 unsigned int pcnVote(const std::vector<LHCb::RawBank* >& banks) const;

 bool checkDataIntegrity(STDecoder& decoder, const STTell1Board* aBoard, 
                         const unsigned int bankSize, const STDAQ::version& bankVersion) const;

 /** list of boards missing in action */
 std::vector<unsigned int> missingInAction(const std::vector<LHCb::RawBank*>& banks) const; 

 /// Decodes error banks
 std::unique_ptr<LHCb::STTELL1BoardErrorBanks> decodeErrors(const LHCb::RawEvent& raw) const;

 /** recover mode **/
 bool recoverMode() const;

 /** can be recovered recover **/
 bool canBeRecovered(const LHCb::STTELL1BoardErrorBank* bank, 
                     const STClusterWord& word, 
                     const unsigned int pcn) const;

 /// compute the spill offset
 void computeSpillOffset(const std::string& location);

 /** check the spill is read out **/
 bool validSpill(const LHCb::ODIN& odin) const;

 /** return spill offset */
 LHCb::STCluster::Spill spill() const;

 unsigned int byteSize(const std::vector<LHCb::RawBank*>& banks) const;

 bool m_skipErrors;
 std::string m_bankTypeString;

 int m_forcedVersion;
 bool m_checkValidSpill;
 LHCb::RawBank::BankType m_errorType; 
 LHCb::RawBank::BankType m_bankType; 
 
private:

  std::string toSpill(const std::string& location) const;
  LHCb::STCluster::Spill m_spillOffset;
  
  std::string m_errorBankString;
 
  bool m_recoverMode;

};



inline LHCb::RawBank::BankType STDecodingBaseAlg::bankType() const {
  return m_bankType;
}

inline bool STDecodingBaseAlg::forceVersion() const{
  return m_forcedVersion >= 0;
}


inline bool STDecodingBaseAlg::recoverMode() const{
  return m_recoverMode;
}

#include "Kernel/STClusterWord.h"
#include "Kernel/PPRepresentation.h"
#include "Kernel/StripRepresentation.h"
#include "Kernel/STDAQDefinitions.h"
#include "Kernel/LHCbConstants.h"
#include "Event/STTELL1Error.h"

inline bool STDecodingBaseAlg::canBeRecovered(const LHCb::STTELL1BoardErrorBank* bank, 
                                              const STClusterWord& word,
                                              const unsigned int pcn) const{

  STDAQ::PPRepresentation ppRep = STDAQ::PPRepresentation(STDAQ::StripRepresentation(word.channelID()));
  unsigned int pp, beetle, port, strip;
  ppRep.decompose(pp, beetle, port, strip); // split up the word 
  const LHCb::STTELL1Error* errorInfo = bank->ppErrorInfo(pp);
  bool ok = false;
  if (errorInfo != 0 ){
    if (errorInfo->linkInfo(beetle, port,pcn) == LHCb::STTELL1Error::kNone){
      ok = true;
    }
  }
  return ok ; 
}

inline LHCb::STCluster::Spill STDecodingBaseAlg::spill() const{
  return m_spillOffset;
}


#endif // STDECODINGBASEALG_H 
