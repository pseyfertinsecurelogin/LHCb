/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef FTRAWBANKDECODER_H
#define FTRAWBANKDECODER_H 1

// Include files
// from Gaudi
#include "Event/FTLiteCluster.h"
#include "Event/RawEvent.h"
#include "FTRawBankParams.h"
#include "GaudiAlg/FixTESPath.h"
#include "GaudiAlg/FunctionalUtilities.h"
#include "GaudiAlg/Transformer.h"
#include "IFTReadoutTool.h"
#include "Kernel/MultiIndexedContainer.h"

using namespace Gaudi::Functional;

using FTLiteClusters = LHCb::FTLiteCluster::FTLiteClusters;

/** @class FTRawBankDecoder FTRawBankDecoder.h
 *  Decode the FT raw bank into FTLiteClusters
 *
 *  @author Olivier Callot
 *  @date   2012-05-11
 */
class FTRawBankDecoder : public Transformer<FTLiteClusters( const LHCb::RawEvent& ),
                                            Gaudi::Functional::Traits::BaseClass_t<FixTESPath<Gaudi::Algorithm>>> {
public:
  /// Standard constructor
  FTRawBankDecoder( const std::string& name, ISvcLocator* pSvcLocator );

  FTLiteClusters operator()( const LHCb::RawEvent& rawEvent ) const override;

private:
  PublicToolHandle<IFTReadoutTool> m_readoutTool = {this, "FTReadoutTool", "FTReadoutTool"};

  // for MC, following property has to be same as cluster creator,
  // not sure how to ensure this
  Gaudi::Property<unsigned int> m_clusterMaxWidth{this, "ClusterMaxWidth", 4u, "Maximal cluster width"};
  // For the v2 and v3 decoding versions, opt out for the default initialization
  // of m_readouttool.
  Gaudi::Property<unsigned int> m_decodingVersion{
      this,
      "DecodingVersion",
      6u,
      [=]( auto& ) { this->m_readoutTool.setEnabled( this->m_decodingVersion > 3u ); },
      Gaudi::Details::Property::ImmediatelyInvokeHandler{true},
      "Set the decoding version"};

  template <unsigned int version>
  FTLiteClusters decode( LHCb::span<const LHCb::RawBank*>, unsigned int nClusters ) const;

  mutable Gaudi::Accumulators::MsgCounter<MSG::ERROR>   m_corrupt{this, "Possibly corrupt data. Ignoring the cluster."};
  mutable Gaudi::Accumulators::MsgCounter<MSG::WARNING> m_nonExistingModule{
      this, "Skipping cluster(s) for non-existing module."};
};

#endif // FTRAWBANKDECODER_H
