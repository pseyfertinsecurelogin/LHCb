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
// C++ code for 'LHCb Tracking package(s)'
//
//   Author: A. Beiter (based on code by M. Needham)
//   Created: 2018-09-04

#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "GaudiAlg/Consumer.h"
#include "Kernel/IUTReadoutTool.h"
#include "Kernel/UTHistoAlgBase.h"
#include "Kernel/UTRawBankMap.h"
#include "Kernel/UTTell1ID.h"
#include <string>

/** @class UTRawBankMonitor UTRawBankMonitor.h
 *  UTCheckers/UTRawBankMonitor.h
 *
 *  Class for checking UT RAW buffer
 *
 *  @author A. Beiter (based on code by M.Needham)
 *  @date   2018-09-04
 */

class UTRawBankMonitor : public Gaudi::Functional::Consumer<void( LHCb::RawEvent const& ),
                                                            Gaudi::Functional::Traits::BaseClass_t<UT::HistoAlgBase>> {

public:
  /// constructor
  UTRawBankMonitor( const std::string& name, ISvcLocator* pSvcLocator )
      : Consumer{name,
                 pSvcLocator,
                 {"RawEventLocations", Gaudi::Functional::concat_alternatives( LHCb::RawEventLocation::Other,
                                                                               LHCb::RawEventLocation::Default )}} {};

  /// initialize
  StatusCode initialize() override;

  /// execute
  void operator()( const LHCb::RawEvent& ) const override;

private:
  StatusCode configureBankType();

  LHCb::RawBank::BankType m_bankType = LHCb::RawBank::LastType;
};

DECLARE_COMPONENT( UTRawBankMonitor )

using namespace LHCb;

//--------------------------------------------------------------------
//
//--------------------------------------------------------------------

StatusCode UTRawBankMonitor::initialize() {

  if ( histoTopDir().empty() ) setHistoTopDir( detType() + "/" );

  return UT::HistoAlgBase::initialize().andThen( &UTRawBankMonitor::configureBankType, this ); // configure banktype
}

void UTRawBankMonitor::operator()( const LHCb::RawEvent& rawEvt ) const {

  // execute once per event

  // init counters
  unsigned int maxBoardSize = 0;
  UTTell1ID    hotBoard( 0, detType() == "UT" );
  unsigned int eventDataSize = 0;

  const auto& tBanks = rawEvt.banks( m_bankType );
  for ( const auto* iterBank : tBanks ) {

    // board info....
    size_t    bankSize = iterBank->size() / sizeof( char );
    UTTell1ID aBoard( iterBank->sourceID(), detType() == "UT" );

    // event counters
    if ( bankSize > maxBoardSize ) {
      maxBoardSize = bankSize;
      hotBoard     = aBoard;
    }
    eventDataSize += bankSize;

    // histogram per board
    plot( (double)bankSize, "board data size", 0., 200., 200 );

    // data size per board
    // unsigned int id = (aBoard.region()*20) + aBoard.subID();
    // const std::map< unsigned int, unsigned int > & SourceIDToTELLmap = readoutTool()->SourceIDToTELLNumberMap();
    // unsigned int tellNumber = SourceIDToTELLmap.find(iterBank->sourceID())->second;
    unsigned int tellNumber = readoutTool()->SourceIDToTELLNumber( iterBank->sourceID() );

    // These hard coded numbers come from here: https://lbtwiki.cern.ch/bin/view/Online/Tell1PortNum
    unsigned int doubleLinkedUTtell1s[] = {1, 2, 3, 4, 5, 6, 8, 9, 10, 13, 14, 15};
    unsigned int numberOfLinks          = 1;

    if ( detType() == "UT" ) {
      for ( unsigned int i = 0; i < 12; i++ ) {
        if ( tellNumber == doubleLinkedUTtell1s[i] ) numberOfLinks = 2;
      }
    }

    double datasize = bankSize / (double)numberOfLinks;
    plot( tellNumber, "data size", 0., 100., 100, datasize );
    plot( tellNumber, "data size unnormalised", 0., 100., 100, (double)bankSize );

  } // iterBank

  // data size
  plot( (double)eventDataSize, 1, "event data size", 0., 10000., 500 );

  // include standard header HARDCODE !!!
  unsigned int headerSize = tBanks.size() * 2u;
  plot( (double)( eventDataSize + headerSize ), 2, "total data size", 0., 10000., 500 );

  plot( (double)maxBoardSize, 3, "hot board size", 0., 200., 200 );
  unsigned int id = ( hotBoard.region() * 20 ) + hotBoard.subID();
  plot( (double)id, 4, "hot board ID", 0., 100., 100 );
}

StatusCode UTRawBankMonitor::configureBankType() {
  m_bankType = UTRawBankMap::stringToType( detType() );
  return m_bankType != RawBank::Velo ? StatusCode::SUCCESS : StatusCode::FAILURE;
}
