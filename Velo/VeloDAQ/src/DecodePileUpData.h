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
#ifndef DECODEPILEUPDATA_H
#define DECODEPILEUPDATA_H 1

// Include files
// from Gaudi
#include "DAQKernel/DecoderAlgBase.h"
#include "Event/VeloCluster.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Kernel/STLExtensions.h"

// from TELL1 Kernel:
#include "Tell1Kernel/PUTell1Core.h"
#include "Tell1Kernel/VeloDecodeConf.h"

/** @class DecodePileUpData DecodePileUpData.h
 *
 *
 *  @author Serena Oggero
 *  @date   2009-06-19
 */

// -----------------------------------
namespace LHCb {
  class RawEvent;
  class RawBank;
} // namespace LHCb
using namespace LHCb;
// -----------------------------------

namespace PuTell1 {
  enum BankTypes { LOPU_NZS = 57 };
}

class DecodePileUpData : public Decoder::AlgBase {
public:
  DecodePileUpData( const std::string& name, ISvcLocator* pSvcLocator ); /// Standard constructor
  StatusCode initialize() override;                                      ///< Algorithm initialization
  StatusCode execute() override;                                         ///< Algorithm execution

  StatusCode getRawEvent();
  StatusCode decode();
  StatusCode getPileUpBank();
  StatusCode writePUBanks( LHCb::VeloClusters*, LHCb::VeloClusters* );
  StatusCode decodePileUpBinary( LHCb::span<const LHCb::RawBank*> banks );
  StatusCode decodePileUpBinaryNZS( LHCb::span<const LHCb::RawBank*> banks );
  StatusCode findPileUpHitsBee( PuTell1::dataObject, int, int, LHCb::VeloClusters* );
  StatusCode findPileUpHitsBeeNZS( PuTell1::dataObject, int, int, LHCb::VeloClusters* );

  int         SensorId( int );
  std::string binary( unsigned int );
  void        inizializePUcontainer( PuTell1::DataTable PUcontainerBee );
  void        Fill( unsigned int wordIt, unsigned int word_Tot, const unsigned int* data_Ptr, int step,
                    PuTell1::DataTable PUcontainerBee );

private:
  LHCb::RawEvent* m_rawEvent;

  DataObjectWriteHandle<LHCb::VeloClusters> m_PUClusterLocation{this, "PUClusterLocation", "Raw/Velo/PUClusters"};
  DataObjectWriteHandle<LHCb::VeloClusters> m_PUClusterNZSLocation{this, "PUClusterNZSLocation",
                                                                   "Raw/Velo/PUClustersNZS"};
  Gaudi::Property<bool>                     m_isNonZeroSupp{this, "NonZeroSupp", true};
  PuTell1::DataTable                        m_PUcontainerBee_NZS;
  PuTell1::DataTable                        m_PUcontainerBee;
};
#endif // DecodePileUpData_H
