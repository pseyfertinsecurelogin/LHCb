// ============================================================================
// Include files 
// ============================================================================
// STD & STL 
// ============================================================================
#include <algorithm>
#include <iterator>
#include <ostream>
#include <tuple>
#include <vector>

// ============================================================================
// DAQEvent
// ============================================================================
#include "Event/RawEvent.h"
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/SmartDataStorePtr.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/ToStream.h"
// ============================================================================
// HltInterfaces
// ============================================================================
#include "Kernel/ReadRoutingBits.h"
#include "LoKi/AuxFunBase.h"
#include "LoKi/Functor.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/ILoKiSvc.h"
#include "LoKi/Interface.h"
#include "LoKi/RoutingBits.h"

// ============================================================================
/** @file
 *  set of helper function to deal with "hlt-routing-bits"
 *  @see Hlt::firedRoutingBits 
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2010-05-17
 */
// ============================================================================
// constructor from the bit 
// ============================================================================
LoKi::HLT::RoutingBits::RoutingBits 
( const unsigned short bit ) 
  : LoKi::AuxFunBase ( std::tie ( bit ) ) 
  , m_bits    ( bit ) 
{}
// ============================================================================
// constructor from the bis 
// ============================================================================
LoKi::HLT::RoutingBits::RoutingBits 
( const unsigned short bit1 , 
  const unsigned short bit2 ) 
  : LoKi::AuxFunBase ( std::tie ( bit1 , bit2 ) ) 
  , m_bits    { bit1, bit2 }
{
  std::sort ( m_fired.begin() , m_fired.end() ) ;  
}
// ============================================================================
// constructor from the bis 
// ============================================================================
LoKi::HLT::RoutingBits::RoutingBits 
( const unsigned short bit1 , 
  const unsigned short bit2 , 
  const unsigned short bit3 ) 
  : LoKi::AuxFunBase ( std::tie ( bit1 , bit2 , bit3 ) ) 
  , m_bits    { bit1, bit2, bit3 }
{
  std::sort ( m_fired.begin() , m_fired.end() ) ;  
}
// ============================================================================
// constructor from the bis 
// ============================================================================
LoKi::HLT::RoutingBits::RoutingBits 
( const unsigned short bit1 , 
  const unsigned short bit2 , 
  const unsigned short bit3 , 
  const unsigned short bit4 ) 
  : LoKi::AuxFunBase ( std::tie ( bit1 , bit2 , bit3 , bit4 ) ) 
  , m_bits    { bit1, bit2, bit3, bit4 }
{
  std::sort ( m_fired.begin() , m_fired.end() ) ;  
}
// ============================================================================
// constructor from the bis 
// ============================================================================
LoKi::HLT::RoutingBits::RoutingBits 
( const std::vector<unsigned int>&  bits ) 
  : LoKi::AuxFunBase ( std::tie ( bits ) ) 
  , m_bits    ( bits ) 
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
LoKi::HLT::RoutingBits::result_type 
LoKi::HLT::RoutingBits::operator() 
  ( /* LoKi::HLT::RoutingBit::argument a */ ) const
{
  if ( !sameEvent() || 0 >= event() || m_fired.empty() ) { getFired() ; }
  //
  return std::any_of( std::begin(m_bits), std::end(m_bits),
                      [&](const unsigned int& bit) {
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
  switch ( m_bits .size() ) 
  {
  case 1 : 
    return  s << m_bits[0] << " ) " ;
  case 2 : 
    return  s << m_bits[0] << "," 
              << m_bits[1] << " ) " ;
  case 3 : 
    return  s << m_bits[0] << "," 
              << m_bits[1] << "," 
              << m_bits[2] << " ) " ;
  case 4 : 
    return  s << m_bits[0] << "," 
              << m_bits[1] << "," 
              << m_bits[2] << "," 
              << m_bits[3] << " ) " ;
  default: 
    break ;
  }
  Gaudi::Utils::toStream ( m_bits , s ) ;
  return s << " ) " ; 
}
// ============================================================================


// ============================================================================
// The END 
// ============================================================================
