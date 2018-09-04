#ifndef UTDECODINGBASEALG_H
#define UTDECODINGBASEALG_H 1

#include "GaudiKernel/DataObjectHandle.h"
#include "Kernel/UTAlgBase.h"
#include "Event/RawBank.h"
#include "Kernel/UTDAQDefinitions.h"
#include "Kernel/STLExtensions.h"

#include "Event/UTCluster.h"
#include "Event/ODIN.h"

#include <vector>
#include <string>

/** @class UTDecodingBaseAlg UTDecodingBaseAlg.h
 *
 *  Algorithm to create UTClusters from RawEvent object
 *
 *  @author M. Needham
 *  @author S. Ponce
 */

#include "Event/RawEvent.h"

#include "Kernel/UTClusterWord.h"
#include "Kernel/UTDecoder.h"
#include "Event/UTTELL1BoardErrorBank.h"

#include <string>


class UTTell1Board;

class UTDecodingBaseAlg : public UT::AlgBase {

public:

  /// Standard constructor
  UTDecodingBaseAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization


protected:


 LHCb::RawBank::BankType bankType() const;

 bool forceVersion() const;

 unsigned int pcnVote(LHCb::span<const LHCb::RawBank* > banks) const;

 bool checkDataIntegrity(UTDecoder& decoder, const UTTell1Board* aBoard,
                         const unsigned int bankSize, const UTDAQ::version& bankVersion) const;

 /** list of boards missing in action */
 std::vector<unsigned int> missingInAction(LHCb::span<const LHCb::RawBank*> banks) const;

 /// Decodes error banks
 std::unique_ptr<LHCb::UTTELL1BoardErrorBanks> decodeErrors(const LHCb::RawEvent& raw) const;

 /** recover mode **/
 bool recoverMode() const;

 /** can be recovered recover **/
 bool canBeRecovered(const LHCb::UTTELL1BoardErrorBank* bank,
                     const UTClusterWord& word,
                     const unsigned int pcn) const;

 /// compute the spill offset
 void computeSpillOffset(const std::string& location);

 /** check the spill is read out **/
 bool validSpill(const LHCb::ODIN& odin) const;

 /** return spill offset */
 LHCb::UTCluster::Spill spill() const;

 unsigned int byteSize(LHCb::span<const LHCb::RawBank*> banks) const;

 bool m_skipErrors;
 std::string m_bankTypeString;

 int m_forcedVersion;
 bool m_checkValidSpill;
 LHCb::RawBank::BankType m_errorType;
 LHCb::RawBank::BankType m_bankType;

private:

  std::string toSpill(const std::string& location) const;
  LHCb::UTCluster::Spill m_spillOffset;

  std::string m_errorBankString;

  bool m_recoverMode;

};



inline LHCb::RawBank::BankType UTDecodingBaseAlg::bankType() const {
  return m_bankType;
}

inline bool UTDecodingBaseAlg::forceVersion() const{
  return m_forcedVersion >= 0;
}


inline bool UTDecodingBaseAlg::recoverMode() const{
  return m_recoverMode;
}

#include "Kernel/UTClusterWord.h"
#include "Kernel/UTPPRepresentation.h"
#include "Kernel/UTStripRepresentation.h"
#include "Kernel/UTDAQDefinitions.h"
#include "Kernel/LHCbConstants.h"
#include "Event/UTTELL1Error.h"

inline bool UTDecodingBaseAlg::canBeRecovered(const LHCb::UTTELL1BoardErrorBank* bank,
                                              const UTClusterWord& word,
                                              const unsigned int pcn) const{

  UTDAQ::UTPPRepresentation ppRep = UTDAQ::UTPPRepresentation(UTDAQ::UTStripRepresentation(word.channelID()));
  unsigned int pp, beetle, port, strip;
  ppRep.decompose(pp, beetle, port, strip); // split up the word
  const LHCb::UTTELL1Error* errorInfo = bank->ppErrorInfo(pp);
  bool ok = false;
  if (errorInfo != 0 ){
    if (errorInfo->linkInfo(beetle, port,pcn) == LHCb::UTTELL1Error::FailureMode::kNone){
      ok = true;
    }
  }
  return ok ;
}

inline LHCb::UTCluster::Spill UTDecodingBaseAlg::spill() const{
  return m_spillOffset;
}


#endif // UTDECODINGBASEALG_H
