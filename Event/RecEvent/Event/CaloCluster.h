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
#include "Event/CaloClusterEntry.h"
#include "Event/CaloPosition.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/NamedRange.h"
#include "GaudiKernel/SerializeSTL.h"
#include "GaudiKernel/SharedObjectsContainer.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_CaloCluster = 2003;

  // Namespace for locations in TDS
  namespace CaloClusterLocation {
    inline const std::string Default      = "Rec/Calo/Clusters";
    inline const std::string EcalRaw      = "Rec/Calo/EcalClustersRaw";
    inline const std::string EcalOverlap  = "Rec/Calo/EcalClustersOverlap";
    inline const std::string Ecal         = "Rec/Calo/EcalClusters";
    inline const std::string Hcal         = "Rec/Calo/HcalClusters";
    inline const std::string EcalSplit    = "Rec/Calo/EcalSplitClusters";
    inline const std::string DefaultHlt   = "Hlt/Calo/Clusters";
    inline const std::string EcalHlt      = "Hlt/Calo/EcalClusters";
    inline const std::string HcalHlt      = "Hlt/Calo/HcalClusters";
    inline const std::string EcalSplitHlt = "Hlt/Calo/EcalSplitClusters";
    inline const std::string EcalHlt1     = "Hlt1/Calo/EcalClusters";
  } // namespace CaloClusterLocation

  /** @class CaloCluster CaloCluster.h
   *
   * Calorimeter Cluster * * * The class represents the cluster in calorimeter as
   * a * collection of connected and tagged cells. * *
   *
   * @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *
   */

  class CaloCluster final : public KeyedObject<int> {
  public:
    /// typedef for std::vector of CaloCluster
    typedef std::vector<CaloCluster*>       Vector;
    typedef std::vector<const CaloCluster*> ConstVector;

    /// typedef for KeyedContainer of CaloCluster
    typedef KeyedContainer<CaloCluster, Containers::HashMap> Container;

    /// For defining SharedObjectContainer
    typedef SharedObjectsContainer<LHCb::CaloCluster> Selection;
    /// For accessing a list of CaloClusters which is either a SharedObjectContainer, a KeyedContainer or a ConstVector
    typedef Gaudi::NamedRange_<ConstVector> Range;
    /// The shortcut for CaloClusterEntry class
    typedef LHCb::CaloClusterEntry Entry;
    /// Container with references to digits
    typedef std::vector<LHCb::CaloClusterEntry> Entries;
    /// The shortcut for container of CaloClusterEntry object
    typedef LHCb::CaloCluster::Entries Digits;
    /// The shortcut for CaloPosition type
    typedef LHCb::CaloPosition Position;

    /// CaloCluster type
    enum Type { Undefined = 0, Invalid, CellularAutomaton, Area3x3, Area2x3x3 };

    /// Default constructor
    CaloCluster() : m_type( LHCb::CaloCluster::Type::Undefined ), m_seed(), m_entries(), m_position() {}

    /// Copy Constructor
    CaloCluster( const LHCb::CaloCluster& right )
        : Base( right.key() )
        , m_type( right.type() )
        , m_seed( right.seed() )
        , m_entries( right.entries() )
        , m_position( right.position() ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve the energy @attention it is just a shortcut!
    double e() const;

    /// Retrieve const  The type of the cluster
    const LHCb::CaloCluster::Type& type() const;

    /// Update  The type of the cluster
    void setType( const LHCb::CaloCluster::Type& value );

    /// Retrieve const  The cellID of the seed digit for the cluster
    const LHCb::CaloCellID& seed() const;

    /// Update  The cellID of the seed digit for the cluster
    void setSeed( const LHCb::CaloCellID& value );

    /// Retrieve const  The cluster contents itself - entries : digits with their statuses
    const std::vector<LHCb::CaloClusterEntry>& entries() const;

    /// Retrieve  The cluster contents itself - entries : digits with their statuses
    std::vector<LHCb::CaloClusterEntry>& entries();

    /// Update  The cluster contents itself - entries : digits with their statuses
    void setEntries( const std::vector<LHCb::CaloClusterEntry>& value );

    /// Retrieve const  Cluster parameters: the position and the spread
    const LHCb::CaloCluster::Position& position() const;

    /// Retrieve  Cluster parameters: the position and the spread
    LHCb::CaloCluster::Position& position();

    /// Update  Cluster parameters: the position and the spread
    void setPosition( const LHCb::CaloCluster::Position& value );

    friend std::ostream& operator<<( std::ostream& str, const CaloCluster& obj ) { return obj.fillStream( str ); }

  protected:
    /// Shortcut for own base class
    typedef KeyedObject<int> Base;

  private:
    /// Assignment operator
    CaloCluster& operator=( const LHCb::CaloCluster& rhs );

    LHCb::CaloCluster::Type             m_type;    ///< The type of the cluster
    LHCb::CaloCellID                    m_seed;    ///< The cellID of the seed digit for the cluster
    std::vector<LHCb::CaloClusterEntry> m_entries; ///< The cluster contents itself - entries : digits with their
                                                   ///< statuses
    LHCb::CaloCluster::Position m_position;        ///< Cluster parameters: the position and the spread

  }; // class CaloCluster

  /// Definition of Keyed Container for CaloCluster
  typedef KeyedContainer<CaloCluster, Containers::HashMap> CaloClusters;

  inline std::ostream& operator<<( std::ostream& s, LHCb::CaloCluster::Type e ) {
    switch ( e ) {
    case LHCb::CaloCluster::Undefined:
      return s << "Undefined";
    case LHCb::CaloCluster::Invalid:
      return s << "Invalid";
    case LHCb::CaloCluster::CellularAutomaton:
      return s << "CellularAutomaton";
    case LHCb::CaloCluster::Area3x3:
      return s << "Area3x3";
    case LHCb::CaloCluster::Area2x3x3:
      return s << "Area2x3x3";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::CaloCluster::Type";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::CaloCluster::clID() const { return LHCb::CaloCluster::classID(); }

inline const CLID& LHCb::CaloCluster::classID() { return CLID_CaloCluster; }

inline std::ostream& LHCb::CaloCluster::fillStream( std::ostream& s ) const {
  s << "{ "
    << "type :	" << m_type << std::endl
    << "seed :	" << m_seed << std::endl
    << "entries :	" << m_entries << std::endl
    << "position :	" << m_position << std::endl
    << " }";
  return s;
}

inline const LHCb::CaloCluster::Type& LHCb::CaloCluster::type() const { return m_type; }

inline void LHCb::CaloCluster::setType( const LHCb::CaloCluster::Type& value ) { m_type = value; }

inline const LHCb::CaloCellID& LHCb::CaloCluster::seed() const { return m_seed; }

inline void LHCb::CaloCluster::setSeed( const LHCb::CaloCellID& value ) { m_seed = value; }

inline const std::vector<LHCb::CaloClusterEntry>& LHCb::CaloCluster::entries() const { return m_entries; }

inline std::vector<LHCb::CaloClusterEntry>& LHCb::CaloCluster::entries() { return m_entries; }

inline void LHCb::CaloCluster::setEntries( const std::vector<LHCb::CaloClusterEntry>& value ) { m_entries = value; }

inline const LHCb::CaloCluster::Position& LHCb::CaloCluster::position() const { return m_position; }

inline LHCb::CaloCluster::Position& LHCb::CaloCluster::position() { return m_position; }

inline void LHCb::CaloCluster::setPosition( const LHCb::CaloCluster::Position& value ) { m_position = value; }

inline double LHCb::CaloCluster::e() const { return position().e(); }
