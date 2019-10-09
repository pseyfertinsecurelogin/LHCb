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
#include "GaudiKernel/NamedRange.h"
#include "GaudiKernel/SerializeSTL.h"
#include "GaudiKernel/SharedObjectsContainer.h"
#include "Kernel/MuonTileID.h"
#include <ostream>
#include <string>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_L0MuonCandidate = 6002;

  // Namespace for locations in TDS
  namespace L0MuonCandidateLocation {
    inline const std::string Default  = "Trig/L0/MuonCtrl";
    inline const std::string Ctrl     = "Trig/L0/MuonCtrl";
    inline const std::string CtrlBCSU = "Trig/L0/MuonCtrlBCSU";
    inline const std::string BCSU     = "Trig/L0/MuonBCSU";
    inline const std::string PU       = "Trig/L0/MuonPU";
  } // namespace L0MuonCandidateLocation

  /** @class L0MuonCandidate L0MuonCandidate.h
   *
   * L0Muon trigger candidate * representing the output of the L0Muon trigger
   * processor.
   *
   * @author A. Tsaregorodtsev
   *
   */

  class L0MuonCandidate final : public KeyedObject<int> {
  public:
    /// typedef for std::vector of L0MuonCandidate
    typedef std::vector<L0MuonCandidate*>       Vector;
    typedef std::vector<const L0MuonCandidate*> ConstVector;

    /// typedef for KeyedContainer of L0MuonCandidate
    typedef KeyedContainer<L0MuonCandidate, Containers::HashMap> Container;

    /// For defining SharedObjectContainer
    typedef SharedObjectsContainer<LHCb::L0MuonCandidate> Selection;
    /// For accessing a list of L0MuonCandidates which is either a SharedObjectContainer, a KeyedContainer or a
    /// ConstVector
    typedef Gaudi::NamedRange_<ConstVector> Range;

    /// Empty constructor
    L0MuonCandidate();

    /// Constructor with full information
    L0MuonCandidate( double pt, double theta, double phi, const std::vector<LHCb::MuonTileID>& pads, int encodedPt );

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Accessor to the list of LHCb::MuonTileIDs in a given station
    std::vector<LHCb::MuonTileID> muonTileIDs( unsigned int station ) const;

    /// Comparison operator for sorting candidates in Pt
    bool operator<( const L0MuonCandidate& lmc ) const;

    /// Comparison operator
    bool operator==( const L0MuonCandidate& lmc ) const;

    /// Retrieve const  Pt of the candidate
    double pt() const;

    /// Retrieve const  Theta at IP of the candidate
    double theta() const;

    /// Retrieve const  Phi at IP of the candidate
    double phi() const;

    /// Retrieve const  Pads used in the candidate
    const std::vector<LHCb::MuonTileID>& pads() const;

    /// Retrieve const  encoded Pt of the candidate
    int encodedPt() const;

    friend std::ostream& operator<<( std::ostream& str, const L0MuonCandidate& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    double                        m_pt;        ///< Pt of the candidate
    double                        m_theta;     ///< Theta at IP of the candidate
    double                        m_phi;       ///< Phi at IP of the candidate
    std::vector<LHCb::MuonTileID> m_pads;      ///< Pads used in the candidate
    int                           m_encodedPt; ///< encoded Pt of the candidate

  }; // class L0MuonCandidate

  /// Definition of Keyed Container for L0MuonCandidate
  typedef KeyedContainer<L0MuonCandidate, Containers::HashMap> L0MuonCandidates;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::L0MuonCandidate::clID() const { return LHCb::L0MuonCandidate::classID(); }

inline const CLID& LHCb::L0MuonCandidate::classID() { return CLID_L0MuonCandidate; }

inline std::ostream& LHCb::L0MuonCandidate::fillStream( std::ostream& s ) const {
  s << "{ "
    << "pt :	" << (float)m_pt << std::endl
    << "theta :	" << (float)m_theta << std::endl
    << "phi :	" << (float)m_phi << std::endl
    << "pads :	" << m_pads << std::endl
    << "encodedPt :	" << m_encodedPt << std::endl
    << " }";
  return s;
}

inline double LHCb::L0MuonCandidate::pt() const { return m_pt; }

inline double LHCb::L0MuonCandidate::theta() const { return m_theta; }

inline double LHCb::L0MuonCandidate::phi() const { return m_phi; }

inline const std::vector<LHCb::MuonTileID>& LHCb::L0MuonCandidate::pads() const { return m_pads; }

inline int LHCb::L0MuonCandidate::encodedPt() const { return m_encodedPt; }
