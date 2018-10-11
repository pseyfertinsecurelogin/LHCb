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
// from LHCb
#include "Event/ODIN.h"
// local
#include "OdinBCIDFilter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : OdinBCIDFilter
//
// 2008-28-10 : Olivier Deschamps
//-----------------------------------------------------------------------------
namespace {
bool apply(OdinBCIDFilter_details::Comparator_t cmp, long bx, long mask, long value) {
  switch(cmp) {
      case OdinBCIDFilter_details::Comparator_t::eq  : return (bx & mask) == value;
      case OdinBCIDFilter_details::Comparator_t::neq : return (bx & mask) != value;
      case OdinBCIDFilter_details::Comparator_t::ge  : return (bx & mask) >= value;
      case OdinBCIDFilter_details::Comparator_t::gt  : return (bx & mask) >  value;
      case OdinBCIDFilter_details::Comparator_t::le  : return (bx & mask) <= value;
      case OdinBCIDFilter_details::Comparator_t::lt  : return (bx & mask) <  value;
  }
  throw "IMPOSSIBLE";
}
}

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( OdinBCIDFilter )

//=============================================================================
// Initialization
//=============================================================================
StatusCode OdinBCIDFilter::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;
  //
  info() << " Filtering criteria : " << ( m_revert ? "!" : "" )
         << "[(BXID & " << m_mask.value() <<")" << toString(m_comparator.value())
         << " " << m_value.value() << "]" << endmsg;

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode OdinBCIDFilter::execute() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  ++m_all;

  // treat trivial requests
  setFilterPassed(false);

  // get ODIN
  LHCb::ODIN* odin = getIfExists<LHCb::ODIN>(LHCb::ODINLocation::Default);
  if( !odin ) return Error("ODIN cannot be loaded",StatusCode::FAILURE);

  bool decision = apply(m_comparator,odin->bunchId(),m_mask,m_value);
  if (m_revert) decision = !decision;
  if (msgLevel(MSG::DEBUG)) debug() << "Accept event : " << odin->bunchId() << " : " << decision << endmsg;
  setFilterPassed( decision );
  if (decision) ++m_acc;

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode OdinBCIDFilter::finalize() {
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Finalize" << endmsg;

  info() << " Filtering criteria : " << ( m_revert ? "!" : "" )
         << "[(BXID & " << m_mask.value() <<")" << toString(m_comparator.value())
         << " " << m_value.value() << "]" << endmsg;
  info() << "   ---> " << m_acc << " accepted events among " << m_all << endmsg;

  return GaudiAlgorithm::finalize();  // must be called after all other actions
}

//=============================================================================
