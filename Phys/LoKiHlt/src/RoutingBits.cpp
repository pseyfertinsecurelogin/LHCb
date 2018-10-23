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
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/SmartIF.h"
// ============================================================================
// DAQEvent
// ============================================================================
#include "Event/RawEvent.h"
// ============================================================================
// HltInterfaces
// ============================================================================
#include "Kernel/ReadRoutingBits.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/ILoKiSvc.h"
#include "LoKi/RoutingBits.h"
// ============================================================================
/** @file
 *  set of helper function to deal with "hlt-routing-bits"
 *  @see Hlt::firedRoutingBits
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2010-05-17
 */
// ============================================================================
// constructor from the bis
// ============================================================================
LoKi::HLT::RoutingBits::RoutingBits( std::vector<unsigned int>  bits )
  : LoKi::AuxFunBase ( std::tie ( bits ) )
  , m_bits    ( std::move(bits) )
{
  std::sort ( m_fired.begin() , m_fired.end() ) ;
}
// ============================================================================
// MANDATORY: clone method ("virtual contructor")
// ============================================================================
LoKi::HLT::RoutingBits* LoKi::HLT::RoutingBits::clone () const
{ return new RoutingBits ( *this ) ; }
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::HLT::RoutingBits::operator()(  ) const
{
  if ( !sameEvent() || 0 >= event() || m_fired.empty() ) { getFired() ; }
  //
  return std::any_of( std::begin(m_bits), std::end(m_bits),
                      [&](unsigned int bit) {
            return std::binary_search( m_fired.begin() , m_fired.end  () , bit );
  });
}
// ============================================================================
// get the fired bits
// ============================================================================
std::size_t LoKi::HLT::RoutingBits::getFired() const
{
  LoKi::ILoKiSvc*   loki   = lokiSvc()  ;
  Assert ( 0 != loki   , "Uanble to get LoKi  Service" ) ;
  SmartIF<IDataProviderSvc> evtSvc  ( loki ) ;
  Assert ( !(!evtSvc)  , "Uanble to get Event Service" ) ;
  SmartDataPtr<LHCb::RawEvent> raw
    ( evtSvc , LHCb::RawEventLocation::Default ) ;
  Assert ( !(!raw)     , "Unable to get Raw Event!" ) ;
  //
  m_fired = Hlt::firedRoutingBits ( raw ) ;
  std::sort ( m_fired.begin() , m_fired.end() ) ;
  //
  return m_fired.size() ;
}
// ============================================================================
// OPTIONAL : the nice printout
// ============================================================================
std::ostream& LoKi::HLT::RoutingBits::fillStream ( std::ostream& s ) const
{
  s << "routingBits ( " ;
  //
  auto i = m_bits.begin();
  if (i!=m_bits.end()) s << *i++;
  while (i!=m_bits.end()) s << "," << *i++;
  s << " ) ";
  Gaudi::Utils::toStream ( m_bits , s ) ;
  return s << " ) " ;
}
// ============================================================================


// ============================================================================
// The END
// ============================================================================
