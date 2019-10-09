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
#include "Event/CaloCluster.h"
#include "Event/CaloDigit.h"
#include "Event/CaloPosition.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/NamedRange.h"
#include "GaudiKernel/SharedObjectsContainer.h"
#include "GaudiKernel/SmartRefVector.h"
#include "Kernel/CaloCellID.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_CaloHypo = 2004;

  // Namespace for locations in TDS
  namespace CaloHypoLocation {
    inline const std::string Default         = "Rec/Calo/Hypos";
    inline const std::string Photons         = "Rec/Calo/Photons";
    inline const std::string Electrons       = "Rec/Calo/Electrons";
    inline const std::string MergedPi0s      = "Rec/Calo/MergedPi0s";
    inline const std::string SplitPhotons    = "Rec/Calo/SplitPhotons";
    inline const std::string DefaultHlt      = "Hlt/Calo/Hypos";
    inline const std::string PhotonsHlt      = "Hlt/Calo/Photons";
    inline const std::string PhotonsHlt1     = "Hlt1/Calo/Photons";
    inline const std::string ElectronsHlt    = "Hlt/Calo/Electrons";
    inline const std::string MergedPi0sHlt   = "Hlt/Calo/MergedPi0s";
    inline const std::string SplitPhotonsHlt = "Hlt/Calo/SplitPhotons";
  } // namespace CaloHypoLocation

  /** @class CaloHypo CaloHypo.h
   *
   * @brief class Calorimeter Hypotheses * * * The class represents the
   * calorimeter hypothesis. * *
   *
   * @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *
   */

  class CaloHypo final : public KeyedObject<int> {
  public:
    /// typedef for std::vector of CaloHypo
    typedef std::vector<CaloHypo*>       Vector;
    typedef std::vector<const CaloHypo*> ConstVector;

    /// typedef for KeyedContainer of CaloHypo
    typedef KeyedContainer<CaloHypo, Containers::HashMap> Container;

    /// For defining SharedObjectContainer
    typedef SharedObjectsContainer<LHCb::CaloHypo> Selection;
    /// For accessing a list of CaloHypos which is either a SharedObjectContainer, a KeyedContainer or a ConstVector
    typedef Gaudi::NamedRange_<ConstVector> Range;
    /// Shortcut for the type of Likelihood of CaloHypo object
    typedef double Likelihood;
    /// Shortcut for the type of CaloPosition object
    typedef LHCb::CaloPosition Position;
    /// shortcut for references to Calorimeter Digits
    typedef SmartRefVector<LHCb::CaloDigit> Digits;
    /// Shortcut for references to Calorimeter Clusters
    typedef SmartRefVector<LHCb::CaloCluster> Clusters;
    /// Shortcut for eferences to Calorimeter Hypothesis
    typedef SmartRefVector<LHCb::CaloHypo> Hypos;

    /// Calo hypotheses
    enum Hypothesis {
      Undefined = 0,
      Mip,
      MipPositive,
      MipNegative,
      Photon,
      PhotonFromMergedPi0,
      BremmstrahlungPhoton,
      Pi0Resolved,
      Pi0Overlapped,
      Pi0Merged,
      EmCharged,
      Positron,
      Electron,
      EmChargedSeed,
      PositronSeed,
      ElectronSeed,
      NeutralHadron,
      ChargedHadron,
      PositiveHadron,
      NegativeHadron,
      Jet,
      Other
    };

    /// Copy Constructor
    CaloHypo( const LHCb::CaloHypo& right )
        : Base()
        , m_hypothesis( right.hypothesis() )
        , m_lh( right.lh() )
        , m_position( right.position() ? new CaloPosition( *right.position() ) : nullptr )
        , m_digits( right.digits() )
        , m_clusters( right.clusters() )
        , m_hypos( right.hypos() ) {}

    /// Default Constructor
    CaloHypo() : m_hypothesis( CaloHypo::Hypothesis::Undefined ), m_lh( -1. ), m_position( 0 ) {}

    /// Destructor
    ~CaloHypo();

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Update the hypothesis's CaloPosition object.
    void setPosition( std::unique_ptr<LHCb::CaloHypo::Position> position );

    /// Retrieve the energy @attention it is just a shortcut!
    double e() const;

    /// Print this CaloHypo data object in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  The hypothesis's ID
    const LHCb::CaloHypo::Hypothesis& hypothesis() const;

    /// Update  The hypothesis's ID
    void setHypothesis( const LHCb::CaloHypo::Hypothesis& value );

    /// Retrieve const  The Hypothesis's likelihood
    const LHCb::CaloHypo::Likelihood& lh() const;

    /// Update  The Hypothesis's likelihood
    void setLh( const LHCb::CaloHypo::Likelihood& value );

    /// Retrieve const  The hypothesis's CaloPosition object
    const LHCb::CaloHypo::Position* position() const;

    /// Retrieve  The hypothesis's CaloPosition object
    LHCb::CaloHypo::Position* position();

    /// Retrieve (const)  References to the Calorimeter Digits
    const SmartRefVector<LHCb::CaloDigit>& digits() const;

    /// Retrieve  References to the Calorimeter Digits
    SmartRefVector<LHCb::CaloDigit>& digits();

    /// Update  References to the Calorimeter Digits
    void setDigits( const SmartRefVector<LHCb::CaloDigit>& value );

    /// Add to  References to the Calorimeter Digits
    void addToDigits( const SmartRef<LHCb::CaloDigit>& value );

    /// Att to (pointer)  References to the Calorimeter Digits
    void addToDigits( const LHCb::CaloDigit* value );

    /// Remove from  References to the Calorimeter Digits
    void removeFromDigits( const SmartRef<LHCb::CaloDigit>& value );

    /// Clear  References to the Calorimeter Digits
    void clearDigits();

    /// Retrieve (const)  References to the Calorimeter Clusters
    const SmartRefVector<LHCb::CaloCluster>& clusters() const;

    /// Update  References to the Calorimeter Clusters
    void setClusters( const SmartRefVector<LHCb::CaloCluster>& value );

    /// Add to  References to the Calorimeter Clusters
    void addToClusters( const SmartRef<LHCb::CaloCluster>& value );

    /// Att to (pointer)  References to the Calorimeter Clusters
    void addToClusters( const LHCb::CaloCluster* value );

    /// Remove from  References to the Calorimeter Clusters
    void removeFromClusters( const SmartRef<LHCb::CaloCluster>& value );

    /// Clear  References to the Calorimeter Clusters
    void clearClusters();

    /// Retrieve (const)  References to the Calorimeter Hypos
    const SmartRefVector<LHCb::CaloHypo>& hypos() const;

    /// Update  References to the Calorimeter Hypos
    void setHypos( const SmartRefVector<LHCb::CaloHypo>& value );

    /// Add to  References to the Calorimeter Hypos
    void addToHypos( const SmartRef<LHCb::CaloHypo>& value );

    /// Att to (pointer)  References to the Calorimeter Hypos
    void addToHypos( const LHCb::CaloHypo* value );

    /// Remove from  References to the Calorimeter Hypos
    void removeFromHypos( const SmartRef<LHCb::CaloHypo>& value );

    /// Clear  References to the Calorimeter Hypos
    void clearHypos();

    friend std::ostream& operator<<( std::ostream& str, const CaloHypo& obj ) { return obj.fillStream( str ); }

  protected:
    /// Shortcut for own base class
    typedef KeyedObject<int> Base;

  private:
    LHCb::CaloHypo::Hypothesis        m_hypothesis; ///< The hypothesis's ID
    LHCb::CaloHypo::Likelihood        m_lh;         ///< The Hypothesis's likelihood
    LHCb::CaloHypo::Position*         m_position;   ///< The hypothesis's CaloPosition object
    SmartRefVector<LHCb::CaloDigit>   m_digits;     ///< References to the Calorimeter Digits
    SmartRefVector<LHCb::CaloCluster> m_clusters;   ///< References to the Calorimeter Clusters
    SmartRefVector<LHCb::CaloHypo>    m_hypos;      ///< References to the Calorimeter Hypos

  }; // class CaloHypo

  /// Definition of Keyed Container for CaloHypo
  typedef KeyedContainer<CaloHypo, Containers::HashMap> CaloHypos;

  inline std::ostream& operator<<( std::ostream& s, LHCb::CaloHypo::Hypothesis e ) {
    switch ( e ) {
    case LHCb::CaloHypo::Undefined:
      return s << "Undefined";
    case LHCb::CaloHypo::Mip:
      return s << "Mip";
    case LHCb::CaloHypo::MipPositive:
      return s << "MipPositive";
    case LHCb::CaloHypo::MipNegative:
      return s << "MipNegative";
    case LHCb::CaloHypo::Photon:
      return s << "Photon";
    case LHCb::CaloHypo::PhotonFromMergedPi0:
      return s << "PhotonFromMergedPi0";
    case LHCb::CaloHypo::BremmstrahlungPhoton:
      return s << "BremmstrahlungPhoton";
    case LHCb::CaloHypo::Pi0Resolved:
      return s << "Pi0Resolved";
    case LHCb::CaloHypo::Pi0Overlapped:
      return s << "Pi0Overlapped";
    case LHCb::CaloHypo::Pi0Merged:
      return s << "Pi0Merged";
    case LHCb::CaloHypo::EmCharged:
      return s << "EmCharged";
    case LHCb::CaloHypo::Positron:
      return s << "Positron";
    case LHCb::CaloHypo::Electron:
      return s << "Electron";
    case LHCb::CaloHypo::EmChargedSeed:
      return s << "EmChargedSeed";
    case LHCb::CaloHypo::PositronSeed:
      return s << "PositronSeed";
    case LHCb::CaloHypo::ElectronSeed:
      return s << "ElectronSeed";
    case LHCb::CaloHypo::NeutralHadron:
      return s << "NeutralHadron";
    case LHCb::CaloHypo::ChargedHadron:
      return s << "ChargedHadron";
    case LHCb::CaloHypo::PositiveHadron:
      return s << "PositiveHadron";
    case LHCb::CaloHypo::NegativeHadron:
      return s << "NegativeHadron";
    case LHCb::CaloHypo::Jet:
      return s << "Jet";
    case LHCb::CaloHypo::Other:
      return s << "Other";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::CaloHypo::Hypothesis";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::CaloHypo::~CaloHypo() { delete m_position; }

inline const CLID& LHCb::CaloHypo::clID() const { return LHCb::CaloHypo::classID(); }

inline const CLID& LHCb::CaloHypo::classID() { return CLID_CaloHypo; }

inline const LHCb::CaloHypo::Hypothesis& LHCb::CaloHypo::hypothesis() const { return m_hypothesis; }

inline void LHCb::CaloHypo::setHypothesis( const LHCb::CaloHypo::Hypothesis& value ) { m_hypothesis = value; }

inline const LHCb::CaloHypo::Likelihood& LHCb::CaloHypo::lh() const { return m_lh; }

inline void LHCb::CaloHypo::setLh( const LHCb::CaloHypo::Likelihood& value ) { m_lh = value; }

inline const LHCb::CaloHypo::Position* LHCb::CaloHypo::position() const { return m_position; }

inline LHCb::CaloHypo::Position* LHCb::CaloHypo::position() { return m_position; }

inline const SmartRefVector<LHCb::CaloDigit>& LHCb::CaloHypo::digits() const { return m_digits; }

inline SmartRefVector<LHCb::CaloDigit>& LHCb::CaloHypo::digits() { return m_digits; }

inline void LHCb::CaloHypo::setDigits( const SmartRefVector<LHCb::CaloDigit>& value ) { m_digits = value; }

inline void LHCb::CaloHypo::addToDigits( const SmartRef<LHCb::CaloDigit>& value ) { m_digits.push_back( value ); }

inline void LHCb::CaloHypo::addToDigits( const LHCb::CaloDigit* value ) { m_digits.push_back( value ); }

inline void LHCb::CaloHypo::removeFromDigits( const SmartRef<LHCb::CaloDigit>& value ) {
  auto i = std::remove( m_digits.begin(), m_digits.end(), value );
  m_digits.erase( i, m_digits.end() );
}

inline void LHCb::CaloHypo::clearDigits() { m_digits.clear(); }

inline const SmartRefVector<LHCb::CaloCluster>& LHCb::CaloHypo::clusters() const { return m_clusters; }

inline void LHCb::CaloHypo::setClusters( const SmartRefVector<LHCb::CaloCluster>& value ) { m_clusters = value; }

inline void LHCb::CaloHypo::addToClusters( const SmartRef<LHCb::CaloCluster>& value ) { m_clusters.push_back( value ); }

inline void LHCb::CaloHypo::addToClusters( const LHCb::CaloCluster* value ) { m_clusters.push_back( value ); }

inline void LHCb::CaloHypo::removeFromClusters( const SmartRef<LHCb::CaloCluster>& value ) {
  auto i = std::remove( m_clusters.begin(), m_clusters.end(), value );
  m_clusters.erase( i, m_clusters.end() );
}

inline void LHCb::CaloHypo::clearClusters() { m_clusters.clear(); }

inline const SmartRefVector<LHCb::CaloHypo>& LHCb::CaloHypo::hypos() const { return m_hypos; }

inline void LHCb::CaloHypo::setHypos( const SmartRefVector<LHCb::CaloHypo>& value ) { m_hypos = value; }

inline void LHCb::CaloHypo::addToHypos( const SmartRef<LHCb::CaloHypo>& value ) { m_hypos.push_back( value ); }

inline void LHCb::CaloHypo::addToHypos( const LHCb::CaloHypo* value ) { m_hypos.push_back( value ); }

inline void LHCb::CaloHypo::removeFromHypos( const SmartRef<LHCb::CaloHypo>& value ) {
  auto i = std::remove( m_hypos.begin(), m_hypos.end(), value );
  m_hypos.erase( i, m_hypos.end() );
}

inline void LHCb::CaloHypo::clearHypos() { m_hypos.clear(); }

inline void LHCb::CaloHypo::setPosition( std::unique_ptr<LHCb::CaloHypo::Position> position ) {

  m_position = position.release();
}

inline double LHCb::CaloHypo::e() const { return m_position ? m_position->e() : 0; }
