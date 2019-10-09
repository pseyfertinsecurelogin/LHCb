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
#include "Event/VeloLiteCluster.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "Kernel/VeloChannelID.h"
#include <utility>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_VeloCluster = 4006;

  // Namespace for locations in TDS
  namespace VeloClusterLocation {
    inline const std::string Default  = "Raw/Velo/Clusters";
    inline const std::string Emulated = "Emu/Velo/Clusters";
  } // namespace VeloClusterLocation

  /** @class VeloCluster VeloCluster.h
   *
   * A cluster as produced by the TELL1 with full ADC information.
   *
   * @author Chris Parkes
   *
   */

  class VeloCluster final : public KeyedObject<LHCb::VeloChannelID> {
  public:
    /// typedef for KeyedContainer of VeloCluster
    typedef KeyedContainer<VeloCluster, Containers::HashMap> Container;

    /// ADC vector pair typedef
    typedef std::vector<std::pair<int, unsigned int>> ADCVector;

    /// Constructor
    VeloCluster( const VeloLiteCluster& lCluster, const ADCVector& strips )
        : m_liteCluster( lCluster ), m_stripValues( strips ) {}

    /// copy constructor
    VeloCluster( const LHCb::VeloCluster& lCluster )
        : KeyedObject<LHCb::VeloChannelID>()
        , m_liteCluster( lCluster.m_liteCluster )
        , m_stripValues( lCluster.m_stripValues ) {}

    /// Default Constructor
    VeloCluster() : m_liteCluster(), m_stripValues() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// number of strips in cluster
    unsigned int size() const;

    /// adc value of strips in cluster
    unsigned int adcValue( unsigned int num ) const;

    /// strip number of strip in cluster
    int strip( unsigned int num ) const;

    /// The  Channel ID
    VeloChannelID channelID() const;

    /// total charge
    double totalCharge() const;

    /// position within a strip of cluster centre, quantized in xxths of a strip
    double interStripFraction() const;

    /// number of strips in cluster, 3 means 3 or more
    unsigned int pseudoSize() const;

    /// denotes a cluster with significant adc counts, less likely to be noise or spillover
    bool highThreshold() const;

    /// clone method
    VeloCluster* clone() const;

    /// first Velo Channel ID
    VeloChannelID firstChannel() const;

    /// first Velo Channel ID
    VeloChannelID lastChannel() const;

    /// channel IDs
    std::vector<LHCb::VeloChannelID> channels() const;

    /// check if R type
    bool isRType() const;

    /// check if Phi type
    bool isPhiType() const;

    /// check if Pile UP
    bool isPileUp() const;

    /// Retrieve const  TELL1 cluster without ADC values
    const LHCb::VeloLiteCluster& liteCluster() const;

    /// Retrieve const  strip numbers and their signals
    const ADCVector& stripValues() const;

    /// Update  strip numbers and their signals
    void setStripValues( const ADCVector& value );

  protected:
  private:
    LHCb::VeloLiteCluster m_liteCluster; ///< TELL1 cluster without ADC values
    ADCVector             m_stripValues; ///< strip numbers and their signals

  }; // class VeloCluster

  /// Definition of Keyed Container for VeloCluster
  typedef KeyedContainer<VeloCluster, Containers::HashMap> VeloClusters;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::VeloCluster::clID() const { return LHCb::VeloCluster::classID(); }

inline const CLID& LHCb::VeloCluster::classID() { return CLID_VeloCluster; }

inline const LHCb::VeloLiteCluster& LHCb::VeloCluster::liteCluster() const { return m_liteCluster; }

inline const LHCb::VeloCluster::ADCVector& LHCb::VeloCluster::stripValues() const { return m_stripValues; }

inline void LHCb::VeloCluster::setStripValues( const ADCVector& value ) { m_stripValues = value; }

inline unsigned int LHCb::VeloCluster::size() const { return stripValues().size(); }

inline unsigned int LHCb::VeloCluster::adcValue( unsigned int num ) const { return stripValues()[num].second; }

inline int LHCb::VeloCluster::strip( unsigned int num ) const { return stripValues()[num].first; }

inline LHCb::VeloChannelID LHCb::VeloCluster::channelID() const { return m_liteCluster.channelID(); }

inline double LHCb::VeloCluster::totalCharge() const {

  double                    sum  = 0;
  ADCVector::const_iterator iter = m_stripValues.begin();
  for ( ; iter != m_stripValues.end(); ++iter ) { sum += iter->second; }
  return sum;
}

inline double LHCb::VeloCluster::interStripFraction() const { return m_liteCluster.interStripFraction(); }

inline unsigned int LHCb::VeloCluster::pseudoSize() const { return m_liteCluster.pseudoSize(); }

inline bool LHCb::VeloCluster::highThreshold() const { return m_liteCluster.highThreshold(); }

inline LHCb::VeloCluster* LHCb::VeloCluster::clone() const { return new LHCb::VeloCluster( *this ); }

inline LHCb::VeloChannelID LHCb::VeloCluster::firstChannel() const {

  return VeloChannelID( channelID().sensor(), stripValues().front().first, channelID().type() );
}

inline LHCb::VeloChannelID LHCb::VeloCluster::lastChannel() const {

  return VeloChannelID( channelID().sensor(), stripValues().back().first, channelID().type() );
}

inline std::vector<LHCb::VeloChannelID> LHCb::VeloCluster::channels() const {

  std::vector<LHCb::VeloChannelID> chanCont;
  VeloChannelID                    first = firstChannel();
  for ( unsigned int i = 0; i < size(); i++ ) {
    VeloChannelID aChan( first.sensor(), first.strip() + i, first.type() );
    chanCont.push_back( aChan );
  }
  return chanCont;
}

inline bool LHCb::VeloCluster::isRType() const { return m_liteCluster.channelID().isRType(); }

inline bool LHCb::VeloCluster::isPhiType() const { return m_liteCluster.channelID().isPhiType(); }

inline bool LHCb::VeloCluster::isPileUp() const { return m_liteCluster.channelID().isPileUp(); }
