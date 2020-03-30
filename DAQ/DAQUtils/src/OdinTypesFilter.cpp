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
#include "Gaudi/Algorithm.h"
#include "GaudiAlg/FilterPredicate.h"
#include "GaudiAlg/FixTESPath.h"
#include "GaudiKernel/ParsersFactory.h"
#include <algorithm>
#include <iomanip>

//-----------------------------------------------------------------------------
// Implementation file for class : OdinTypesFilter
//
// 2008-02-05 : Olivier Deschamps
//-----------------------------------------------------------------------------

namespace {
  //=======================================================
  enum class Log_t { And, Or };

  const char* toString( Log_t op ) {
    switch ( op ) {
    case Log_t::And:
      return "AND";
    case Log_t::Or:
      return "OR";
    }
    throw "IMPOSSIBLE";
  }

  StatusCode parse( Log_t& op, const std::string& in ) {
    if ( in == "AND" || in == "And" || in == "and" ) {
      op = Log_t::And;
      return StatusCode::SUCCESS;
    }
    if ( in == "OR" || in == "Or" || in == "or" ) {
      op = Log_t::Or;
      return StatusCode::SUCCESS;
    }
    return StatusCode::FAILURE;
  }

  std::ostream& toStream( const Log_t& op, std::ostream& os ) { return os << std::quoted( toString( op ), '\'' ); }
  //=======================================================

  template <typename Type, int Max>
  struct odin_type_t {
    int type;

    static constexpr odin_type_t All() { return {-1}; }

    friend constexpr bool operator==( const odin_type_t<Type, Max>& lhs, const odin_type_t<Type, Max>& rhs ) {
      return lhs.type == rhs.type;
    }

    friend constexpr bool operator==( const Type& lhs, const odin_type_t<Type, Max>& rhs ) {
      return rhs == All() || lhs == (Type)rhs.type;
    }

    friend constexpr bool operator==( const odin_type_t<Type, Max>& lhs, const Type& rhs ) { return rhs == lhs; }
  };

  template <typename Type, int Max>
  StatusCode parse( odin_type_t<Type, Max>& t, const std::string& in ) {
    if ( in == "ALL" ) {
      t = odin_type_t<Type, Max>::All();
      return StatusCode::SUCCESS;
    }
    for ( int k = 0; k <= Max; ++k ) {
      std::ostringstream s;
      s << (Type)k;
      if ( s.str() == in ) {
        t.type = k;
        return StatusCode::SUCCESS;
      }
    }
    return StatusCode::FAILURE;
  }

  template <typename Type, int Max>
  StatusCode parse( std::vector<odin_type_t<Type, Max>>& v, const std::string& in ) {
    std::vector<std::string> vs;
    using Gaudi::Parsers::parse;
    auto sc = parse( vs, in );
    if ( !sc ) return sc;
    v.clear();
    v.reserve( vs.size() );
    try {
      std::transform( vs.begin(), vs.end(), std::back_inserter( v ), []( const std::string& s ) {
        odin_type_t<Type, Max> t{};
        auto                   sc = parse( t, s );
        if ( !sc ) throw GaudiException( "Bad Parse", "", sc );
        return t;
      } );
    } catch ( const GaudiException& ge ) { return ge.code(); }
    return StatusCode::SUCCESS;
  }

  template <typename Type, int Max>
  std::string toString( odin_type_t<Type, Max>& t ) {
    if ( t == odin_type_t<Type, Max>::All() ) return "ALL";
    std::ostringstream oss;
    oss << (Type)t.type;
    return oss.str();
  }

  template <typename Type, int Max>
  std::ostream& toStream( const odin_type_t<Type, Max>& t, std::ostream& os ) {
    if ( t == odin_type_t<Type, Max>::All() ) return os << "\'ALL\'";
    return os << '\'' << (Type)t.type << '\'';
  }

  template <typename Type, int Max>
  std::ostream& operator<<( std::ostream& os, const odin_type_t<Type, Max>& t ) {
    return toStream( t, os );
  }

  //=====

