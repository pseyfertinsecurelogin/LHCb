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
#include "Event/ProcessHeader.h"
#include "GaudiKernel/SmartRefVector.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class MCVertex;

  // Class ID definition
  static const CLID CLID_MCHeader = 104;

  // Namespace for locations in TDS
  namespace MCHeaderLocation {
    inline const std::string Default = LHCb::ProcessHeaderLocation::MC;
  }

  /** @class MCHeader MCHeader.h
   *
   * Header for simulation information
   *
   * @author P. Koppenburg, revised by G.Corti
   *
   */

  class MCHeader : public ProcessHeader {
  public:
    /// Copy constructor. Creates a new MCHeader with the same information
    MCHeader( const LHCb::MCHeader& head )
        : ProcessHeader( head )
        , m_evtNumber( head.evtNumber() )
        , m_evtTime( head.evtTime() )
        , m_primaryVertices( head.primaryVertices() ) {}

    /// Default Constructor
    MCHeader() : m_evtNumber( 0 ), m_evtTime( 0 ) {}

    /// Default Destructor
    virtual ~MCHeader() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Number of primary vertices in bunch crossing
    unsigned int numOfPrimaryVertices() const;

    /// Retrieve const  Event number
    long long evtNumber() const;

    /// Update  Event number
    void setEvtNumber( long long value );

    /// Retrieve const  Event time
    unsigned long long evtTime() const;

    /// Update  Event time
    void setEvtTime( unsigned long long value );

    /// Retrieve (const)  Pointer to primary vertices
    const SmartRefVector<LHCb::MCVertex>& primaryVertices() const;

    /// Update  Pointer to primary vertices
    void setPrimaryVertices( const SmartRefVector<LHCb::MCVertex>& value );

    /// Add to  Pointer to primary vertices
    void addToPrimaryVertices( const SmartRef<LHCb::MCVertex>& value );

    /// Att to (pointer)  Pointer to primary vertices
    void addToPrimaryVertices( const LHCb::MCVertex* value );

    /// Remove from  Pointer to primary vertices
    void removeFromPrimaryVertices( const SmartRef<LHCb::MCVertex>& value );

    /// Clear  Pointer to primary vertices
    void clearPrimaryVertices();

    friend std::ostream& operator<<( std::ostream& str, const MCHeader& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    long long                      m_evtNumber;       ///< Event number
    unsigned long long             m_evtTime;         ///< Event time
    SmartRefVector<LHCb::MCVertex> m_primaryVertices; ///< Pointer to primary vertices

  }; // class MCHeader

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations
#include "MCVertex.h"

inline const CLID& LHCb::MCHeader::clID() const { return LHCb::MCHeader::classID(); }

inline const CLID& LHCb::MCHeader::classID() { return CLID_MCHeader; }

inline std::ostream& LHCb::MCHeader::fillStream( std::ostream& s ) const {
  ProcessHeader::fillStream( s );
  s << "{ "
    << "evtNumber :	" << m_evtNumber << std::endl
    << "evtTime :	" << m_evtTime << std::endl
    << " }";
  return s;
}

inline long long LHCb::MCHeader::evtNumber() const { return m_evtNumber; }

inline void LHCb::MCHeader::setEvtNumber( long long value ) { m_evtNumber = value; }

inline unsigned long long LHCb::MCHeader::evtTime() const { return m_evtTime; }

inline void LHCb::MCHeader::setEvtTime( unsigned long long value ) { m_evtTime = value; }

inline const SmartRefVector<LHCb::MCVertex>& LHCb::MCHeader::primaryVertices() const { return m_primaryVertices; }

inline void LHCb::MCHeader::setPrimaryVertices( const SmartRefVector<LHCb::MCVertex>& value ) {
  m_primaryVertices = value;
}

inline void LHCb::MCHeader::addToPrimaryVertices( const SmartRef<LHCb::MCVertex>& value ) {
  m_primaryVertices.push_back( value );
}

inline void LHCb::MCHeader::addToPrimaryVertices( const LHCb::MCVertex* value ) {
  m_primaryVertices.push_back( value );
}

inline void LHCb::MCHeader::removeFromPrimaryVertices( const SmartRef<LHCb::MCVertex>& value ) {
  auto i = std::remove( m_primaryVertices.begin(), m_primaryVertices.end(), value );
  m_primaryVertices.erase( i, m_primaryVertices.end() );
}

inline void LHCb::MCHeader::clearPrimaryVertices() { m_primaryVertices.clear(); }

inline unsigned int LHCb::MCHeader::numOfPrimaryVertices() const { return primaryVertices().size(); }
