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
#ifndef FTRAWBANKENCODER_H
#define FTRAWBANKENCODER_H 1

#include "IFTReadoutTool.h"

#include "Event/FTCluster.h"
#include "Event/RawEvent.h"

#include "GaudiAlg/GaudiAlgorithm.h"

#include <cstdint>

/** @class FTRawBankEncoder FTRawBankEncoder.h
 *  Encode the FTCLusters into raw banks
 *
 *  @author Olivier Callot
 *  @date   2012-05-11
 */
class FTRawBankEncoder : public GaudiAlgorithm {
public:
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode execute() override; ///< Algorithm execution

private:
  PublicToolHandle<IFTReadoutTool> m_readoutTool = {this, "FTReadoutTool", "FTReadoutTool"};

  Gaudi::Property<std::string> m_inputLocation{this, "InputLocation", LHCb::FTClusterLocation::Default};
  Gaudi::Property<std::string> m_outputLocation{this, "OutputLocation", LHCb::RawEventLocation::Default};
};
#endif // FTRAWBANKENCODER_H
