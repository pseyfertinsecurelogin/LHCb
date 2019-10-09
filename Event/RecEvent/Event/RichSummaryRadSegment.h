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
#include "Event/RichSummaryPhoton.h"
#include "GaudiKernel/SerializeSTL.h"
#include "Kernel/RichParticleIDType.h"
#include "Kernel/RichRadiatorType.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  /** @class RichSummaryRadSegment RichSummaryRadSegment.h
   *
   * RICH reconstruction summary object representing a single trajectory through
   * a RICH radiator
   *
   * @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *
   */

  class RichSummaryRadSegment final {
  public:
    /// Vector of RichSummaryRadSegments
    typedef std::vector<LHCb::RichSummaryRadSegment> Vector;

    /// Default Constructor
    RichSummaryRadSegment()
        : m_bitPackedWord( 0 )
        , m_photons()
        , m_expectedCkTheta( Rich::NParticleTypes, 0 )
        , m_expectedCkThetaError( Rich::NParticleTypes, 0 )
        , m_expectedNumPhotons( Rich::NParticleTypes, 0 ) {}

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const;

    /// Returns the expected Cherenkov theta values for the given mass hypothesis
    float expectedCkTheta( const Rich::ParticleIDType type ) const;

    /// Sets the expected Cherenkov theta values for the given mass hypothesis
    void setExpectedCkTheta( const Rich::ParticleIDType type, const float ckTheta );

    /// Returns the expected error on the Cherenkov theta values for the given mass hypothesis
    float expectedCkThetaError( const Rich::ParticleIDType type ) const;

    /// Sets the expected error on the Cherenkov theta values for the given mass hypothesis
    void setExpectedCkThetaError( const Rich::ParticleIDType type, const float ckErr );

    /// Returns the expected number of observed signal Cherenkov photons for each mass hypothesis
    float expectedNumPhotons( const Rich::ParticleIDType type ) const;

    /// Sets the expected number of observed signal Cherenkov photons for each mass hypothesis
    void setExpectedNumPhotons( const Rich::ParticleIDType type, const float nPhots );

    /// Retrieve The radiator type
    Rich::RadiatorType radiator() const;

    /// Update The radiator type
    void setRadiator( const Rich::RadiatorType& value );

    /// Retrieve const  Vector of photons for this track segment
    const LHCb::RichSummaryPhoton::Vector& photons() const;

    /// Update  Vector of photons for this track segment
    void setPhotons( const LHCb::RichSummaryPhoton::Vector& value );

    friend std::ostream& operator<<( std::ostream& str, const RichSummaryRadSegment& obj ) {
      return obj.fillStream( str );
    }

  protected:
  private:
    /// Offsets of bitfield bitPackedWord
    enum bitPackedWordBits { radiatorBits = 0 };

    /// Bitmasks for bitfield bitPackedWord
    enum bitPackedWordMasks { radiatorMask = 0x7L };

    unsigned int                    m_bitPackedWord;   ///< Bit-packed information (radiator type etc.)
    LHCb::RichSummaryPhoton::Vector m_photons;         ///< Vector of photons for this track segment
    std::vector<float>              m_expectedCkTheta; ///< Expected Cherenkov theta values for each mass hypothesis
    std::vector<float> m_expectedCkThetaError;         ///< Expected error on the Cherenkov theta values for each mass
                                                       ///< hypothesis
    std::vector<float> m_expectedNumPhotons; ///< Expected number of observed signal Cherenkov photons for each mass
                                             ///< hypothesis

  }; // class RichSummaryRadSegment

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::RichSummaryRadSegment::fillStream( std::ostream& s ) const {
  s << "{ "
    << "bitPackedWord :	" << m_bitPackedWord << std::endl
    << "photons :	" << m_photons << std::endl
    << "expectedCkTheta :	" << m_expectedCkTheta << std::endl
    << "expectedCkThetaError :	" << m_expectedCkThetaError << std::endl
    << "expectedNumPhotons :	" << m_expectedNumPhotons << std::endl
    << " }";
  return s;
}

inline Rich::RadiatorType LHCb::RichSummaryRadSegment::radiator() const {
  return ( Rich::RadiatorType )( ( m_bitPackedWord & radiatorMask ) >> radiatorBits );
}

inline void LHCb::RichSummaryRadSegment::setRadiator( const Rich::RadiatorType& value ) {
  unsigned int val = (unsigned int)value;
  m_bitPackedWord &= ~radiatorMask;
  m_bitPackedWord |= ( ( ( (unsigned int)val ) << radiatorBits ) & radiatorMask );
}

inline const LHCb::RichSummaryPhoton::Vector& LHCb::RichSummaryRadSegment::photons() const { return m_photons; }

inline void LHCb::RichSummaryRadSegment::setPhotons( const LHCb::RichSummaryPhoton::Vector& value ) {
  m_photons = value;
}

inline float LHCb::RichSummaryRadSegment::expectedCkTheta( const Rich::ParticleIDType type ) const {

  return m_expectedCkTheta[type];
}

inline void LHCb::RichSummaryRadSegment::setExpectedCkTheta( const Rich::ParticleIDType type, const float ckTheta ) {

  m_expectedCkTheta[type] = ckTheta;
}

inline float LHCb::RichSummaryRadSegment::expectedCkThetaError( const Rich::ParticleIDType type ) const {

  return m_expectedCkThetaError[type];
}

inline void LHCb::RichSummaryRadSegment::setExpectedCkThetaError( const Rich::ParticleIDType type, const float ckErr ) {

  m_expectedCkThetaError[type] = ckErr;
}

inline float LHCb::RichSummaryRadSegment::expectedNumPhotons( const Rich::ParticleIDType type ) const {

  return m_expectedNumPhotons[type];
}

inline void LHCb::RichSummaryRadSegment::setExpectedNumPhotons( const Rich::ParticleIDType type, const float nPhots ) {

  m_expectedNumPhotons[type] = nPhots;
}
