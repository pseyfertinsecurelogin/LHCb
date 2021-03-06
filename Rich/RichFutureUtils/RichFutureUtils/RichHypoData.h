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

//------------------------------------------------------------------------
/** @file RichHypoData.h
 *
 *  Header file for utility class : Rich::Future::HypoData
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2003-07-31
 */
//------------------------------------------------------------------------

#pragma once

// STL
#include <ostream>
#include <type_traits>
#include <vector>

// Gaudi
#include "GaudiKernel/Kernel.h"

// LHCbKernel
#include "Kernel/RichParticleIDType.h"

// local
#include "RichUtils/RichSIMDTypes.h"
#include "RichUtils/StlArray.h"

// Vc
#include <Vc/common/alignedbase.h>

namespace Rich::Future {

  //------------------------------------------------------------------------
  /** @class HypoData RichHypoData.h RichFutureUtils/RichHypoData.h
   *
   *  A utility class providing an efficient fixed sized array
   *  for particle hypothesis data
   *
   *  Type traits are used to only support arithmetic or pointer types.
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2003-07-31
   */
  //------------------------------------------------------------------------

  template <typename TYPE>
  class HypoData : public Vc::AlignedBase<Vc::VectorAlignment> {

  public:
    // definitions

    /// Data Type
    using Type = TYPE;

    /// Definition of internal data array type
    using DataArray = Rich::ParticleArray<TYPE>;

    /// Type for vector
    using Vector = Rich::SIMD::STDVector<HypoData>;

  public:
    // constructors and destructors

    /// Default Constructor
    HypoData() = default;

    /** Constructor with explicit data initialisation value
     *
     *  @param value The data initialisation value for each mass hypothesis
     */
    explicit HypoData( const TYPE value ) { resetData( value ); }

    /// 'Copy' constructor from another templated type
    template <typename OTYPE>
    explicit HypoData( const HypoData<OTYPE>& data ) {
      std::size_t i = 0;
      GAUDI_LOOP_UNROLL( Rich::NParticleTypes )
      for ( const auto& d : data.dataArray() ) { m_data[i++] = TYPE( d ); }
    }

  public:
    // methods

    /** Read/Write access operator
     *
     *  @param type  The mass hypothesis for which the data is requested
     *  @return The data value
     */
    inline TYPE& operator[]( const Rich::ParticleIDType type ) noexcept { return m_data[type]; }

    /** Read access operator
     *
     *  @param type  The mass hypothesis for which the data is requested
     *  @return The data value
     */
    inline const TYPE& operator[]( const Rich::ParticleIDType type ) const noexcept { return m_data[type]; }

    /** Set the data value for a given particle hypothesis
     *
     *  @param type  The mass hypothesis for which the data is for
     *  @param value The data value
     */
    inline void setData( const Rich::ParticleIDType type, const TYPE value ) noexcept { m_data[type] = value; }

    /** Reset the data for all mass hypotheses. Following this call all data
     *  fields will be flagged as invalid (i.e. unset)
     *
     *  @param value The reset value
     */
    inline void resetData( const TYPE value ) noexcept { m_data.fill( value ); }

    /** Reset the data for all mass hypotheses. Following this call all data
     *  fields will be flagged as invalid (i.e. unset)
     *
     *  @attention The data values themselves are unaffected
     */
    inline void resetData() noexcept {
      if constexpr ( std::is_pointer_v<TYPE> ) {
        resetData( nullptr );
      } else {
        resetData( TYPE( 0 ) );
      }
    }

    /** Reset data for given particle hypothesis. Following this call the
     *  data for the given mas hypothesis will be flagged as invalid (i.e. unset)
     *
     *  @param type  The mass hypothesis to reset
     *  @param value The reset value
     */
    inline void resetData( const Rich::ParticleIDType type, const TYPE value ) noexcept { m_data[type] = value; }

