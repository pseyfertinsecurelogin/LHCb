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
#ifndef LOKI_TOCPP_H
#define LOKI_TOCPP_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <array>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
// ============================================================================
// boost
// ============================================================================
#include "boost/algorithm/string/join.hpp"
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/System.h"
#include "GaudiKernel/ToStream.h"
// ============================================================================
// Local
// ============================================================================
#include "LoKi/KinTypes.h"
// ============================================================================
/** @file LoKi/ToCpp.h
 *  set of utilities used fro autogeneration of C++ code for LoKi-functors
 *  @see LoKi::AuxFunBase
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2015-03-30
 *
 */
// ============================================================================
// Clang specific cling JIT issue.. see
// https://gitlab.cern.ch/lhcb/LHCb/merge_requests/2258
// IncrementalExecutor::executeFunction: symbol
// '_ZNSt5arrayINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEELm4EED1Ev'
// unresolved while linking function '_GLOBAL__sub_I_cling_module_394'!
// You are probably missing the definition of
// std::array<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, 4ul>::~array()
// Maybe you need to load the corresponding shared library?
// ============================================================================
#if defined( __clang__ )
namespace {
  static std::array<std::string, 4ul> sa_4;
}
#endif
// ============================================================================
// Forward declarations
// ============================================================================
namespace LoKi {
  class AuxFunBase;
  class Dump;
  class FirstN;
  class Sort;
  class Param;
  class Histo;
  class CounterDef;
  //
  class GAUDI_API StrKeep {
  public:
    StrKeep( std::string data );
    [[nodiscard]] const std::string& data() const { return m_data; }

