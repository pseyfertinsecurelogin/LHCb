/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

// Include files
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/VectorMap.h"
#include <algorithm>
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_LumiMethods = 13505;

  /** @class LumiMethods LumiMethods.h
   *
   * Enum class to flag Lumi methods
   *
   * @author Jaap Panman
   *
   */

  class LumiMethods final : public DataObject {
  public:
    /// lookup table for method keys
    enum methodKey {
      NoMethod       = 0,  // Method undefined
      L0RateMethod   = 1,  // L0Rate Method
      RandomMethod   = 2,  // Random Method
      CorrectionFlag = 19, // Correction Method Flag
      PoissonOffset  = 50, // Offset for values over threshold
      Unknown        = 98, // unknown value
      LastGlobal     = 99  // User variables should use flags greater than this value
    };

    /// Default Constructor
    LumiMethods() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// conversion of string to enum for type methodKey
    static LHCb::LumiMethods::methodKey methodKeyToType( const std::string& aName );

    /// conversion to string for enum type methodKey
    static const std::string& methodKeyToString( int aEnum );

    /// intelligent printout
    std::ostream& fillStream( std::ostream& s ) const override;

    friend std::ostream& operator<<( std::ostream& str, const LumiMethods& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    static const GaudiUtils::VectorMap<std::string, methodKey>& s_methodKeyTypMap();

  }; // class LumiMethods

  inline std::ostream& operator<<( std::ostream& s, LHCb::LumiMethods::methodKey e ) {
    switch ( e ) {
    case LHCb::LumiMethods::NoMethod:
      return s << "NoMethod";
    case LHCb::LumiMethods::L0RateMethod:
      return s << "L0RateMethod";
    case LHCb::LumiMethods::RandomMethod:
      return s << "RandomMethod";
    case LHCb::LumiMethods::CorrectionFlag:
      return s << "CorrectionFlag";
    case LHCb::LumiMethods::PoissonOffset:
      return s << "PoissonOffset";
    case LHCb::LumiMethods::Unknown:
      return s << "Unknown";
    case LHCb::LumiMethods::LastGlobal:
      return s << "LastGlobal";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::LumiMethods::methodKey";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::LumiMethods::clID() const { return LHCb::LumiMethods::classID(); }

inline const CLID& LHCb::LumiMethods::classID() { return CLID_LumiMethods; }

inline const GaudiUtils::VectorMap<std::string, LHCb::LumiMethods::methodKey>& LHCb::LumiMethods::s_methodKeyTypMap() {
  static const GaudiUtils::VectorMap<std::string, methodKey> m = {
      {"NoMethod", NoMethod},           {"L0RateMethod", L0RateMethod},
      {"RandomMethod", RandomMethod},   {"CorrectionFlag", CorrectionFlag},
      {"PoissonOffset", PoissonOffset}, {"Unknown", Unknown},
      {"LastGlobal", LastGlobal}};
  return m;
}

inline LHCb::LumiMethods::methodKey LHCb::LumiMethods::methodKeyToType( const std::string& aName ) {
  auto iter = s_methodKeyTypMap().find( aName );
  return iter != s_methodKeyTypMap().end() ? iter->second : Unknown;
}

inline const std::string& LHCb::LumiMethods::methodKeyToString( int aEnum ) {
  static const std::string s_Unknown = "Unknown";
  auto                     iter      = std::find_if( s_methodKeyTypMap().begin(), s_methodKeyTypMap().end(),
                            [&]( const std::pair<const std::string, methodKey>& i ) { return i.second == aEnum; } );
  return iter != s_methodKeyTypMap().end() ? iter->first : s_Unknown;
}

inline std::ostream& LHCb::LumiMethods::fillStream( std::ostream& s ) const {

  std::ostream& ss = const_cast<std::ostream&>( s );
  ss << "{ }";

  return ss;
}
