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

//-----------------------------------------------------------------------------
/** @file RichRadIntersection.h
 *
 *  Header file for RICH utility class : Rich::RadIntersection
 *
 *  @author Antonis Papanestis
 *  @date   2006-02-28
 */
//-----------------------------------------------------------------------------

#pragma once

// STL
#include <algorithm>
#include <cmath>
#include <functional>
#include <utility>
#include <vector>

// Include files
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// DetDesc
class DeRichRadiator;

namespace Rich {

  /** @class RadIntersection RichRadIntersection.h Kernel/RichRadIntersection.h
   *
   *  RICH Radiator intersection information.
   *
   *  Contains the entry and exit points of a single intersection with a radiator
   *  volume, together with the momentum vector at the intersection points, and
   *  a pointer to the intersected radiator volume.
   *
   *  @author Antonis Papanestis
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2006-02-28
   */

  class RadIntersection {

  public:
    /// Definition of a vector of intersections
    using Vector = std::vector<Rich::RadIntersection>;
    // using Vector = std::vector<Rich::RadIntersection>;

  public:
    /// Default constructor
    RadIntersection() = default;

    /// Default destructor
    ~RadIntersection() = default;

    /// Default Copy Constructor
    RadIntersection( const RadIntersection& ) = default;

    /// Default Copy Operator
    RadIntersection& operator=( const RadIntersection& ) = default;

    /// Default Move Constructor
    RadIntersection( RadIntersection&& ) = default;

    /// Default Move Operator
    RadIntersection& operator=( RadIntersection&& ) = default;

  public:
    /// Constructor from (copied) entry and exit points and momentum vectors, and a pointer to a
    /// DeRichRadiator
    RadIntersection( Gaudi::XYZPoint       entryPoint, ///< The radiator entry point
                     Gaudi::XYZVector      entryVect,  ///< The direction vector at the entry point
                     Gaudi::XYZPoint       exitPoint,  ///< The radiator exit point
                     Gaudi::XYZVector      exitVect,   ///< The direction vector at the exit point
                     const DeRichRadiator* rad         ///< Pointer to the intersected radiator volume
                     )
        : m_entryPoint( std::move( entryPoint ) )
        , m_entryVect( std::move( entryVect ) )
        , m_exitPoint( std::move( exitPoint ) )
        , m_exitVect( std::move( exitVect ) )
        , m_radiator( rad ) {}

    /// Constructor from (moved) entry and exit points and momentum vectors, and a pointer to a
    /// DeRichRadiator
    RadIntersection( Gaudi::XYZPoint&&     entryPoint, ///< The radiator entry point
                     Gaudi::XYZVector&&    entryVect,  ///< The direction vector at the entry point
                     Gaudi::XYZPoint&&     exitPoint,  ///< The radiator exit point
                     Gaudi::XYZVector&&    exitVect,   ///< The direction vector at the exit point
                     const DeRichRadiator* rad         ///< Pointer to the intersected radiator volume
                     )
        : m_entryPoint( std::move( entryPoint ) )
        , m_entryVect( std::move( entryVect ) )
        , m_exitPoint( std::move( exitPoint ) )
        , m_exitVect( std::move( exitVect ) )
        , m_radiator( rad ) {}

  public:
    /// Set the entry point to the radiator
    inline void setEntryPoint( const Gaudi::XYZPoint& entryPoint ) noexcept { m_entryPoint = entryPoint; }

    /// Get the entry point to the radiator
    [[nodiscard]] inline const Gaudi::XYZPoint& entryPoint() const noexcept { return m_entryPoint; }

    /// Set the entry momentum vector to the radiator
    inline void setEntryMomentum( const Gaudi::XYZVector& entryV ) noexcept { m_entryVect = entryV; }

    /// Get the entry point to the radiator
    [[nodiscard]] inline const Gaudi::XYZVector& entryMomentum() const noexcept { return m_entryVect; }

    /// Set the exit point of the radiator
    inline void setExitPoint( const Gaudi::XYZPoint& exitPoint ) noexcept { m_exitPoint = exitPoint; }

    /// Get the exit point to the radiator
    [[nodiscard]] inline const Gaudi::XYZPoint& exitPoint() const noexcept { return m_exitPoint; }

    /// Set the exit momentum vector to the radiator
    inline void setExitMomentum( const Gaudi::XYZVector& exitV ) noexcept { m_exitVect = exitV; }

    /// Get the entry point to the radiator
    [[nodiscard]] inline const Gaudi::XYZVector& exitMomentum() const noexcept { return m_exitVect; }

    /// Set pointer to associated radiator detector element
    inline void setRadiator( const DeRichRadiator* radiator ) noexcept { m_radiator = radiator; }

    /// Get pointer to associated radiator detector element
    [[nodiscard]] inline const DeRichRadiator* radiator() const noexcept { return m_radiator; }

    /// Returns the path length (squared) in the given radiator
    [[nodiscard]] inline double pathLength2() const { return ( exitPoint() - entryPoint() ).Mag2(); }

    /// Returns the path length in the given radiator
    [[nodiscard]] inline double pathLength() const { return std::sqrt( pathLength2() ); }

    /// Overload output to ostream
    friend inline std::ostream& operator<<( std::ostream& os, const Rich::RadIntersection& intersect ) {
      os << "["
         //<< " " << intersect.radiator()->radiatorID()
         << " Entry: Point=" << intersect.entryPoint() << " Dir=" << intersect.entryMomentum()
         << " Exit: Point=" << intersect.exitPoint() << " Dir=" << intersect.exitMomentum() << " ]";
      return os;
    }

  private:
    /// Entry point for radiator
    Gaudi::XYZPoint m_entryPoint;

    /// Momentum (direction) vector at the entry point
    Gaudi::XYZVector m_entryVect;

    /// Exit point for radiator
    Gaudi::XYZPoint m_exitPoint;

    /// Momentum (direction) vector at the entry point
    Gaudi::XYZVector m_exitVect;

    /// pointer to DeRichRadiator for this radiator
    const DeRichRadiator* m_radiator = nullptr;

  public:
    /** @class Sorter RichRadIntersection.h Kernel/RichRadIntersection.h
     *
     *  Utility class to sort the Radiator Intersections
     *
     *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
     *  @date   14/10/2006
     */
    class Sorter {
    private: // utility classes
      /// Functor to sort RichSmartIDs by Rich then panel numbers
      struct SortByZ {

        /** Sort operator for the RadIntersection
         *
         *  Sorts into order of increasing z position
         *
         *  @param i1 First RadIntersection
         *  @param i2 Second RadIntersection
         *
         *  @return bool indicating if i1 should be listed before i2
         */
        inline bool operator()( const Rich::RadIntersection& i1, const Rich::RadIntersection& i2 ) const {
          return ( i1.entryPoint().z() < i2.entryPoint().z() );
        }
      };

    public:
      /// Constructor
      Sorter() = default;

      /// Destructor
      ~Sorter() = default;

      /// Sort the vector of intersections into order increasing Z position
      inline static void sortByZ( Rich::RadIntersection::Vector& intersections ) {
        std::stable_sort( intersections.begin(), intersections.end(), Rich::RadIntersection::Sorter::SortByZ() );
      }
    };
  };

} // namespace Rich
