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
#ifndef LUTFORFUNCTION_H
#define LUTFORFUNCTION_H 1

#include <algorithm>
#include <array>
#include <type_traits>
#include <utility>

/** @class LUTForFunction LUTForFunction.h Kernel/LUTForFunction.h
 *
 * A N-dimensional binned lookup table for functions
 *
 * @code
 * Example:
 *
 * Fill table
 * ----------
 * fillTable( [&](const std::array<float,2>& var) {
 *                  return ...;
 *            } );
 *
 * Retrieve information
 * --------------------
 * getValue( { 1.5, 4.1 } );
 * or
 * getInterpolatedValue( { 1.5, 4.1 } );
 * @endcode
 *
 *  @author Mariusz Witek
 *  @date   2006-09-25
 *  @update for A-Team framework 2007-08-20 SHM
 *
 */

namespace LUT {
  struct Range {
    float min, max;
  };

  namespace detail {
    template <typename T>
    inline constexpr bool is_range_v = std::is_same_v<T, Range>;
  } // namespace detail
} // namespace LUT

template <int... nBin>
class LUTForFunction final {
  using tag_t = std::make_index_sequence<sizeof...( nBin )>;

public:
  constexpr static int nVar = sizeof...( nBin );

  template <typename... R, typename = std::enable_if_t<sizeof...( R ) == nVar && ( ... && LUT::detail::is_range_v<R> )>>
  constexpr LUTForFunction( R... r )
      : m_minVar{r.min...}, m_maxVar{r.max...}, m_deltaVar{( r.max - r.min ) / nBin...} {}

  template <typename Fun>
  void fillTable( const Fun& f ) {
    std::array<int, nVar> idx{0};
    bool                  done = false;
    while ( !done ) {
      m_table[to_offset( idx )] = f( to_var( idx, tag_t{} ) );
      idx[0]++;
      for ( int i = 0; i < nVar - 1; i++ ) {
        if ( idx[i] > nBins( i ) ) {
          idx[i] = 0;
          ++idx[i + 1];
        } else {
          break;
        }
      }
      done = ( idx[nVar - 1] > nBins( nVar - 1 ) );
    }
  }

  float getValue( const std::array<float, nVar>& var ) const {
    return m_table[to_offset( to_idx( var, 0.5, tag_t{} ) )];
  }

  float getInterpolatedValue( const std::array<float, nVar>& var ) const {
    auto  idxVec    = to_idx( var, 0.000000001, tag_t{} );
    auto  vars      = to_var( idxVec, tag_t{} );
    float tabVal    = m_table[to_offset( idxVec )];
    float addTabVal = 0.0;
    for ( int i = 0; i < nVar; i++ ) {
      if ( var[i] < vars[i] ) continue;
      if ( idxVec[i] > nBins( i ) - 1 ) continue;
      if ( var[i] < m_minVar[i] || var[i] > m_maxVar[i] ) continue;
      idxVec[i]++;
      float dTab_dVar = ( m_table[to_offset( idxVec )] - tabVal ) / m_deltaVar[i];
      idxVec[i]--;
      float dVar = var[i] - vars[i];
      addTabVal += dTab_dVar * dVar;
    }
    tabVal += addTabVal;
    return tabVal;
  }

  // it would be better not to have to expose these details....
  constexpr static int nBins( int i ) noexcept { return std::array{nBin...}[i]; }
  constexpr float      min( int i ) const noexcept { return m_minVar[i]; }
  constexpr float      max( int i ) const noexcept { return m_maxVar[i]; }
  constexpr float      delta( int i ) const noexcept { return m_deltaVar[i]; }
  const auto&          table() const { return m_table; }

private:
  constexpr static int to_offset( const std::array<int, nVar>& idx ) {
    int location = idx[nVar - 1];
    for ( int i = nVar - 2; i >= 0; i-- ) location = location * ( nBins( i ) + 1 ) + idx[i];
    return location;
  }

  template <size_t... i>
  constexpr auto to_var( const std::array<int, nVar>& idx, std::index_sequence<i...> ) const {
    return std::array{( m_minVar[i] + idx[i] * m_deltaVar[i] )...};
  }

  template <size_t... i>
  constexpr auto to_idx( const std::array<float, nVar>& var, float offset, std::index_sequence<i...> ) const {
    return std::array{std::clamp( (int)( ( var[i] - m_minVar[i] ) / m_deltaVar[i] + offset ), 0, nBins( i ) )...};
  }

  const std::array<float, nVar> m_minVar;
  const std::array<float, nVar> m_maxVar;
  const std::array<float, nVar> m_deltaVar;

  std::array<float, ( ... * ( nBin + 1 ) )> m_table = {0};
};

#endif // LUTFORFUNCTION_H