    /** Reset data for given particle hypothesis. Following this call the
     *  data for the given mas hypothesis will be flagged as invalid (i.e. unset)
     *
     *  This implementation is for arithemtic types, and sets the value to 0
     *
     *  @param type  The mass hypothesis to reset
     */
    inline void resetData( const Rich::ParticleIDType type ) noexcept {
      if constexpr ( std::is_pointer_v<TYPE> ) {
        resetData( type, nullptr );
      } else {
        resetData( type, TYPE( 0 ) );
      }
    }

    /** Const Accessor to data array
     *
     *  @return Const reference to the internal data array
     */
    inline const DataArray& dataArray() const& noexcept { return m_data; }

  public:
    // messaging

    /// Implement textual ostream << method
    friend inline std::ostream& operator<<( std::ostream& os, const HypoData<TYPE>& data ) {
      os << "[ ";
      for ( const auto id : Rich::particles() ) { os << data[id] << " "; }
      return os << "]";
    }

  public:
    // mathematical operations (only for non-pointer types)

    // in place operations

    /// multiple all hypotheses by given value
    inline std::enable_if_t<!std::is_pointer_v<TYPE>, HypoData<TYPE>&> //
    operator*=( const TYPE value ) noexcept {
      GAUDI_LOOP_UNROLL( Rich::NParticleTypes )
      for ( auto& d : m_data ) { d *= value; }
      return *this;
    }

    /// shift all hypotheses by given value
    inline std::enable_if_t<!std::is_pointer_v<TYPE>, HypoData<TYPE>&> //
    operator+=( const TYPE value ) noexcept {
      GAUDI_LOOP_UNROLL( Rich::NParticleTypes )
      for ( auto& d : m_data ) { d += value; }
      return *this;
    }

    /// multiple all hypotheses by given hypo data
    inline std::enable_if_t<!std::is_pointer_v<TYPE>, HypoData<TYPE>&> //
    operator*=( const HypoData<TYPE>& data ) noexcept {
      std::size_t i = 0;
      GAUDI_LOOP_UNROLL( Rich::NParticleTypes )
      for ( const auto& d : data.dataArray() ) { m_data[i++] *= d; }
      return *this;
    }

    /// shift all hypotheses by given hypo data
    inline std::enable_if_t<!std::is_pointer_v<TYPE>, HypoData<TYPE>&> //
    operator+=( const HypoData<TYPE>& data ) noexcept {
      std::size_t i = 0;
      GAUDI_LOOP_UNROLL( Rich::NParticleTypes )
      for ( const auto& d : data.dataArray() ) { m_data[i++] += d; }
      return *this;
    }

    // create new arrays

    /// multiple all hypotheses by given value
    inline std::enable_if_t<!std::is_pointer_v<TYPE>, HypoData<TYPE>> //
    operator*( const TYPE value ) const noexcept {
      HypoData<TYPE> ret = *this;
      ret *= value;
      return ret;
    }

    /// shift all hypotheses by given value
    inline std::enable_if_t<!std::is_pointer_v<TYPE>, HypoData<TYPE>> //
    operator+( const TYPE value ) const noexcept {
      HypoData<TYPE> ret = *this;
      ret += value;
      return ret;
    }

    /// multiple all hypotheses by given value
    inline std::enable_if_t<!std::is_pointer_v<TYPE>, HypoData<TYPE>> //
    operator*( const HypoData<TYPE>& data ) const noexcept {
      HypoData<TYPE> ret = *this;
      ret *= data;
      return ret;
    }

    /// shift all hypotheses by given value
    inline std::enable_if_t<!std::is_pointer_v<TYPE>, HypoData<TYPE>> //
    operator+( const HypoData<TYPE>& data ) const noexcept {
      HypoData<TYPE> ret = *this;
      ret += data;
      return ret;
    }

  private:
    // data

    /// The internal representation of the data
    DataArray m_data = {{}};
  };

} // namespace Rich::Future
