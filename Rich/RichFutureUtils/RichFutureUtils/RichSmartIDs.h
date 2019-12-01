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
#include <array>
#include <memory>

// RichDet
#include "RichDet/DeRich1.h"
#include "RichDet/DeRich2.h"
#include "RichDet/DeRichPD.h"
#include "RichDet/DeRichPDPanel.h"
#include "RichDet/DeRichSystem.h"

// Det Desc
#include "DetDesc/ConditionKey.h"
#include "DetDesc/IConditionDerivationMgr.h"

// Kernel
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichSide.h"
#include "Kernel/RichSmartID.h"

// Utils
#include "RichUtils/RichException.h"
#include "RichUtils/RichMap.h"
#include "RichUtils/RichPixelCluster.h"
#include "RichUtils/RichSIMDTypes.h"
#include "RichUtils/RichSmartIDSorter.h"

// Gaudi
#include "GaudiKernel/Point3DTypes.h"

namespace Rich::Utils {

  /** @class RichSmartIDs RichSmartIDs.h
   *
   *  Utility helper class for RichSmartIDs
   *
   *  @author Chris Jones
   *  @date   25/11/2019
   */
  class RichSmartIDs final {

  public:
    /// Disallow default constructor
    RichSmartIDs() = delete;

    /// Constructor from RICH detector elements
    RichSmartIDs( const DeRichSystem& richSys, //
                  const DeRich1&      rich1,   //
                  const DeRich2&      rich2 )
        : m_richS{&richSys} //
        , m_riches{&rich1, &rich2} {}

  public:
    // types

    // SIMD types
    using FP         = Rich::SIMD::DefaultScalarFP; ///< Default scalar floating point type
    using SIMDFP     = SIMD::FP<FP>;                ///< Default vector floating point type
    using SIMDVector = SIMD::Vector<FP>;            ///< Default vector Vector class
    using SIMDPoint  = SIMD::Point<FP>;             ///< Default vector Point class

  private:
    // methods

    /// Get the PD panel for the given SmartID
    inline decltype( auto ) panel( const LHCb::RichSmartID& ID ) const noexcept {
      return m_riches[ID.rich()]->pdPanel( ID.panel() );
    }

    /// Get the PD panel for the given RICH and side
    inline decltype( auto ) panel( const Rich::DetectorType rich, const Rich::Side side ) const noexcept {
      return m_riches[rich]->pdPanel( side );
    }

    /// Get the position for a given SmartID.
    inline bool _globalPosition( const LHCb::RichSmartID& smartID, //
                                 Gaudi::XYZPoint&         detectPoint ) const {
      return panel( smartID )->detectionPoint( smartID, detectPoint, m_hitPhotoCathSide );
    }

    /// Get the position for a given SmartID and associated DeRichPD object
    inline bool _globalPosition( const LHCb::RichSmartID& smartID, //
                                 const DeRichPD*          dePD,    //
                                 Gaudi::XYZPoint&         detectPoint ) const {
      return dePD->detectionPoint( smartID, detectPoint, m_hitPhotoCathSide );
    }

  public:
    // methods

    // Converts a RichSmartID channel identification into a position in global LHCb coordinates.
    bool globalPosition( const LHCb::RichSmartID& id, //
                         Gaudi::XYZPoint&         detectPoint ) const {
      return _globalPosition( id, detectPoint );
    }

    // Finds the average position of a cluster of RichSmartIDs, in global LHCb coordinates
    bool globalPosition( const Rich::PDPixelCluster& cluster, //
                         Gaudi::XYZPoint&            detectPoint ) const;

    // Finds the average positions of a vector of clusters, in global LHCb coordinates
    LHCb::STL::Vector<Gaudi::XYZPoint> globalPositions( const Rich::PDPixelCluster::Vector& clusters,
                                                        const bool ignoreClusters = false ) const;

    // Converts an PD RichSmartID identification into a position in global LHCb coordinates.
    bool pdPosition( const LHCb::RichSmartID& pdid, //
                     Gaudi::XYZPoint&         pdPoint ) const;

    // Computes the global position coordinate for a given position in local
    Gaudi::XYZPoint globalPosition( const Gaudi::XYZPoint&   localPoint, //
                                    const Rich::DetectorType rich,       //
                                    const Rich::Side         side ) const {
      return panel( rich, side )->PDPanelToGlobalMatrix() * localPoint;
    }

    // Converts a position in global coordinates to the corresponding RichSmartID
    bool smartID( const Gaudi::XYZPoint& globalPoint, //
                  LHCb::RichSmartID&     smartid ) const;

    // Supplies a vector of all currently active and valid channels in the RICH detectors
    LHCb::RichSmartID::Vector readoutChannelList() const;

    // Converts a position in global coordinates to the local coordinate system.
    Gaudi::XYZPoint globalToPDPanel( const Gaudi::XYZPoint& globalPoint ) const;

    // Converts a SIMD position in global coordinates to the local coordinate system
    SIMDPoint globalToPDPanel( const Rich::DetectorType rich, //
                               const SIMDPoint&         globalPoint ) const;

    // Converts a SIMD position in global coordinates to the local coordinate system
    SIMDPoint globalToPDPanel( const Rich::DetectorType rich, //
                               const Rich::Side         side, //
                               const SIMDPoint&         globalPoint ) const {
      return panel( rich, side )->globalToPDPanelMatrixSIMD() * globalPoint;
    }

  public:
    // conditions handling

    /// Creates a condition derivation for the given key
    template <typename PARENT>
    static decltype( auto ) //
    addConditionDerivation( PARENT* parent, LHCb::DetDesc::ConditionKey key ) {
      if ( parent->msgLevel( MSG::DEBUG ) ) {
        parent->debug() << "RichSmartIDs::addConditionDerivation : Key=" << key << endmsg;
      }
      return LHCb::DetDesc::                                        //
          addConditionDerivation<RichSmartIDs( const DeRichSystem&, //
                                               const DeRich1&,      //
                                               const DeRich2& )>(   //
              parent->conditionDerivationMgr(),                     // manager
              {DeRichLocations::RichSystem,                         // input conditions
               DeRichLocations::Rich1,                              // locations
               DeRichLocations::Rich2},                             //
              std::move( key ) );                                   // output location
    }

    /// Default conditions name
    static constexpr const char* DefaultConditionKey = "RichSmartIDs-Handler";

  private:
    // data

    /// RichSystem object
    const DeRichSystem* m_richS = nullptr;

    /// Pointers to RICH1 and RICH2
    Rich::DetectorArray<const DeRich*> m_riches = {{}};

    /// false to get the hit on the outside of PD window (including refraction)
    bool m_hitPhotoCathSide = false;
  };

} // namespace Rich::Utils
