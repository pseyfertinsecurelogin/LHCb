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
#include "Event/HepMCEvent.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SmartRef.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_GenCollision = 204;

  // Namespace for locations in TDS
  namespace GenCollisionLocation {
    inline const std::string Default = "Gen/Collisions";
  }

  /** @class GenCollision GenCollision.h
   *
   * All generator-level information for a given collision (An event is made up
   * of several collisions (e.g. one per PileUp))
   *
   * @author G.Corti, revised by P. Koppenburg
   *
   */

  class GenCollision : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of GenCollision
    typedef KeyedContainer<GenCollision, Containers::HashMap> Container;

    /// Default Constructor
    GenCollision()
        : m_isSignal( false )
        , m_processType( 0 )
        , m_sHat( 0.0 )
        , m_tHat( 0.0 )
        , m_uHat( 0.0 )
        , m_ptHat( 0.0 )
        , m_x1Bjorken( 0.0 )
        , m_x2Bjorken( 0.0 ) {}

    /// Default Destructor
    virtual ~GenCollision() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  true for collision that contains the generated signal. false for minimum bias and pileup.
    bool isSignal() const;

    /// Update  true for collision that contains the generated signal. false for minimum bias and pileup.
    void setIsSignal( bool value );

    /// Retrieve const  Generator process type
    long processType() const;

    /// Update  Generator process type
    void setProcessType( long value );

    /// Retrieve const  Mandelstam s hat
    double sHat() const;

    /// Update  Mandelstam s hat
    void setSHat( double value );

    /// Retrieve const  Mandelstam t hat
    double tHat() const;

    /// Update  Mandelstam t hat
    void setTHat( double value );

    /// Retrieve const  Mandelstam u hat
    double uHat() const;

    /// Update  Mandelstam u hat
    void setUHat( double value );

    /// Retrieve const  Mandelstam pt hat
    double ptHat() const;

    /// Update  Mandelstam pt hat
    void setPtHat( double value );

    /// Retrieve const  Bjorken x1
    double x1Bjorken() const;

    /// Update  Bjorken x1
    void setX1Bjorken( double value );

    /// Retrieve const  Bjorken x2
    double x2Bjorken() const;

    /// Update  Bjorken x2
    void setX2Bjorken( double value );

    /// Retrieve (const)  Reference to HepMC Event of this collision
    const LHCb::HepMCEvent* event() const;

    /// Update  Reference to HepMC Event of this collision
    void setEvent( const SmartRef<LHCb::HepMCEvent>& value );

    /// Update (pointer)  Reference to HepMC Event of this collision
    void setEvent( const LHCb::HepMCEvent* value );

    friend std::ostream& operator<<( std::ostream& str, const GenCollision& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    bool   m_isSignal;    ///< true for collision that contains the generated signal. false for minimum bias and pileup.
    long   m_processType; ///< Generator process type
    double m_sHat;        ///< Mandelstam s hat
    double m_tHat;        ///< Mandelstam t hat
    double m_uHat;        ///< Mandelstam u hat
    double m_ptHat;       ///< Mandelstam pt hat
    double m_x1Bjorken;   ///< Bjorken x1
    double m_x2Bjorken;   ///< Bjorken x2
    SmartRef<LHCb::HepMCEvent> m_event; ///< Reference to HepMC Event of this collision

  }; // class GenCollision

  /// Definition of Keyed Container for GenCollision
  typedef KeyedContainer<GenCollision, Containers::HashMap> GenCollisions;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::GenCollision::clID() const { return LHCb::GenCollision::classID(); }

inline const CLID& LHCb::GenCollision::classID() { return CLID_GenCollision; }

inline std::ostream& LHCb::GenCollision::fillStream( std::ostream& s ) const {
  char l_isSignal = ( m_isSignal ) ? 'T' : 'F';
  s << "{ "
    << "isSignal :	" << l_isSignal << std::endl
    << "processType :	" << m_processType << std::endl
    << "sHat :	" << (float)m_sHat << std::endl
    << "tHat :	" << (float)m_tHat << std::endl
    << "uHat :	" << (float)m_uHat << std::endl
    << "ptHat :	" << (float)m_ptHat << std::endl
    << "x1Bjorken :	" << (float)m_x1Bjorken << std::endl
    << "x2Bjorken :	" << (float)m_x2Bjorken << std::endl
    << " }";
  return s;
}

inline bool LHCb::GenCollision::isSignal() const { return m_isSignal; }

inline void LHCb::GenCollision::setIsSignal( bool value ) { m_isSignal = value; }

inline long LHCb::GenCollision::processType() const { return m_processType; }

inline void LHCb::GenCollision::setProcessType( long value ) { m_processType = value; }

inline double LHCb::GenCollision::sHat() const { return m_sHat; }

inline void LHCb::GenCollision::setSHat( double value ) { m_sHat = value; }

inline double LHCb::GenCollision::tHat() const { return m_tHat; }

inline void LHCb::GenCollision::setTHat( double value ) { m_tHat = value; }

inline double LHCb::GenCollision::uHat() const { return m_uHat; }

inline void LHCb::GenCollision::setUHat( double value ) { m_uHat = value; }

inline double LHCb::GenCollision::ptHat() const { return m_ptHat; }

inline void LHCb::GenCollision::setPtHat( double value ) { m_ptHat = value; }

inline double LHCb::GenCollision::x1Bjorken() const { return m_x1Bjorken; }

inline void LHCb::GenCollision::setX1Bjorken( double value ) { m_x1Bjorken = value; }

inline double LHCb::GenCollision::x2Bjorken() const { return m_x2Bjorken; }

inline void LHCb::GenCollision::setX2Bjorken( double value ) { m_x2Bjorken = value; }

inline const LHCb::HepMCEvent* LHCb::GenCollision::event() const { return m_event; }

inline void LHCb::GenCollision::setEvent( const SmartRef<LHCb::HepMCEvent>& value ) { m_event = value; }

inline void LHCb::GenCollision::setEvent( const LHCb::HepMCEvent* value ) { m_event = value; }
