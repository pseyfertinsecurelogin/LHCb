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
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/NamedRange.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SharedObjectsContainer.h"
#include "Kernel/CaloCellID.h"
#include <ostream>
#include <string>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_L0CaloCandidate = 6001;

  // Namespace for locations in TDS
  namespace L0CaloCandidateLocation {
    inline const std::string Default = "Trig/L0/Calo";
    inline const std::string Full    = "Trig/L0/FullCalo";
  } // namespace L0CaloCandidateLocation

  /** @class L0CaloCandidate L0CaloCandidate.h
   *
   * This class represents the input from the calorimeter to the L0 trigger
   *
   * @author Olivier Callot
   *
   */

  class L0CaloCandidate final : public ContainedObject {
  public:
    /// typedef for std::vector of L0CaloCandidate
    typedef std::vector<L0CaloCandidate*>       Vector;
    typedef std::vector<const L0CaloCandidate*> ConstVector;

    /// typedef for ObjectVector of L0CaloCandidate
    typedef ObjectVector<L0CaloCandidate> Container;

    /// For defining SharedObjectContainer
    typedef SharedObjectsContainer<LHCb::L0CaloCandidate> Selection;
    /// For accessing a list of L0CaloCandidates which is either a SharedObjectContainer, a KeyedContainer or a
    /// ConstVector
    typedef Gaudi::NamedRange_<ConstVector> Range;

    /// special constructor
    L0CaloCandidate( int type, const CaloCellID& ID, int etCode, double et, const Gaudi::XYZPoint& cellCenter,
                     double cellSize )
        : m_type( type ), m_ID( ID ), m_etCode( etCode ), m_et( et ), m_position( cellCenter ), m_posTol( cellSize ) {}

    /// Default Constructor
    L0CaloCandidate() : m_type( 0 ), m_ID(), m_etCode( 0 ), m_et( 0.0 ), m_position(), m_posTol( 0.0 ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Returns the type of candidate as a string, for printing
    std::string typeName() const;

    /// Print on MsgStream
    MsgStream& fillStream( MsgStream& msg ) const;

    /// Print operator
    friend MsgStream& operator<<( MsgStream& ms, const LHCb::L0CaloCandidate& cand );

    /// Print operator
    friend MsgStream& operator<<( MsgStream& ms, const LHCb::L0CaloCandidate* cand );

    /// Retrieve const  Type of trigger, from L0DUBase::Type
    int type() const;

    /// Retrieve const  Calo Cell ID. To access MC information...
    const LHCb::CaloCellID& id() const;

    /// Retrieve const  integer version of Et.
    int etCode() const;

    /// Retrieve const  Transverse energy
    double et() const;

    /// Retrieve const  Position
    const Gaudi::XYZPoint& position() const;

    /// Retrieve const  Tolerance on x/y position = 1/2 cell size
    double posTol() const;

    friend std::ostream& operator<<( std::ostream& str, const L0CaloCandidate& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    int              m_type;     ///< Type of trigger, from L0DUBase::Type
    LHCb::CaloCellID m_ID;       ///< Calo Cell ID. To access MC information...
    int              m_etCode;   ///< integer version of Et.
    double           m_et;       ///< Transverse energy
    Gaudi::XYZPoint  m_position; ///< Position
    double           m_posTol;   ///< Tolerance on x/y position = 1/2 cell size

  }; // class L0CaloCandidate

  /// Definition of vector container type for L0CaloCandidate
  typedef ObjectVector<L0CaloCandidate> L0CaloCandidates;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::L0CaloCandidate::clID() const { return LHCb::L0CaloCandidate::classID(); }

inline const CLID& LHCb::L0CaloCandidate::classID() { return CLID_L0CaloCandidate; }

inline std::ostream& LHCb::L0CaloCandidate::fillStream( std::ostream& s ) const {
  s << "{ "
    << "type :	" << m_type << std::endl
    << "ID :	" << m_ID << std::endl
    << "etCode :	" << m_etCode << std::endl
    << "et :	" << (float)m_et << std::endl
    << "position :	" << m_position << std::endl
    << "posTol :	" << (float)m_posTol << std::endl
    << " }";
  return s;
}

inline int LHCb::L0CaloCandidate::type() const { return m_type; }

inline const LHCb::CaloCellID& LHCb::L0CaloCandidate::id() const { return m_ID; }

inline int LHCb::L0CaloCandidate::etCode() const { return m_etCode; }

inline double LHCb::L0CaloCandidate::et() const { return m_et; }

inline const Gaudi::XYZPoint& LHCb::L0CaloCandidate::position() const { return m_position; }

inline double LHCb::L0CaloCandidate::posTol() const { return m_posTol; }
