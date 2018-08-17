
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

// LHCbKernel
#include "Kernel/RichParticleIDType.h"

// local
#include "RichUtils/StlArray.h"
#include "RichUtils/RichSIMDTypes.h"

// Vc
#include <Vc/common/alignedbase.h>

namespace Rich::Future
{

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

  template < typename TYPE >
  class HypoData : public Vc::AlignedBase<Vc::VectorAlignment>
  {

  public: // definitions

    /// Data Type
    using Type = TYPE;

    /// Definition of internal data array type
    using DataArray = Rich::ParticleArray<TYPE>;

    /// Type for vector
    using Vector = Rich::SIMD::STDVector<HypoData>;

  public: // constructors and destructors

    /// Default Constructor
    HypoData() = default;

    /** Constructor with explicit data initialisation value
     *
     *  @param value The data initialisation value for each mass hypothesis
     */
    explicit HypoData( const TYPE value ) { resetData(value); }

  public: // methods

    /** Read/Write access operator
     *
     *  @param type  The mass hypothesis for which the data is requested
     *  @return The data value
     */
    inline TYPE & operator[] ( const Rich::ParticleIDType type ) noexcept
    {
      return m_data[type];
    }

    /** Read access operator
     *
     *  @param type  The mass hypothesis for which the data is requested
     *  @return The data value
     */
    inline const TYPE & operator[] ( const Rich::ParticleIDType type ) const noexcept
    {
      return m_data[type];
    }

    /** Set the data value for a given particle hypothesis
     *
     *  @param type  The mass hypothesis for which the data is for
     *  @param value The data value
     */
    inline void setData( const Rich::ParticleIDType type, const TYPE value ) noexcept
    {
      m_data[type] = value;
    }

    /** Reset the data for all mass hypotheses. Following this call all data
     *  fields will be flagged as invalid (i.e. unset)
     *
     *  @param value The reset value
     */
    inline void resetData( const TYPE value ) noexcept
    {
      m_data.fill( value );
    }

    /** Reset the data for all mass hypotheses. Following this call all data
     *  fields will be flagged as invalid (i.e. unset)
     *
     *  @attention The data values themselves are unaffected
     */
    inline void resetData() noexcept 
    {
      if constexpr ( std::is_pointer<TYPE>::value )
      {
        resetData( nullptr );
      }
      else
      {
        resetData( TYPE(0) ); 
      }
    }

    /** Reset data for given particle hypothesis. Following this call the
     *  data for the given mas hypothesis will be flagged as invalid (i.e. unset)
     *
     *  @param type  The mass hypothesis to reset
     *  @param value The reset value
     */
    inline void resetData( const Rich::ParticleIDType type, const TYPE value ) noexcept
    {
      m_data[type] = value;
    }

    /** Reset data for given particle hypothesis. Following this call the
     *  data for the given mas hypothesis will be flagged as invalid (i.e. unset)
     *
     *  This implementation is for arithemtic types, and sets the value to 0
     *
     *  @param type  The mass hypothesis to reset
     */
    inline void resetData( const Rich::ParticleIDType type ) noexcept
    {
      if constexpr ( std::is_pointer<TYPE>::value )
      { 
        resetData( type, nullptr );  
      }
      else
      {
        resetData( type, TYPE(0) );
      }
    }

    /** Const Accessor to data array
     *
     *  @return Const reference to the internal data array
     */
    inline const DataArray & dataArray() const & noexcept
    {
      return m_data;
    }

  public:

    /// Implement textual ostream << method
    friend inline std::ostream& operator << ( std::ostream& os,
                                              const HypoData<TYPE>& data )
    {
      os << "[ ";
      for ( const auto id : Rich::particles() ) { os << data[id] << " "; }
      return os << "]";
    }

  private: // data

    /// The internal representation of the data
    DataArray m_data = {{}};

  };

}
