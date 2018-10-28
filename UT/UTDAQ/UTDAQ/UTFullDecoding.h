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
#ifndef UTFULLDECODING_H
#define UTFULLDECODING_H 1

// Include files
#include "Kernel/UTAlgBase.h"
#include "Event/RawBank.h"

/** @class UTFullDecoding UTFullDecoding.h
 *
 *  Algorithm to decode the NZS UT data in the RawEvent buffer into UUTELL1Data
 *  objects. Job options:
 *  - \b PrintErrorInfo: Flag to print out errors from event info.
 *  - \b InputLocation: Location of RawEvent
 *  - \b OutputLocation: Location of NZS output data, e.g. UTFull
 *  - \b EventInfoLocation: Location of event info data
 *  \sa{https://edms.cern.ch/document/692431/3}.
 *
 *  @author Andy Beiter (based on code by Mathias Knecht, Jeroen van Tilburg)
 *  @date   2018-09-04
 */
class UTFullDecoding : public UT::AlgBase {

public:

  /// Standard constructor
  UTFullDecoding( const std::string& name, ISvcLocator* pSvcLocator );
  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  LHCb::RawBank::BankType m_bankType;

  // job options
  Gaudi::Property<bool> m_printErrorInfo
    {this, "PrintErrorInfo", true};///< Flag to print out errors from event info
  std::string m_inputLocation;     ///< Location of RawEvent
  std::string m_outputLocation;    ///< Location of NZS output data, e.g. UTFull
  std::string m_eventInfoLocation; ///< Location of event info data
};
#endif // UTFULLDECODING_H
