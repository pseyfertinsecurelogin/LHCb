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
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include <ostream>
#include <utility>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  inline constexpr CLID CLID_WeightsVector = 10031;

  // Namespace for locations in TDS
  namespace WeightsVectorLocation {
    inline const std::string Default = "Rec/Vertex/Weights";
  }

  /** @class WeightsVector WeightsVector.h
   *
   * Temporary class for storing primary vertex track weights, pending redesign
   * of RecVertex
   *
   * @author Marco Cattaneo
   *
   */

  class WeightsVector final : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of WeightsVector
    using Container = KeyedContainer<WeightsVector, Containers::HashMap>;

    /// Type for storage of weight data
    using WeightData = std::pair<int, float>;
    /// Vector of weight data
    using WeightDataVector = std::vector<WeightData>;

    /// Copy constructor. Creates a new WeightsVector with the same information
    WeightsVector( const LHCb::WeightsVector& wvec ) : KeyedObject<int>(), m_weights( wvec.weights() ) {}
    WeightsVector& operator=( const LHCb::WeightsVector& ) = delete;

    /// Default Constructor
    WeightsVector() = default;

    // Retrieve pointer to class definition structure
    [[nodiscard]] const CLID& clID() const override { return LHCb::WeightsVector::classID(); }
    static const CLID&        classID() { return CLID_WeightsVector; }

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  List of track keys with weight
    [[nodiscard]] const std::vector<std::pair<int, float>>& weights() const { return m_weights; }

    /// Update  List of track keys with weight
    void setWeights( const std::vector<std::pair<int, float>>& value ) { m_weights = value; }

    friend std::ostream& operator<<( std::ostream& str, const WeightsVector& obj ) { return obj.fillStream( str ); }

  private:
    std::vector<std::pair<int, float>> m_weights; ///< List of track keys with weight

  }; // class WeightsVector

  /// Definition of Keyed Container for WeightsVector
  typedef KeyedContainer<WeightsVector, Containers::HashMap> WeightsVectors;

  inline std::ostream& WeightsVector::fillStream( std::ostream& s ) const {
    s << "{ "
      << "weights :	" << m_weights << std::endl
      << " }";
    return s;
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------