  using odin_trigger_type_t = odin_type_t<LHCb::ODIN::TriggerType, ( LHCb::ODIN::Word8Masks::TriggerTypeMask >>
                                                                     LHCb::ODIN::Word8Bits::TriggerTypeBits )>;

  using odin_readout_type_t = odin_type_t<LHCb::ODIN::ReadoutTypes, ( LHCb::ODIN::Word8Masks_v4::ReadoutTypeMask >>
                                                                      LHCb::ODIN::Word8Bits_v4::ReadoutTypeBits )>;

  using odin_bx_type_t =
      odin_type_t<LHCb::ODIN::BXTypes, ( LHCb::ODIN::Word8Masks::BXTypeMask >> LHCb::ODIN::Word8Bits::BXTypeBits )>;

  using odin_calibration_type_t =
      odin_type_t<LHCb::ODIN::CalibrationTypes,
                  ( LHCb::ODIN::Word8Masks::CalibrationTypeMask >> LHCb::ODIN::Word8Bits::CalibrationTypeBits )>;

  template <typename... Args>
  bool boolean_combine( Log_t op, Args&&... args ) {
    switch ( op ) {
    case Log_t::And:
      return ( ... && std::forward<Args>( args ) );
    case Log_t::Or:
      return ( ... || std::forward<Args>( args ) );
    }
    throw std::runtime_error( std::string{"IMPOSSIBLE: "} + __func__ );
  }

} // namespace

/** @class OdinTypesFilter OdinTypesFilter.h component/OdinTypesFilter.h
 *  @author Olivier Deschamps
 *  @date   2008-02-05
 */
