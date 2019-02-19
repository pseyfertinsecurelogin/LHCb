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
#ifndef CHECKCALOBANK_H
#define CHECKCALOBANK_H 1

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
// from Event
#include "Event/RawEvent.h"

/** @class CheckCaloBank CheckCaloBank.h
 *
 *    @author: Olivier Deschamps
 *    @date:   O6 Septembre 2006
 */

class CheckCaloBank : public GaudiAlgorithm {
public:
  CheckCaloBank( const std::string& name, ISvcLocator* pSvcLocator );
  StatusCode initialize() override;
  StatusCode execute() override;

private:
  LHCb::RawBank::BankType m_trig;
  LHCb::RawBank::BankType m_bank;
  std::string             m_typ;

  /// List of locations in the transient store to search the RawEvent object.
  std::vector<std::string> m_rawEventLocations;
};
#endif //    CHECKCALOBANK_H
