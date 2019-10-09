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
#include "Event/Particle.h"
#include "Event/Tagger.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include "GaudiKernel/SmartRef.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_FlavourTag = 805;

  // Namespace for locations in TDS
  namespace FlavourTagLocation {
    inline const std::string Default = "Phys/Tagging";
  }

  /** @class FlavourTag FlavourTag.h
   *
   * The result of the tagging algorithm
   *
   * @author Marco Musy
   *
   */

  class FlavourTag final : public KeyedObject<int> {
  public:
    /// typedef for std::vector of FlavourTag
    typedef std::vector<FlavourTag*>       Vector;
    typedef std::vector<const FlavourTag*> ConstVector;

    /// typedef for KeyedContainer of FlavourTag
    typedef KeyedContainer<FlavourTag, Containers::HashMap> Container;

    /// Possible result of the tagging algorithm
    enum TagResult { b = -1, bbar = 1, none = 0 };

    /// Copy Constructor
    FlavourTag( const FlavourTag& c )
        : KeyedObject<int>()
        , m_decision( c.decision() )
        , m_omega( c.omega() )
        , m_taggers( c.taggers() )
        , m_decisionOS( c.decisionOS() )
        , m_omegaOS( c.omegaOS() )
        , m_taggedB( c.taggedB() ) {}

    /// Default Constructor
    FlavourTag() : m_decision( 0 ), m_omega( 0.50 ), m_taggers(), m_decisionOS( 0 ), m_omegaOS( 0.50 ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Assignment operator
    FlavourTag& operator=( const FlavourTag& rhs );

    /// Add one Tagger
    void addTagger( const LHCb::Tagger& HAS );

    /// Retrieve const  The result of the tagging algorithm
    short int decision() const;

    /// Update  The result of the tagging algorithm
    void setDecision( short int value );

    /// Retrieve const  Wrong tag fraction (predicted)
    float omega() const;

    /// Update  Wrong tag fraction (predicted)
    void setOmega( float value );

    /// Retrieve const  Vector of Taggers
    const std::vector<LHCb::Tagger>& taggers() const;

    /// Update  Vector of Taggers
    void setTaggers( const std::vector<LHCb::Tagger>& value );

    /// Retrieve const  decision of opposite side taggers only
    short int decisionOS() const;

    /// Update  decision of opposite side taggers only
    void setDecisionOS( short int value );

    /// Retrieve const  Wrong tag fraction (predicted) using opposite side only
    float omegaOS() const;

    /// Update  Wrong tag fraction (predicted) using opposite side only
    void setOmegaOS( float value );

    /// Retrieve (const)  The B meson for which this tag has been made
    const LHCb::Particle* taggedB() const;

    /// Update  The B meson for which this tag has been made
    void setTaggedB( const SmartRef<LHCb::Particle>& value );

    /// Update (pointer)  The B meson for which this tag has been made
    void setTaggedB( const LHCb::Particle* value );

    friend std::ostream& operator<<( std::ostream& str, const FlavourTag& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    short int                 m_decision;   ///< The result of the tagging algorithm
    float                     m_omega;      ///< Wrong tag fraction (predicted)
    std::vector<LHCb::Tagger> m_taggers;    ///< Vector of Taggers
    short int                 m_decisionOS; ///< decision of opposite side taggers only
    float                     m_omegaOS;    ///< Wrong tag fraction (predicted) using opposite side only
    SmartRef<LHCb::Particle>  m_taggedB;    ///< The B meson for which this tag has been made

  }; // class FlavourTag

  /// Definition of Keyed Container for FlavourTag
  typedef KeyedContainer<FlavourTag, Containers::HashMap> FlavourTags;

  inline std::ostream& operator<<( std::ostream& s, LHCb::FlavourTag::TagResult e ) {
    switch ( e ) {
    case LHCb::FlavourTag::b:
      return s << "b";
    case LHCb::FlavourTag::bbar:
      return s << "bbar";
    case LHCb::FlavourTag::none:
      return s << "none";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::FlavourTag::TagResult";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::FlavourTag::clID() const { return LHCb::FlavourTag::classID(); }

inline const CLID& LHCb::FlavourTag::classID() { return CLID_FlavourTag; }

inline std::ostream& LHCb::FlavourTag::fillStream( std::ostream& s ) const {
  s << "{ "
    << "decision :	" << m_decision << std::endl
    << "omega :	" << m_omega << std::endl
    << "taggers :	" << m_taggers << std::endl
    << "decisionOS :	" << m_decisionOS << std::endl
    << "omegaOS :	" << m_omegaOS << std::endl
    << " }";
  return s;
}

inline short int LHCb::FlavourTag::decision() const { return m_decision; }

inline void LHCb::FlavourTag::setDecision( short int value ) { m_decision = value; }

inline float LHCb::FlavourTag::omega() const { return m_omega; }

inline void LHCb::FlavourTag::setOmega( float value ) { m_omega = value; }

inline const std::vector<LHCb::Tagger>& LHCb::FlavourTag::taggers() const { return m_taggers; }

inline void LHCb::FlavourTag::setTaggers( const std::vector<LHCb::Tagger>& value ) { m_taggers = value; }

inline short int LHCb::FlavourTag::decisionOS() const { return m_decisionOS; }

inline void LHCb::FlavourTag::setDecisionOS( short int value ) { m_decisionOS = value; }

inline float LHCb::FlavourTag::omegaOS() const { return m_omegaOS; }

inline void LHCb::FlavourTag::setOmegaOS( float value ) { m_omegaOS = value; }

inline const LHCb::Particle* LHCb::FlavourTag::taggedB() const { return m_taggedB; }

inline void LHCb::FlavourTag::setTaggedB( const SmartRef<LHCb::Particle>& value ) { m_taggedB = value; }

inline void LHCb::FlavourTag::setTaggedB( const LHCb::Particle* value ) { m_taggedB = value; }

inline LHCb::FlavourTag& LHCb::FlavourTag::operator=( const FlavourTag& rhs ) {

  if ( this != &rhs ) {
    m_decision   = rhs.m_decision;
    m_omega      = rhs.m_omega;
    m_omegaOS    = rhs.m_omegaOS;
    m_taggers    = rhs.m_taggers;
    m_decisionOS = rhs.m_decisionOS;
    m_taggedB    = rhs.m_taggedB;
  }
  return *this;
}

inline void LHCb::FlavourTag::addTagger( const LHCb::Tagger& HAS ) {

  m_taggers.push_back( HAS );
  return;
}
