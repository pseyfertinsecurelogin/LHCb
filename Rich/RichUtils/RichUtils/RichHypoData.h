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
 *  Header file for utility class : Rich::HypoData
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2003-07-31
 */
//------------------------------------------------------------------------

#pragma once

// STL
#include <ostream>
#include <type_traits>

// LHCbKernel
#include "Kernel/MemPoolAlloc.h"
#include "Kernel/RichParticleIDType.h"

// local
#include "RichUtils/StlArray.h"

namespace Rich {

  //------------------------------------------------------------------------
  /** @class HypoData RichHypoData.h RichUtils/RichHypoData.h
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

  template <typename TYPE,
            typename = typename std::enable_if<std::is_arithmetic<TYPE>::value || std::is_pointer<TYPE>::value>::type>
  class HypoData : public LHCb::MemPoolAlloc<HypoData<TYPE>> {

  public: // definitions
    /// Definition of internal data array type
    using DataArray = std::array<TYPE, Rich::NParticleTypes>;
    /// Definition of internal validity array type
    using ValidityArray = std::array<bool, Rich::NParticleTypes>;

  public: // constructors and destructors
    /// Default Constructor
    HypoData() = default;

    /** Constructor with explicit data initialisation value
     *
     *  @param value The data initialisation value for each mass hypothesis
     */
    explicit HypoData( const TYPE value ) { resetData( value ); }

  public: // methods
    /** Read access operator
     *
     *  @param type  The mass hypothesis for which the data is requested
     *  @return The data value
     */
    inline const TYPE& operator[]( const Rich::ParticleIDType type ) const { return m_data[type]; }

    /** Set the data value for a given particle hypothesis
     *
     *  @param type  The mass hypothesis for which the data is for
     *  @param value The data value
     */
    inline void setData( const Rich::ParticleIDType type, const TYPE value ) noexcept {
      m_valid[type] = true;
      m_data[type]  = value;
    }

    /** Reset the data for all mass hypotheses. Following this call all data
     *  fields will be flagged as invalid (i.e. unset)
     *
     *  @param value The reset value
     */
    inline void resetData( const TYPE value ) noexcept {
      m_valid.fill( false );
      m_data.fill( value );
    }

    /** Reset the data for all mass hypotheses. Following this call all data
     *  fields will be flagged as invalid (i.e. unset)
     *
     *  @attention The data values themselves are unaffected
     */
    inline void resetData() noexcept {
      if constexpr ( std::is_arithmetic<TYPE>::value ) {
        resetData( 0 );
      } else if constexpr ( std::is_pointer<TYPE>::value ) {
        resetData( nullptr );
      }
    }

    /** Reset data for given particle hypothesis. Following this call the
     *  data for the given mas hypothesis will be flagged as invalid (i.e. unset)
     *
     *  @param type  The mass hypothesis to reset
     *  @param value The reset value
     */
    inline void resetData( const Rich::ParticleIDType type, const TYPE value ) noexcept {
      m_valid[type] = false;
      m_data[type]  = value;
    }

    /** Reset data for given particle hypothesis. Following this call the
     *  data for the given mas hypothesis will be flagged as invalid (i.e. unset)
     *
     *  @param type  The mass hypothesis to reset
     */
    inline void resetData( const Rich::ParticleIDType type ) noexcept {
      if constexpr ( std::is_arithmetic<TYPE>::value ) {
        resetData( type, 0 );
      } else if constexpr ( std::is_pointer<TYPE>::value ) {
        resetData( type, nullptr );
      }
    }

    /** Const Accessor to data array
     *
     *  @return Const reference to the internal data array
     */
    inline const DataArray& dataArray() const& noexcept { return m_data; }

    /** Check whether a piece of data has been initialised
     *
     *  @param type The mass hypothesis to test
     *
     *  @return boolean indicating the status of the data
     *  @retval true  Data field has been explicitly set
     *  @retval false Data field has not been set.
     *                Value will be the initialisation (or reset) value
     */
    inline bool dataIsValid( const Rich::ParticleIDType type ) const noexcept { return m_valid[type]; }

  public:
    /// Implement textual ostream << method
    friend inline std::ostream& operator<<( std::ostream& os, const HypoData<TYPE>& data ) {
      os << "[ ";
      for ( const auto id : Rich::particles() ) { os << data[id] << " "; }
      return os << "]";
    }

  private: // data
    /// The internal representation of the data
    DataArray m_data = {{}};

    /// The validity flags
    ValidityArray m_valid = {{}};
  };

} // namespace Rich
