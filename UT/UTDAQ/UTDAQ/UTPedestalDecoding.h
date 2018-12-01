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
#ifndef UTPEDESTALDECODING_H
#define UTPEDESTALDECODING_H 1

// Include files
// from Gaudi
#include "Kernel/UTAlgBase.h"
#include "Event/RawBank.h"

/** @class UTPedestalDecoding UTPedestalDecoding.h
 *
 *  Algorithm to decode the pedestal bank in the RawEvent buffer into
 *  UTTELL1Data objects. Job options:
 *  - \b InputLocation: Location of RawEvent
 *  - \b OutputLocation: Location of output pedestal data, e.g. TTPedestal
 *  \sa{http://edms.cern.ch/doc/695007}.
 *
 *  @author Andy Beiter (based on code by Mathias Knecht, Jeroen van Tilburg)
 *  @date   2018-09-04
 */
class UTPedestalDecoding : public UT::AlgBase {

public:
  /// Standard constructor
  UTPedestalDecoding( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  LHCb::RawBank::BankType m_bankType;

  // job options
  std::string m_inputLocation;  ///< Location of RawEvent
  std::string m_outputLocation; ///< Location of output pedestal (UTTELL1Data)

};
#endif // UTPEDESTALDECODING_H
