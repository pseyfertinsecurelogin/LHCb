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
// $Id: FilterByBankType.h,v 1.1 2009-03-09 21:18:07 jvantilb Exp $
#ifndef FILTERBYRUNEVENT_H
#define FILTERBYRUNEVENT_H 1

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

/** @class FilterByBankType FilterByBankType.h
 *
 *  Algorithm that filters events based on the presence of specific banks.
 *
 *  Algorithm properties:
 *  @c FilterByBankType has the following properties:
 *  - @c PassSelectedEvents: A boolean with a default value of @c true.
 *    If @c PassSelectedEvents is set to @c true (the default),
 *    @c FilterByBankType will pass only the specified events and reject all
 *    other events. If @c PassSelectedEvents is set to @c false,
 *    @c FilterByBankType will reject the specified events and pass all
 *    other events.
 *  - @c BankNames: A vector of strings with the bank names. If the event
 *    contains a bank which is in this list, the event is selected. Regular
 *    expressions (perl-like) can be used. By default
 *    all events with error banks from any subdetector are selected. The
 *    possible bank names can be found:
 *    @c DAQEvent/Event/RawBank.h in the LHCB project.
 *  - @c InputLocation: The location of the @c RawEvent. By default the
 *    location is taken from @c  RawEventLocation::Default.
 *
 *  Example of usage:
 *
 *  In order to configure an algorithm that selects events containing an
 *  ODIN bank or a pedestal bank from any subdetector:
 *  @code
 *
 *  //
 *  ApplicationMgr.TopAlg += { 'FilterByBankType/SelectODINEvents' }
 *  SelectODINEvents.PassSelectedEvents = true;
 *  SelectODINEvents.BankNames          = {"ODIN", ".*Pedestal"};
 *
 *  @endcode
 *
 *  @author Jeroen van Tilburg
 *  @date   2009-03-06
 *
 */
class FilterByBankType final : public GaudiAlgorithm
{

public:

  /// Standard constructor
  FilterByBankType( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override;    ///< Algorithm execution
  StatusCode initialize() override; ///< Algorithm initialization

private:

  /// Internal enum vector of selected bank types
  std::vector<LHCb::RawBank::BankType> m_bankTypes;

  // Job options
  std::string m_inputLocation;          ///< Location in TES of the RawEvent
  std::vector<std::string> m_bankNames; ///< List of banks to look for.
  bool m_passSelect;                    ///< Flag to determine selection logic.

};

#endif // FILTERBYRUNEVENT_H
