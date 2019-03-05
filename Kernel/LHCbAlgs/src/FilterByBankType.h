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
#ifndef FILTERBYRUNEVENT_H
#define FILTERBYRUNEVENT_H 1

#include "Event/RawBank.h"

#include "GaudiAlg/FilterPredicate.h"

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
class FilterByBankType final : public Gaudi::Functional::FilterPredicate<bool( const LHCb::RawEvent& )> {

public:
  /// Standard constructor
  FilterByBankType( const std::string& name, ISvcLocator* pSvcLocator );

  bool operator()( const LHCb::RawEvent& ) const override; ///< Algorithm execution

private:
  /// Internal enum vector of selected bank types
  std::vector<LHCb::RawBank::BankType> m_bankTypes;

  // Job options
  Gaudi::Property<bool> m_passSelect{this, "PassSelectedEvents", true}; ///< Flag to determine selection logic.
  Gaudi::Property<std::vector<std::string>> m_bankNames{
      this,
      "BankNames",
      {".*Error"},
      [this]( const Property& ) {
        // Loop over the list of possible BankTypes
        info() << ( ( m_passSelect ) ? "Selecting" : "Ignoring" ) << " events with banks: ";
        for ( unsigned int iBank = 0; iBank < LHCb::RawBank::LastType; ++iBank ) {
          const auto bankName = LHCb::RawBank::typeName( LHCb::RawBank::BankType( iBank ) );
          // make an enum vector from the string vector of bank names
          for ( const auto& BankName : m_bankNames ) {
            // Use the regular expression
            std::regex e( BankName );
            if ( std::regex_match( bankName, e ) ) {
              m_bankTypes.push_back( LHCb::RawBank::BankType( iBank ) );
              info() << bankName << "(" << iBank << ")  ";
            }
          }
        }
        info() << endmsg;
      },
      Gaudi::Details::Property::ImmediatelyInvokeHandler{true}}; ///< List of banks to look for.
};

#endif // FILTERBYRUNEVENT_H
