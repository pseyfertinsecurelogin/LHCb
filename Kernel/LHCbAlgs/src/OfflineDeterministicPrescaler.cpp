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
#include "OfflineDeterministicPrescaler.h"
#include "DeterministicPrescalerGenerator.h"
#include <cmath>

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( OfflineDeterministicPrescaler )

OfflineDeterministicPrescaler::OfflineDeterministicPrescaler( const std::string& name, ISvcLocator* pSvcLocator )
    : FilterPredicate{name, pSvcLocator, {"RecHeader", LHCb::RecHeaderLocation::Default}}
    , m_initial{mixString( name.size(), name )} {}

StatusCode OfflineDeterministicPrescaler::initialize() {
  return GaudiAlgorithm::initialize().andThen( [&] {
    if ( msgLevel( MSG::DEBUG ) ) debug() << " generated initial value " << m_initial << endmsg;

    if ( m_acc != boost::integer_traits<uint32_t>::const_max ) {
      info() << "Prescaling events; keeping " << m_accFrac.value() << " of events " << endmsg;
    }
  } );
}

bool OfflineDeterministicPrescaler::accept( const LHCb::RecHeader& header ) const {
  auto x = mix64( mix32( mix64( m_initial, header.gpsTime() ), header.runNumber() ),
                  (unsigned long long)header.evtNumber() );
  if ( msgLevel( MSG::DEBUG ) )
    debug() << "  gpsTime: " << header.gpsTime() << "  run#: " << header.runNumber() << "  evt#: " << header.evtNumber()
            << "  -->  " << x << endmsg;

  // at this point, we assume 'x' to be uniformly distributed in [0,0xffffffff]
  // (and yes, this was verified to be sufficiently true on a sample of 10K MC events ;-)
  return x < m_acc;
}

bool OfflineDeterministicPrescaler::operator()( const LHCb::RecHeader& header ) const {
  const bool acc = ( ( m_acc == boost::integer_traits<uint32_t>::const_max ) || ( m_acc != 0 && accept( header ) ) );
  m_counter += acc;
  if ( msgLevel( MSG::DEBUG ) ) debug() << ( acc ? "Accepted" : "Rejected" ) << endmsg;
  return acc;
}
