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
#ifndef L0CALO_L0CALOCANDIDATESFROMRAWBANK_H
#define L0CALO_L0CALOCANDIDATESFROMRAWBANK_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"

// From Event
#include "Event/RawEvent.h"
#include "Event/L0CaloCandidate.h"

// forward declaration
namespace LHCb {
  class RawBankReadoutStatus ;
}
class DeCalorimeter ;

static const InterfaceID IID_L0CaloCandidatesFromRawBank ( "L0CaloCandidatesFromRawBank", 4, 0 );

/** @class L0CaloCandidatesFromRawBank L0CaloCandidatesFromRawBank.h
 *  Tool to convert the Raw Bank L0CaloCandidate to TES containers
 *
 *  @author Olivier Callot
 *  @author Marie-Helene Schune
 *  @author Patrick Robbe
 *  @date   2006-11-06
 */
class L0CaloCandidatesFromRawBank : public GaudiTool {
public:

  // Return the interface ID
  static const InterfaceID& interfaceID() { return IID_L0CaloCandidatesFromRawBank; }

  /// Standard constructor
  L0CaloCandidatesFromRawBank( const std::string& type,
                               const std::string& name,
                               const IInterface* parent);

  virtual ~L0CaloCandidatesFromRawBank( ); ///< Destructor

  StatusCode initialize() override; ///< Initialisation

  void convertRawBankToTES( std::vector<std::vector< unsigned int > >& data,
                            LHCb::L0CaloCandidates * outFull,
                            LHCb::L0CaloCandidates * out,
                            const int version ,
                            LHCb::RawBankReadoutStatus & readoutStatus );

protected:

private:
  DeCalorimeter* m_ecal;         ///< ECAL detector element
  DeCalorimeter* m_hcal;         ///< HCAL detector element
  double         m_etScale;      ///< Conversion int -> Et.
  bool           m_doDebugDecoding ; ///< Store intermediate information for debug
  bool           m_fixFor2016 ;  ///< Fix energy to correspond to 2016 conditions
  bool           m_simulation ;  ///< Data or MC, relevant only for fixFor2016

  int correctedEnergy( const int oldEnergy , const LHCb::CaloCellID & id ) const ;
};
#endif // L0CALO_L0CALOCANDIDATESFROMRAWBANK_H
