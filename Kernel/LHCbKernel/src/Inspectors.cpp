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
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <ostream>
#include <sstream>
#include <utility>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IAlgManager.h"
#include "GaudiKernel/IAlgorithm.h"
#include "GaudiKernel/INamedInterface.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/System.h"
#include "GaudiKernel/TypeNameString.h"
// ============================================================================
// LHcbKernel
// ============================================================================
#include "Kernel/Inspectors.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/format.hpp"
// ============================================================================
/** @file
 *  The implementation file fior variour inspectors
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2008-08-02
 */
// ============================================================================
/*  simple function to extract from the given component
 *  the list of properties, specified by name
 *
 *  @param cmp the component
 *  @param name list of property names
 *  @param output (OUTPUT) the list of properties
 *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
 *  @date 2008-08-02
 */
// ============================================================================
void Gaudi::Utils::properties( const IInterface* cmp, const Gaudi::Utils::Names& names,
                               Gaudi::Utils::Properties& output ) {
  // 1) clear the output container
  output.clear();
  // 2) check arguments
  if ( nullptr == cmp || names.empty() ) { return; } // RETURN
  // 3) fill the output container
  for ( auto item = names.begin(); names.end() != item; ++item ) {
    // get the property
    const ::Property* p = Gaudi::Utils::getProperty( cmp, *item );
    if ( nullptr == p ) { continue; } // CONTINUE
    // put the proeprty into the output container
    output.push_back( p );
  }
}
// ============================================================================
/* simple function to extract from the given component
 *  the list of properties, specified by name
 *
 *  @param cmp the component
 *  @param name list of property names
 *  @retutn the list of properties
 *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
 *  @date 2008-08-02
 */
// ============================================================================
Gaudi::Utils::Properties Gaudi::Utils::properties( const IInterface* cmp, const Gaudi::Utils::Names& names ) {
  Gaudi::Utils::Properties props;
  Gaudi::Utils::properties( cmp, names, props );
  return props; // RETURN
}
// ============================================================================
// Inspectors
// ============================================================================
namespace {
  // own printout             1   2 3             4
  const std::string s_fmt1 = "%-2d%s%-30.30s%|45t|%-30.30s";
  // global layout            1       2
  const std::string s_fmt2 = "%s%|65t|%s";
  // ==========================================================================
} // namespace
// ============================================================================
/*  constructor the list of properties
 *  @param props   list of properties to be inspected
 */
// ============================================================================
LHCb::Inspector::Inspector( Gaudi::Utils::Names props )
    : m_members(), m_names( std::move( props ) ), m_algMgr( Gaudi::svcLocator() ) {
  m_members.emplace_back( "Members" );
  m_members.emplace_back( "TopAlg" );
}
// ============================================================================
/* constructor the list of properties
 *  @param props   list of properties to be inspected
 *  @param members list of structural properties
 */
// ============================================================================
LHCb::Inspector::Inspector( Gaudi::Utils::Names props, Gaudi::Utils::Names members )
    : m_members( std::move( members ) ), m_names( std::move( props ) ), m_algMgr( Gaudi::svcLocator() ) {}
// ============================================================================
StatusCode LHCb::Inspector::inspect( const IInterface* component, std::ostream& stream, const size_t level ) const {
  if ( nullptr == component ) { return StatusCode( 500 ); } // REUTRN
  // ==========================================================================
  // create the format object:
  boost::format fmt1( s_fmt1 );
  // start the formatting
  fmt1 % level;                             // 1) # level
  fmt1 % std::string( 3 * level + 1, ' ' ); // 2) indentation
  {
    // get the name
    auto*                    _cmp = const_cast<IInterface*>( component );
    SmartIF<INamedInterface> inamed( _cmp );
    if ( !inamed ) {
      fmt1 % "<UNKNOWN>";
    } else {
      fmt1 % inamed->name();
    } // 3) name
  }
  fmt1 % System::typeinfoName( typeid( *component ) ); // 4) type
  // ==========================================================================
  std::string part1 = fmt1.str();
  // check own properties
  Gaudi::Utils::Properties props = Gaudi::Utils::properties( component, m_names );
  // print properties
  if ( props.empty() ) {
    boost::format fmt2( s_fmt2 );
    fmt2 % part1 % "";
    stream << fmt2 << std::endl;
  }
  for ( auto ip = props.begin(); props.end() != ip; ++ip ) {
    boost::format fmt2( s_fmt2 );
    fmt2 % part1 % ( **ip );
    stream << fmt2 << std::endl;
    part1.clear();
  }
  // check the structural properties
  Gaudi::Utils::Properties members = Gaudi::Utils::properties( component, m_members );
  // no structural properties ?
  if ( members.empty() ) { return StatusCode( StatusCode::SUCCESS, true ); } // REUTRN
  //
  for ( auto im = members.begin(); members.end() != im; ++im ) {
    const Property* m = *im;
    if ( nullptr == m ) { continue; } // CONTINUE
    // get the property
    using Members = std::vector<std::string>;
    SimpleProperty<Members> vm;
    // assign from the source:
    vm.assign( *m ); // ASSIGN PROPERTY
    // Loop over all "members" (or "TopAlgs")
    const Members& vct = vm.value();
    for ( auto id = vct.begin(); vct.end() != id; ++id ) {
      /// decode the string into "type" and "name" :
      Gaudi::Utils::TypeNameString item( *id );
      /// get the algorithm by name
      IAlgorithm* alg = nullptr;
      StatusCode  sc  = m_algMgr->getAlgorithm( item.name(), alg );
      if ( sc.isFailure() ) { return StatusCode{510 + sc.getCode()}; } // RETURN
      if ( nullptr == alg ) { return StatusCode{503}; }                // RETURN
      //
      // start the recursion here!!
      sc = inspect( alg, stream, level + 1 );
      if ( sc.isFailure() ) { return sc; } // RETURN
    }                                      // end of the loop over "members/topalgs"
  }                                        // end of the loop over structural properties
  //
  return StatusCode( StatusCode::SUCCESS, true ); // RETURN
}
// ============================================================================
/*  inspect the component
 *  @param component the component to be inspected
 *  @param level the recursion level
 *  @return the inspection result
 */
// ============================================================================
std::string LHCb::Inspector::inspect( const IInterface* component, const size_t level ) const {
  std::ostringstream out;
  StatusCode         sc = inspect( component, out, level );
  if ( sc.isFailure() ) { out << "StatusCode: " << sc << std::endl; }
  return out.str();
}

// ============================================================================
// The END
// ============================================================================
