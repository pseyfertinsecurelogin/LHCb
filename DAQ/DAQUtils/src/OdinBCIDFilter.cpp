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
#include "Event/ODIN.h"
#include "GaudiAlg/FilterPredicate.h"

//-----------------------------------------------------------------------------
// Implementation file for class : OdinBCIDFilter
//
// 2008-28-10 : Olivier Deschamps
//-----------------------------------------------------------------------------

namespace {

  enum class Comparator_t { eq, neq, lt, le, gt, ge };

  const char* toString( Comparator_t cmp ) {
    switch ( cmp ) {
    case Comparator_t::eq:
      return "==";
    case Comparator_t::neq:
      return "!=";
    case Comparator_t::lt:
      return "<";
    case Comparator_t::le:
      return "<=";
    case Comparator_t::gt:
      return ">";
    case Comparator_t::ge:
      return ">=";
    }
    throw "IMPOSSIBLE!";
  }

  std::ostream& toStream( Comparator_t cmp, std::ostream& os ) { return os << std::quoted( toString( cmp ), '\'' ); }

  StatusCode parse( Comparator_t& cmp, const std::string& in ) {
    for ( Comparator_t ref : {Comparator_t::eq, Comparator_t::neq, Comparator_t::lt, Comparator_t::le, Comparator_t::gt,
                              Comparator_t::ge} ) {
      if ( in != toString( ref ) ) continue;
      cmp = ref;
      return StatusCode::SUCCESS;
    }
    if ( in == "=" ) {
      cmp = Comparator_t::eq;
      return StatusCode::SUCCESS;
    }
    return StatusCode::FAILURE;
  }

  bool apply( Comparator_t cmp, long bx, long mask, long value ) {
    switch ( cmp ) {
    case Comparator_t::eq:
      return ( bx & mask ) == value;
    case Comparator_t::neq:
      return ( bx & mask ) != value;
    case Comparator_t::ge:
      return ( bx & mask ) >= value;
    case Comparator_t::gt:
      return ( bx & mask ) > value;
    case Comparator_t::le:
      return ( bx & mask ) <= value;
    case Comparator_t::lt:
      return ( bx & mask ) < value;
    }
    throw "IMPOSSIBLE";
  }

} // namespace

/** @class OdinBCIDFilter OdinBCIDFilter.h component/OdinBCIDFilter.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2008-02-05
 */
class OdinBCIDFilter : public Gaudi::Functional::FilterPredicate<bool( const LHCb::ODIN& )> {
public:
  /// Standard constructor
  OdinBCIDFilter( const std::string& name, ISvcLocator* pSvcLocator )
      : FilterPredicate{name, pSvcLocator, {"ODINLocation", LHCb::ODINLocation::Default}} {}

  StatusCode initialize() override;                          ///< Algorithm initialization
  bool       operator()( const LHCb::ODIN& ) const override; ///< Algorithm execution
  StatusCode finalize() override;                            ///< Algorithm finalization

private:
  // Default : filtering odd-parity BXID
  Gaudi::Property<long>         m_mask{this, "Mask", 0x1};
  Gaudi::Property<long>         m_value{this, "Value", 1};
  Gaudi::Property<Comparator_t> m_comparator{this, "Comparator", Comparator_t::eq};
  Gaudi::Property<bool>         m_revert{this, "Revert", false};

  mutable Gaudi::Accumulators::BinomialCounter<> m_counter{this, "Accepted"};
};
// Declaration of the Algorithm Factory
DECLARE_COMPONENT( OdinBCIDFilter )

//=============================================================================
// Initialization
//=============================================================================
StatusCode OdinBCIDFilter::initialize() {
  return FilterPredicate::initialize().andThen( [&] {
    info() << " Filtering criteria : " << ( m_revert ? "!" : "" ) << "[(BXID & " << m_mask.value() << ")"
           << toString( m_comparator.value() ) << " " << m_value.value() << "]" << endmsg;
  } );
}

//=============================================================================
// Main execution
//=============================================================================
bool OdinBCIDFilter::operator()( const LHCb::ODIN& odin ) const {
  bool decision = apply( m_comparator, odin.bunchId(), m_mask, m_value );
  if ( m_revert ) decision = !decision;
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Accept event : " << odin.bunchId() << " : " << decision << endmsg;
  m_counter += decision;
  return decision;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode OdinBCIDFilter::finalize() {
  info() << " Filtering criteria : " << ( m_revert ? "!" : "" ) << "[(BXID & " << m_mask.value() << ")"
         << toString( m_comparator.value() ) << " " << m_value.value() << "]" << endmsg;
  info() << "   ---> " << m_counter.nTrueEntries() << " accepted events among " << m_counter.nEntries() << endmsg;
  return FilterPredicate::finalize(); // must be called after all other actions
}

//=============================================================================