class OdinTypesFilter
    : public Gaudi::Functional::FilterPredicate<bool( LHCb::ODIN const& ),
                                                Gaudi::Functional::Traits::BaseClass_t<FixTESPath<Gaudi::Algorithm>>> {
public:
  /// Standard constructor
  OdinTypesFilter( const std::string& name, ISvcLocator* pSvcLocator )
      : FilterPredicate( name, pSvcLocator, {"ODIN", LHCb::ODINLocation::Default} ) {}

  StatusCode initialize() override;                          ///< Algorithm initialization
  bool       operator()( const LHCb::ODIN& ) const override; ///< Algorithm execution
  StatusCode finalize() override;                            ///< Algorithm finalization

private:
  Gaudi::Property<Log_t>                                m_log{this, "Logical", Log_t::And};
  Gaudi::Property<std::vector<odin_trigger_type_t>>     m_trs{this, "TriggerTypes", {odin_trigger_type_t::All()}};
  Gaudi::Property<std::vector<odin_bx_type_t>>          m_bxs{this, "BXTypes", {odin_bx_type_t::All()}};
  Gaudi::Property<std::vector<odin_readout_type_t>>     m_ros{this, "ReadoutTypes", {odin_readout_type_t::All()}};
  Gaudi::Property<std::vector<odin_calibration_type_t>> m_cls{
      this, "CalibrationTypes", {odin_calibration_type_t::All()}};
  Gaudi::Property<int>                           m_winmin{this, "TAEWindowMoreThan", -1};
  Gaudi::Property<int>                           m_winmax{this, "TAEWindowLessThan", 99};
  mutable Gaudi::Accumulators::BinomialCounter<> m_acc{this, "#accept"};
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( OdinTypesFilter )

//=============================================================================
// Initialization
//=============================================================================
StatusCode OdinTypesFilter::initialize() {
  return FilterPredicate::initialize().andThen( [&] {
    // selection :
    info() << "Accepted BXTypes : " << m_bxs.value() << endmsg;
    info() << toString( m_log.value() ) << endmsg;
    info() << "Accepted TriggerTypes : " << m_trs.value() << endmsg;
    info() << toString( m_log.value() ) << endmsg;
    info() << "Accepted ReadoutTypes : " << m_ros.value() << endmsg;
    info() << toString( m_log.value() ) << endmsg;
    info() << "Accepted CalibrationTypes : " << m_cls.value() << endmsg;
    info() << toString( m_log.value() ) << endmsg;
    info() << "TAE Window in [" << m_winmin + 1 << "," << m_winmax - 1 << "]" << endmsg;

    // warn about trivial requests
    if ( m_log == Log_t::And && boolean_combine( Log_t::Or, m_bxs.empty(), m_trs.empty(), m_ros.empty(),
                                                 m_winmin >= m_winmax, m_cls.empty() ) )
      warning() << "BXTypes, TriggerTypes, ReadoutTypes or TAEWindow is empty : ALL events will be rejected !!"
                << endmsg;
    if ( m_log == Log_t::Or && boolean_combine( Log_t::And, m_bxs.empty(), m_trs.empty(), m_ros.empty(),
                                                m_winmin >= m_winmax, m_cls.empty() ) )
      warning() << "BXTypes, TriggerTypes, ReadoutTypes and TAEWindow are empties : ALL events will be rejected !!"
                << endmsg;
    if ( boolean_combine( m_log, m_bxs.value().front() == odin_bx_type_t::All(),
                          m_cls.value().front() == odin_calibration_type_t::All(),
                          m_trs.value().front() == odin_trigger_type_t::All(),
                          m_ros.value().front() == odin_readout_type_t::All(), m_winmin<0, m_winmax> 7 ) )
      warning() << "OdinTypesFilter has no effect : ALL events will be accepted !!" << endmsg;
  } );
}

//=============================================================================
// Main execution
//=============================================================================
bool OdinTypesFilter::operator()( const LHCb::ODIN& odin ) const {

  // treat trivial requests
  if ( boolean_combine( m_log, m_bxs.value().front() == odin_bx_type_t::All(),
                        m_cls.value().front() == odin_calibration_type_t::All(),
                        m_trs.value().front() == odin_trigger_type_t::All(),
                        m_ros.value().front() == odin_readout_type_t::All(), m_winmin < 0 && m_winmax > 7 ) ) {
    m_acc += true;
    return true;
  }

  if ( boolean_combine( m_log,
                        ( m_bxs.empty(), m_cls.empty(), m_trs.empty(), m_ros.empty(), m_winmin >= m_winmax ) ) ) {
    m_acc += false;
    return false;
  }

  if ( msgLevel( MSG::DEBUG ) )
    debug() << " Trigger Type : " << odin.triggerType() << " BXType : " << odin.bunchCrossingType() << endmsg;

  auto equals_ = []( auto val ) { return [=]( const auto& i ) { return val == i; }; };
  bool clPass  = std::any_of( m_cls.begin(), m_cls.end(), equals_( odin.calibrationType() ) );
  bool bxPass  = std::any_of( m_bxs.begin(), m_bxs.end(), equals_( odin.bunchCrossingType() ) );
  bool trPass  = std::any_of( m_trs.begin(), m_trs.end(), equals_( odin.triggerType() ) );
  bool roPass  = std::any_of( m_ros.begin(), m_ros.end(), equals_( odin.readoutType() ) );
  bool taePass = ( (int)odin.timeAlignmentEventWindow() > m_winmin && (int)odin.timeAlignmentEventWindow() < m_winmax );

  auto filterPassed = boolean_combine( m_log, trPass, bxPass, roPass, taePass, clPass );

  m_acc += filterPassed;
  return filterPassed;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode OdinTypesFilter::finalize() {

  info() << "Accepted BXTypes : " << m_bxs.value() << endmsg << toString( m_log.value() ) << endmsg
         << "Accepted TriggerTypes : " << m_trs.value() << endmsg << toString( m_log.value() ) << endmsg
         << "Accepted CalibrationTypes : " << m_cls.value() << endmsg << toString( m_log.value() ) << endmsg
         << "Accepted ReadoutTypes : " << m_ros.value() << endmsg << toString( m_log.value() ) << endmsg
         << "TAE Window in [" << m_winmin.value() << "," << m_winmax.value() << "]" << endmsg << "   ---> "
         << m_acc.nTrueEntries() << " accepted events among " << m_acc.nEntries() << endmsg;

  return FilterPredicate::finalize(); // must be called after all other actions
}

//=============================================================================
