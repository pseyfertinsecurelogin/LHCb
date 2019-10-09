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
#include "Kernel/FTChannelID.h"

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_FTCluster = 14003;

  // Namespace for locations in TDS
  namespace FTClusterLocation {
    inline const std::string Default = "Raw/FT/Clusters";
  }

  /** @class FTCluster FTCluster.h
   *
   * This class represents the clusters made of consecutive FTChannels
   *
   * @author Olivier Callot,Eric Cogneras
   *
   */

  class FTCluster final : public KeyedObject<LHCb::FTChannelID> {
  public:
    /// typedef for KeyedContainer of FTCluster
    typedef KeyedContainer<FTCluster, Containers::HashMap> Container;

    /// Constructor
    FTCluster( const LHCb::FTChannelID& id, float fraction, int fractionBit,
               const std::vector<LHCb::FTChannelID>& channelIDs, int isLarge, float charge );

    /// Default Constructor
    FTCluster() : m_fraction( 0.0 ), m_fractionBit( 0 ), m_isLarge( 0 ), m_charge( 0.0 ), m_channelIDs() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Retrieve the FTChannelID, key of the object
    const LHCb::FTChannelID& channelID() const;

    /// Number of cells in this cluster
    int size() const;

    /// Retrieve const  Fractional part of the channel ID, when barycentre is computed. between -0.25 and 0.75
    float fraction() const;

    /// Update  Fractional part of the channel ID, when barycentre is computed. between -0.25 and 0.75
    void setFraction( float value );

    /// Retrieve const  The fraction of channel as the integer value (0 or 1)
    int fractionBit() const;

    /// Update  The fraction of channel as the integer value (0 or 1)
    void setFractionBit( int value );

    /// Retrieve const  size category of the cluster: 0 lt 4, 1 lt 8, 2 others
    int isLarge() const;

    /// Update  size category of the cluster: 0 lt 4, 1 lt 8, 2 others
    void setIsLarge( int value );

    /// Retrieve const  Total charge in ADC count
    float charge() const;

    /// Update  Total charge in ADC count
    void setCharge( float value );

    /// Retrieve const  all FTChannelIDs corresponding to this cluster
    const std::vector<LHCb::FTChannelID>& channelIDs() const;

    float m_fraction;    ///< Fractional part of the channel ID, when barycentre is computed. between -0.25 and 0.75
    int   m_fractionBit; ///< The fraction of channel as the integer value (0 or 1)
    int   m_isLarge;     ///< size category of the cluster: 0 lt 4, 1 lt 8, 2 others
    float m_charge;      ///< Total charge in ADC count
    std::vector<LHCb::FTChannelID> m_channelIDs; ///< all FTChannelIDs corresponding to this cluster

  protected:
  private:
  }; // class FTCluster

  /// Definition of Keyed Container for FTCluster
  typedef KeyedContainer<FTCluster, Containers::HashMap> FTClusters;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::FTCluster::FTCluster( const LHCb::FTChannelID& id, float fraction, int fractionBit,
                                   const std::vector<LHCb::FTChannelID>& channelIDs, int isLarge, float charge )
    : m_fraction( fraction )
    , m_fractionBit( fractionBit )
    , m_isLarge( isLarge )
    , m_charge( charge )
    , m_channelIDs( channelIDs ) {
  this->setKey( id );
}

inline const CLID& LHCb::FTCluster::clID() const { return LHCb::FTCluster::classID(); }

inline const CLID& LHCb::FTCluster::classID() { return CLID_FTCluster; }

inline float LHCb::FTCluster::fraction() const { return m_fraction; }

inline void LHCb::FTCluster::setFraction( float value ) { m_fraction = value; }

inline int LHCb::FTCluster::fractionBit() const { return m_fractionBit; }

inline void LHCb::FTCluster::setFractionBit( int value ) { m_fractionBit = value; }

inline int LHCb::FTCluster::isLarge() const { return m_isLarge; }

inline void LHCb::FTCluster::setIsLarge( int value ) { m_isLarge = value; }

inline float LHCb::FTCluster::charge() const { return m_charge; }

inline void LHCb::FTCluster::setCharge( float value ) { m_charge = value; }

inline const std::vector<LHCb::FTChannelID>& LHCb::FTCluster::channelIDs() const { return m_channelIDs; }

inline const LHCb::FTChannelID& LHCb::FTCluster::channelID() const { return key(); }

inline int LHCb::FTCluster::size() const { return m_channelIDs.size(); }
