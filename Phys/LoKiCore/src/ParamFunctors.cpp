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
#include <regex>
#include <utility>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IAlgContextSvc.h"
#include "GaudiKernel/IAlgorithm.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/SmartIF.h"
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GetAlg.h"
// ============================================================================
// local
// ============================================================================
#include "LoKi/ILoKiSvc.h"
#include "LoKi/Interface.h"
#include "LoKi/Param.h"
#include "LoKi/ParamFunctors.h"
// ============================================================================
/** @file
 *  Implementation file for class LoKi::Param
 *  @see LoKi::Param
 *
 *  @date 2014-02-02
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 */
// ============================================================================
namespace {
  typedef std::map<std::string, double> MAP_D;
  typedef std::map<std::string, float>  MAP_F;
  typedef std::map<std::string, int>    MAP_I;
  //
  using DPROP = SimpleProperty<double>;
  using MPROP = SimpleProperty<MAP_D>;
  //
  class MyAlgSel : public Gaudi::Utils::AlgSelector {
  public:
    // ========================================================================
    MyAlgSel( std::string name, std::string prop, std::string key )
        : m_name( std::move( name ) ), m_prop( std::move( prop ) ), m_key( std::move( key ) ) {}
    // ========================================================================
  public:
    // ========================================================================
    /** the only one essential method:
     *  Seek for algorithm that
     *    - matched the name pattern
     *    - has property "Property"
     *    - the property should be convertible either to
     *        simple double or map { string: double}
     *    - for map-case, the presence of the
     *       corresponding key in map is required.
     */
    bool operator()( const IAlgorithm* a ) const override {
      //
      if ( !a ) { return false; }
      //
      if ( !m_name.empty() ) {
        std::regex pattern( m_name );
        if ( !std::regex_match( a->name(), pattern ) ) { return false; }
      }
      //
      const Property* p = Gaudi::Utils::getProperty( a, m_prop );
      if ( !p ) { return false; }
      //
      DPROP dp;
      MPROP mp;
      //
      bool db = false;
      bool mb = false;
      //
      try {
        db = dp.assign( *p );
      } catch ( ... ) { db = false; }
      try {
        mb = mp.assign( *p );
      } catch ( ... ) { mb = false; }
      //
      if ( !db && !mb ) {
        return false;
      } else if ( db && m_key.empty() ) {
        return true;
      } // TRUE
      else if ( mb ) {
        const MAP_D& m = mp.value();
        if ( m.end() != m.find( m_key ) ) { return true; } // TRUE
      }
      // finally: false
      return false; // finally: false
    }

  private:
    // ========================================================================
    std::string m_name;
    std::string m_prop;
    std::string m_key;
    // ========================================================================
  };
  // ==========================================================================
  const Property* getProperty( const SmartIF<IAlgContextSvc>& cntx, const LoKi::Param& param ) {
    //
    if ( !cntx ) { return nullptr; }
    //
    MyAlgSel sel( param.algorithm(), param.property(), param.key() );

    IAlgorithm* ialg = Gaudi::Utils::getAlgorithm( cntx, sel );
    if ( !ialg ) { return nullptr; }
    //
    return Gaudi::Utils::getProperty( ialg, param.property() );
  }
  // ==========================================================================
  const Property* getProperty( const LoKi::ILoKiSvc* svc, const LoKi::Param& param ) {
    //
    if ( !svc ) { return nullptr; }
    //
    auto* svc0 = const_cast<LoKi::ILoKiSvc*>( svc );
    // get the context service:
    SmartIF<IAlgContextSvc> cntx( svc0 );
    //
    return getProperty( cntx, param );
  }
  // ==========================================================================
  const Property* getProperty( const LoKi::AuxFunBase& base, const LoKi::Param& param ) {
    return getProperty( base.lokiSvc(), param );
  }
  // ==========================================================================
} // namespace
// ============================================================================
/*  Standard constructor from property
 *  @param property   the property
 */
// ============================================================================
LoKi::Parameters::ParamBase::ParamBase( const Property& property )
    : LoKi::AuxFunBase(), m_param(), m_property( &property ) {}
// ============================================================================
/*  constructor from Param-object
 *  @param property the property description
 */
