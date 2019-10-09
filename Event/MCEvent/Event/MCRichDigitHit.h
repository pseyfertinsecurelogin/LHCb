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
#include "Event/MCRichDigitHistoryCode.h"
#include "Event/MCRichHit.h"
#include "GaudiKernel/SmartRef.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class MCRichDigitHit MCRichDigitHit.h
   *
   * Utility class used by MCRichDigit to store an associated MCRichHit and a
   * history code which summarises how the MCRichHit was producted and also how
   * it was used in the digitisation to create the parent MCRichDigit
   *
   * @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *
   */

  class MCRichDigitHit {
  public:
    /// Vector of MCRichDigitHit
    typedef std::vector<LHCb::MCRichDigitHit> Vector;

    /// Constructor from MCRichHit and history code
    MCRichDigitHit( const LHCb::MCRichHit& hit, const LHCb::MCRichDigitHistoryCode hist )
        : m_history( hist ), m_mcRichHit( &hit ) {}

    /// Default Constructor
    MCRichDigitHit() : m_history( 0 ) {}

    /// Default Destructor
    ~MCRichDigitHit() {}

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  Bit-packed word containing the overall history information for this hit. A summary of both how
    /// the hit was produced and also how it was used in the digitisation
    const LHCb::MCRichDigitHistoryCode& history() const;

    /// Update  Bit-packed word containing the overall history information for this hit. A summary of both how the hit
    /// was produced and also how it was used in the digitisation
    void setHistory( const LHCb::MCRichDigitHistoryCode& value );

    /// Retrieve (const)  Associated MCRichHit
    const LHCb::MCRichHit* mcRichHit() const;

    /// Update  Associated MCRichHit
    void setMcRichHit( const SmartRef<LHCb::MCRichHit>& value );

    /// Update (pointer)  Associated MCRichHit
    void setMcRichHit( const LHCb::MCRichHit* value );

    friend std::ostream& operator<<( std::ostream& str, const MCRichDigitHit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    LHCb::MCRichDigitHistoryCode m_history; ///< Bit-packed word containing the overall history information for this
                                            ///< hit. A summary of both how the hit was produced and also how it was
                                            ///< used in the digitisation
    SmartRef<LHCb::MCRichHit> m_mcRichHit;  ///< Associated MCRichHit

  }; // class MCRichDigitHit

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::MCRichDigitHit::fillStream( std::ostream& s ) const {
  s << "{ "
    << "history :	" << m_history << std::endl
    << " }";
  return s;
}

inline const LHCb::MCRichDigitHistoryCode& LHCb::MCRichDigitHit::history() const { return m_history; }

inline void LHCb::MCRichDigitHit::setHistory( const LHCb::MCRichDigitHistoryCode& value ) { m_history = value; }

inline const LHCb::MCRichHit* LHCb::MCRichDigitHit::mcRichHit() const { return m_mcRichHit; }

inline void LHCb::MCRichDigitHit::setMcRichHit( const SmartRef<LHCb::MCRichHit>& value ) { m_mcRichHit = value; }

inline void LHCb::MCRichDigitHit::setMcRichHit( const LHCb::MCRichHit* value ) { m_mcRichHit = value; }
