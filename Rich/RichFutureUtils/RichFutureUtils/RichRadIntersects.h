/*****************************************************************************\
* (c) Copyright 2000-2020 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

#pragma once

// STL
#include <algorithm>
#include <array>
#include <vector>

// LHCbKernel
#include "Kernel/RichRadIntersection.h"
#include "Kernel/RichRadiatorType.h"

// Math
#include "GaudiKernel/Transform3DTypes.h"

// RichDet
#include "RichDet/DeRichRadiator.h"

// Det Desc
#include "DetDesc/ConditionKey.h"
#include "DetDesc/IConditionDerivationMgr.h"

namespace Rich::Utils {

  //-----------------------------------------------------------------------------
  /** @class RadIntersects RichRadIntersects.h
   *
   *  Utility to find intersections with the RICH radiators
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   2019-11-29
   */
  //-----------------------------------------------------------------------------

  class RadIntersects final {

  public:
    // construtors

    // no default constructor
    RadIntersects() = delete;

    // From radiators
    RadIntersects( const DeRichRadiator& r1gas,  // RICH1 gas volume
                   const DeRichRadiator& r2gas ) // RICH2 gas volume
        : m_radiators{nullptr, &r1gas, &r2gas}   // Aero hardcoded to off...
    {}

  public:
    /** @brief Finds intersections of a given vector from a given point (entry/exit)
     *
     * @param globalPoint   The start point for the intersection extraplotion
     * @param globalVector  The direction vector for the intersection extraplotion
     * @param radiator      The radiator to find the intersections in
     * @param intersections The found intersections
     *
     * @return The number of intersections
     */
    inline unsigned int intersections( const Gaudi::XYZPoint&         globalPoint,  //
                                       const Gaudi::XYZVector&        globalVector, //
                                       const Rich::RadiatorType       radiator,     //
                                       Rich::RadIntersection::Vector& intersections ) const {

      // clear any current intersections
      intersections.clear();

      // Try intersection with the correct volume
      auto R = m_radiators[radiator];
      if ( R ) {
        Gaudi::XYZPoint entry, exit;
        if ( R->intersectionPoints( globalPoint, globalVector, entry, exit ) ) {
          // save this intersection
          intersections.emplace_back( entry, globalVector, exit, globalVector, R );
        }
      }

      // return the number of intersections
      return intersections.size();
    }

  private:
    /// Radiators for each RICH
    RadiatorArray<const DeRichRadiator*> m_radiators = {{}};

  public:
    // conditions handling

    /// Creates a condition derivation for the given key
    template <typename PARENT>
    static inline LHCb::DetDesc::IConditionDerivationMgr::DerivationId //
    addConditionDerivation( PARENT* parent, LHCb::DetDesc::ConditionKey key ) {
      if ( parent->msgLevel( MSG::DEBUG ) ) {
        parent->debug() << "RadIntersects::addConditionDerivation : Key=" << key << endmsg;
      }
      using SA = std::array<std::string, 2>; //
      return LHCb::DetDesc::                 //
          addConditionDerivation( parent->conditionDerivationMgr(),
                                  SA{DeRichLocations::Rich1Gas,     // input conditions locations
                                     DeRichLocations::Rich2Gas},    //
                                  std::move( key ),                 // output derived condition location
                                  []( const DeRichRadiator& r1gas,  //
                                      const DeRichRadiator& r2gas ) //
                                  {
                                    return RadIntersects{r1gas, r2gas};
                                  } );
    }
  };

} // namespace Rich::Utils
