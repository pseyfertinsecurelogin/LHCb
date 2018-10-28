/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef RAWBANKTOSTLITECLUSTERALG_H
#define RAWBANKTOSTLITECLUSTERALG_H 1

#include "UTDecodingBaseAlg.h"
#include "Event/RawBank.h"
#include "Kernel/UTDAQDefinitions.h"
#include "GaudiAlg/Transformer.h"

#include "Event/UTLiteCluster.h"

#include <vector>
#include <string>
#include <utility>

/** @class RawBankToUTLiteClusterAlg RawBankToUTLiteClusterAlg.h
 *
 *  Algorithm to create UTClusters from RawEvent object
 *
 *  @author A. Beiter based on code by:
 *  @author M. Needham
 *  @author S. Ponce
 */


#include "Kernel/UTClusterWord.h"

class SiADCWord;
class UTTell1Board;

namespace LHCb{
 class UTChannelID;
 class UTLiteCluster;
}

typedef Gaudi::Functional::Transformer<LHCb::UTLiteCluster::UTLiteClusters(const LHCb::ODIN&, const LHCb::RawEvent&),
  Gaudi::Functional::Traits::BaseClass_t<UTDecodingBaseAlg>> RawBankToUTLiteClusterAlgBaseClass;

class RawBankToUTLiteClusterAlg final : public RawBankToUTLiteClusterAlgBaseClass {

public:

  /// Standard constructor
  RawBankToUTLiteClusterAlg(const std::string& name, ISvcLocator* pSvcLocator);

  StatusCode initialize() override;   ///< Algorithm initialization
  StatusCode finalize() override;     ///< Algorithm finalization
  LHCb::UTLiteCluster::UTLiteClusters operator()(const LHCb::ODIN&, const LHCb::RawEvent&) const override;

private:

  // create Clusters from this type
  StatusCode decodeBanks(const LHCb::RawEvent& rawEvt, LHCb::UTLiteCluster::UTLiteClusters& fCont) const;

  // add a single cluster to the output container
  void createCluster(const UTTell1Board* aBoard,  const UTDAQ::version& bankVersion,
                     const UTClusterWord& aWord, LHCb::UTLiteCluster::UTLiteClusters& fCont, const bool isUT) const;
};

#include "Kernel/UTTell1Board.h"
#include "Kernel/IUTReadoutTool.h"

inline void RawBankToUTLiteClusterAlg::createCluster(const UTTell1Board* aBoard,  const UTDAQ::version& bankVersion,
                                                     const UTClusterWord& aWord, LHCb::UTLiteCluster::UTLiteClusters& fCont,
                                                     const bool isUT) const{

  const unsigned int fracStrip = aWord.fracStripBits();
  const UTTell1Board::chanPair chan = aBoard->DAQToOffline(fracStrip, bankVersion, UTDAQ::UTStripRepresentation(aWord.channelID()));
  fCont.emplace_back( chan.second,
                      aWord.pseudoSizeBits(),
                      aWord.hasHighThreshold(),
                      chan.first,
                      isUT);
}

#endif //  RAWBANKTOSTLITECLUSTERALG_H