  private:
    std::string m_data;
  };
} // namespace LoKi
// ============================================================================
namespace LHCb {
  class ParticleID;
  class LHCbID;
} // namespace LHCb
// ============================================================================
namespace Gaudi {
  class StringKey;
  class Time;
  class TimeSpan;
  class Histo1DDef;
} // namespace Gaudi
// ============================================================================
namespace Decays {
  class iNode;
}
// ============================================================================
namespace GaudiAlg {
  class ID;
}
// ============================================================================
struct IDVAlgorithm;
class GaudiAlgorithm;
// ============================================================================
namespace Gaudi {
  namespace Utils {
    // ========================================================================
    GAUDI_API std::string toCpp( const LoKi::AuxFunBase& o );
    GAUDI_API std::string toCpp( const LoKi::CounterDef& o );
    GAUDI_API std::string toCpp( const LoKi::Dump& o );
    GAUDI_API std::string toCpp( const LoKi::FirstN& o );
    GAUDI_API std::string toCpp( const LoKi::Histo& o );
    GAUDI_API std::string toCpp( const LoKi::LorentzVector& o );
    GAUDI_API std::string toCpp( const LoKi::Point3D& o );
    GAUDI_API std::string toCpp( const LoKi::Param& o );
    GAUDI_API std::string toCpp( const LoKi::Sort& o );
    GAUDI_API std::string toCpp( const LoKi::StrKeep& o );
    GAUDI_API std::string toCpp( const LoKi::ThreeVector& o );
    //
    GAUDI_API std::string toCpp( const LHCb::LHCbID& o );
    GAUDI_API std::string toCpp( const LHCb::ParticleID& o );
    //
    GAUDI_API std::string toCpp( const Gaudi::Time& o );
    GAUDI_API std::string toCpp( const Gaudi::TimeSpan& o );
    GAUDI_API std::string toCpp( const Gaudi::StringKey& o );
    GAUDI_API std::string toCpp( const Gaudi::Histo1DDef& o );
    //
    GAUDI_API std::string toCpp( const GaudiAlg::ID& o );
    // ========================================================================
    GAUDI_API std::string toCpp( const Decays::iNode& o );
    // ========================================================================
    GAUDI_API std::string toCpp( const IDVAlgorithm* /* a */ );
    GAUDI_API std::string toCpp( const GaudiAlgorithm* /* a */ );
    // ========================================================================
  } // namespace Utils
} // namespace Gaudi
// ============================================================================
namespace Gaudi {
  namespace Utils {
    // ========================================================================
    // strings and chars
    // ========================================================================
    inline std::string toCpp( const char s ) { return "'" + std::string( 1, s ) + "'"; }
    inline std::string toCpp( const std::string& s ) { return '"' + s + '"'; }
    template <unsigned N>
    inline std::string toCpp( const char ( &s )[N] ) {
      return toCpp( std::string( s, s + N ) );
    }
    // ========================================================================
    inline std::string toCpp( const short o ) { return toString( o ); }
    inline std::string toCpp( const unsigned short o ) { return toString( o ); }
    inline std::string toCpp( const int o ) { return toString( o ); }
    inline std::string toCpp( const unsigned int o ) { return toString( o ); }
    inline std::string toCpp( const long o ) { return toString( o ); }
    inline std::string toCpp( const unsigned long o ) { return toString( o ); }
    // ========================================================================
    inline std::string toCpp( const long long o ) { return toString( o ); }
    inline std::string toCpp( const unsigned long long o ) { return toString( o ); }
    inline std::string toCpp( const bool o ) { return o ? "true" : "false"; }
    // ========================================================================
    //
    GAUDI_API
    std::string        toCpp( const long double o, const unsigned short p = 16 );
    inline std::string toCpp( const double o ) { return toCpp( o, 16 ); }
    inline std::string toCpp( const float o ) { return toCpp( o, 12 ); }
    // ========================================================================
    GAUDI_API
    std::string toCpp( const LoKi::AuxFunBase& o );
    // ========================================================================
    // declarations (1)
    // ========================================================================
    // std::vector
    template <class TYPE>
    inline std::string toCpp( const std::vector<TYPE>& v );
    // std::map
    template <class TYPE1, class TYPE2>
    inline std::string toCpp( const std::map<TYPE1, TYPE2>& v );
    // std::tuple
    template <typename... Args>
    inline std::string toCpp( const std::tuple<Args...>& t );
    // ========================================================================
    // implementations
    // ========================================================================
    template <class TYPE>
    inline std::string toCpp( const std::vector<TYPE>& v ) {
      std::string o = "std::vector<" + System::typeinfoName( typeid( TYPE ) ) + ">{";
      for ( const auto& i : v ) { o.append( toCpp( i ) + ", " ); }
      //
      return o + '}';
    }
    // ========================================================================
    template <class TYPE1, class TYPE2>
    inline std::string toCpp( const std::map<TYPE1, TYPE2>& v ) {
      std::string o =
          "std::map<" + System::typeinfoName( typeid( TYPE1 ) ) + "," + System::typeinfoName( typeid( TYPE2 ) ) + ">{";
      for ( const auto& i : v ) { o.append( "{" + toCpp( i.first ) + ", " + toCpp( i.second ) + "} ," ); }
      //
      return o + '}';
    }
    // ========================================================================
    namespace details {
      // ======================================================================
      /// recursive print of std::tuple
      // ======================================================================
      template <typename Tuple, std::size_t... I>
      std::string toCpp( const Tuple& t, std::index_sequence<I...> ) {
        std::array<std::string, sizeof...( I )> strs{Gaudi::Utils::toCpp( std::get<I>( t ) )...};
        return boost::algorithm::join( strs, " , " );
      }
    } // namespace details
    // ========================================================================
    /// std::tuple as comma-separated list - what we need for AuxFunBase
    template <typename... Args>
    inline std::string toCpp_lst( const std::tuple<Args...>& t ) {
      return details::toCpp( t, std::index_sequence_for<Args...>{} );
    }
    /// std::tuple, just for completness
    template <typename... Args>
    inline std::string toCpp( const std::tuple<Args...>& t ) {
      return System::typeinfoName( typeid( t ) ) + "{" + toCpp_lst( t ) + "}";
    }
    // ========================================================================
  } // namespace Utils
} // namespace Gaudi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_TOCPP_H