// ============================================================================
LoKi::Parameters::ParamBase::ParamBase( LoKi::Param property )
    : LoKi::AuxFunBase(), m_param( std::move( property ) ), m_property( nullptr ) {
  if ( gaudi() ) { getProp(); }
}
// ============================================================================
void LoKi::Parameters::ParamBase::getProp() const {
  m_property = ::getProperty( *this, m_param );
  Assert( nullptr != m_property, "Unable to get property" );
}

// ============================================================================
// constructor from parameter
// ============================================================================
LoKi::Parameters::Parameter::Parameter( const LoKi::Param& param )
    : LoKi::AuxFunBase( std::tie( param ) ), LoKi::Parameters::ParamBase( param ) {
  if ( gaudi() ) { getProp(); }
}
// ============================================================================
// constructor from parameter
// ============================================================================
LoKi::Parameters::Parameter::Parameter( const std::string& param )
    : LoKi::AuxFunBase( std::tie( param ) ), LoKi::Parameters::ParamBase( param ) {
  if ( gaudi() ) { getProp(); }
}
// ============================================================================
// clone-method: "virtual constructor"
// ============================================================================
LoKi::Parameters::Parameter* LoKi::Parameters::Parameter::clone() const {
  return new LoKi::Parameters::Parameter( *this );
}
// ============================================================================
void LoKi::Parameters::Parameter::getParams() const {
  const Property* p = property();
  if ( !p ) { getProp(); }
  //
  if ( dynamic_cast<const PropertyWithValue<double>*>( p ) ) {
    m_prop = prop_t::scalar_d;
  } else if ( dynamic_cast<const PropertyWithValue<float>*>( p ) ) {
    m_prop = prop_t::scalar_f;
  } else if ( dynamic_cast<const PropertyWithValue<int>*>( p ) ) {
    m_prop = prop_t::scalar_i;
  } else if ( dynamic_cast<const PropertyWithValue<MAP_D>*>( p ) ) {
    m_prop = prop_t::map_d;
  } else if ( dynamic_cast<const PropertyWithValue<MAP_F>*>( p ) ) {
    m_prop = prop_t::map_f;
  } else if ( dynamic_cast<const PropertyWithValue<MAP_I>*>( p ) ) {
    m_prop = prop_t::map_i;
  } else {
    m_prop = prop_t::unknown;
  }
  //
  Assert( m_prop != prop_t::unknown, "Invalid property type" );
}
// ============================================================================
// optional: nice printout
// ============================================================================
std::ostream& LoKi::Parameters::Parameter::fillStream( std::ostream& s ) const { return s << param(); }
// ============================================================================
// the major method
// ============================================================================
namespace {
  // ==========================================================================
  template <class TYPE>
  TYPE get( const Property* prop ) {
    const auto* p = static_cast<const PropertyWithValue<TYPE>*>( prop );
    return p->value();
  }
  // ==========================================================================
} // namespace
// ============================================================================
double LoKi::Parameters::Parameter::operator()() const {
  //
  switch ( m_prop ) {
  case prop_t::scalar_d:
    return get<double>( property() );
  case prop_t::scalar_f:
    return get<float>( property() );
  case prop_t::scalar_i:
    return get<int>( property() );
  case prop_t::map_d: {
    const auto* p  = dynamic_cast<const PropertyWithValue<MAP_D>*>( property() );
    const auto& m  = p->value();
    auto        it = m.find( param().key() );
    Assert( m.end() != it, "No proper key is found" );
    return it->second;
  }
  case prop_t::map_f: {
    const auto* p  = dynamic_cast<const PropertyWithValue<MAP_F>*>( property() );
    const auto& m  = p->value();
    auto        it = m.find( param().key() );
    Assert( m.end() != it, "No proper key is found" );
    return it->second;
  }
  case prop_t::map_i: {
    const auto* p  = dynamic_cast<const PropertyWithValue<MAP_I>*>( property() );
    const auto& m  = p->value();
    auto        it = m.find( param().key() );
    Assert( m.end() != it, "No proper key is found" );
    return it->second;
  }
  default: {
    Assert( false, "Invalid type of parameter" );
    return 0;
  }
  }
  //
}
// ============================================================================
// The END
// ============================================================================
