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
  static const CLID CLID_WeightsVector = 10031;

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
    typedef KeyedContainer<WeightsVector, Containers::HashMap> Container;

    /// Type for storage of weight data
    typedef std::pair<int, float> WeightData;
    /// Vector of weight data
    typedef std::vector<WeightData> WeightDataVector;

    /// Copy constructor. Creates a new WeightsVector with the same information
    WeightsVector( const LHCb::WeightsVector& wvec ) : KeyedObject<int>(), m_weights( wvec.weights() ) {}

    /// Default Constructor
    WeightsVector() : m_weights() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  List of track keys with weight
    const std::vector<std::pair<int, float>>& weights() const;

    /// Update  List of track keys with weight
    void setWeights( const std::vector<std::pair<int, float>>& value );

    friend std::ostream& operator<<( std::ostream& str, const WeightsVector& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    std::vector<std::pair<int, float>> m_weights; ///< List of track keys with weight

  }; // class WeightsVector

  /// Definition of Keyed Container for WeightsVector
  typedef KeyedContainer<WeightsVector, Containers::HashMap> WeightsVectors;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::WeightsVector::clID() const { return LHCb::WeightsVector::classID(); }

inline const CLID& LHCb::WeightsVector::classID() { return CLID_WeightsVector; }

inline std::ostream& LHCb::WeightsVector::fillStream( std::ostream& s ) const {
  s << "{ "
    << "weights :	" << m_weights << std::endl
    << " }";
  return s;
}

inline const std::vector<std::pair<int, float>>& LHCb::WeightsVector::weights() const { return m_weights; }

inline void LHCb::WeightsVector::setWeights( const std::vector<std::pair<int, float>>& value ) { m_weights = value; }
