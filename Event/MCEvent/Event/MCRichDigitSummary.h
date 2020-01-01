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
#include "Event/MCParticle.h"
#include "Event/MCRichDigitHistoryCode.h"
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "Kernel/RichSmartID.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MCRichDigitSummary = 12409;

  // Namespace for locations in TDS
  namespace MCRichDigitSummaryLocation {
    inline const std::string Default = "MC/Rich/DigitSummaries";
  }

  /** @class MCRichDigitSummary MCRichDigitSummary.h
   *
   * Summary object for MCRichDigits (RichSmartIDs)
   *
   * @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *
   */

  class MCRichDigitSummary : public ContainedObject {
  public:
    /// typedef for std::vector of MCRichDigitSummary
    using Vector      = std::vector<MCRichDigitSummary*>;
    using ConstVector = std::vector<const MCRichDigitSummary*>;

    /// typedef for ObjectVector of MCRichDigitSummary
    using Container = ObjectVector<MCRichDigitSummary>;

    /// Default Constructor
    MCRichDigitSummary() = default;

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Bit-packed history information
    const LHCb::MCRichDigitHistoryCode& history() const;

    /// Update  Bit-packed history information
    void setHistory( const LHCb::MCRichDigitHistoryCode& value );

    /// Retrieve const  RichSmartID channel identifier
    const LHCb::RichSmartID& richSmartID() const;

    /// Update  RichSmartID channel identifier
    void setRichSmartID( const LHCb::RichSmartID& value );

    /// Retrieve (const)  MCParticle associated to this RichDigit (RichSmartID)
    const LHCb::MCParticle* mcParticle() const;

    /// Update  MCParticle associated to this RichDigit (RichSmartID)
    void setMCParticle( const SmartRef<LHCb::MCParticle>& value );

    /// Update (pointer)  MCParticle associated to this RichDigit (RichSmartID)
    void setMCParticle( const LHCb::MCParticle* value );

    friend std::ostream& operator<<( std::ostream& str, const MCRichDigitSummary& obj ) {
      return obj.fillStream( str );
    }

  private:
    LHCb::MCRichDigitHistoryCode m_history{0};  ///< Bit-packed history information
    LHCb::RichSmartID            m_richSmartID; ///< RichSmartID channel identifier
    SmartRef<LHCb::MCParticle>   m_MCParticle;  ///< MCParticle associated to this RichDigit (RichSmartID)

  }; // class MCRichDigitSummary

  /// Definition of vector container type for MCRichDigitSummary
  using MCRichDigitSummarys = ObjectVector<MCRichDigitSummary>;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCRichDigitSummary::clID() const { return LHCb::MCRichDigitSummary::classID(); }

inline const CLID& LHCb::MCRichDigitSummary::classID() { return CLID_MCRichDigitSummary; }

inline std::ostream& LHCb::MCRichDigitSummary::fillStream( std::ostream& s ) const {
  s << "{ "
    << "history :	" << m_history << std::endl
    << "richSmartID :	" << m_richSmartID << std::endl
    << " }";
  return s;
}

inline const LHCb::MCRichDigitHistoryCode& LHCb::MCRichDigitSummary::history() const { return m_history; }

inline void LHCb::MCRichDigitSummary::setHistory( const LHCb::MCRichDigitHistoryCode& value ) { m_history = value; }

inline const LHCb::RichSmartID& LHCb::MCRichDigitSummary::richSmartID() const { return m_richSmartID; }

inline void LHCb::MCRichDigitSummary::setRichSmartID( const LHCb::RichSmartID& value ) { m_richSmartID = value; }

inline const LHCb::MCParticle* LHCb::MCRichDigitSummary::mcParticle() const { return m_MCParticle; }

inline void LHCb::MCRichDigitSummary::setMCParticle( const SmartRef<LHCb::MCParticle>& value ) { m_MCParticle = value; }

inline void LHCb::MCRichDigitSummary::setMCParticle( const LHCb::MCParticle* value ) { m_MCParticle = value; }
