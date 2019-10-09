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
#include "Event/MCRichDigitHit.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "Kernel/RichSmartID.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MCRichDigit = 12408;

  // Namespace for locations in TDS
  namespace MCRichDigitLocation {
    inline const std::string Default = "MC/Rich/Digits";
  }

  /** @class MCRichDigit MCRichDigit.h
   *
   * Monte Carlo Rich Digitisation object
   *
   * @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *
   */

  class MCRichDigit : public KeyedObject<LHCb::RichSmartID> {
  public:
    /// typedef for std::vector of MCRichDigit
    typedef std::vector<MCRichDigit*>       Vector;
    typedef std::vector<const MCRichDigit*> ConstVector;

    /// typedef for KeyedContainer of MCRichDigit
    typedef KeyedContainer<MCRichDigit, Containers::HashMap> Container;

    /// Default Constructor
    MCRichDigit() : m_hits(), m_history( 0 ) {}

    /// Default Destructor
    virtual ~MCRichDigit() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Get the channel identifier for this digit (RichSmartID)
    RichSmartID richSmartID() const;

    /// Print this MCRichDigit in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  MCRichHits contributing to this digitisation object, together with their history code
    const LHCb::MCRichDigitHit::Vector& hits() const;

    /// Update  MCRichHits contributing to this digitisation object, together with their history code
    void setHits( const LHCb::MCRichDigitHit::Vector& value );

    /// Retrieve const  Bit-packed word containing the overall history information for this digit
    const LHCb::MCRichDigitHistoryCode& history() const;

    /// Update  Bit-packed word containing the overall history information for this digit
    void setHistory( const LHCb::MCRichDigitHistoryCode& value );

    friend std::ostream& operator<<( std::ostream& str, const MCRichDigit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    LHCb::MCRichDigitHit::Vector m_hits; ///< MCRichHits contributing to this digitisation object, together with their
                                         ///< history code
    LHCb::MCRichDigitHistoryCode m_history; ///< Bit-packed word containing the overall history information for this
                                            ///< digit

  }; // class MCRichDigit

  /// Definition of Keyed Container for MCRichDigit
  typedef KeyedContainer<MCRichDigit, Containers::HashMap> MCRichDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCRichDigit::clID() const { return LHCb::MCRichDigit::classID(); }

inline const CLID& LHCb::MCRichDigit::classID() { return CLID_MCRichDigit; }

inline const LHCb::MCRichDigitHit::Vector& LHCb::MCRichDigit::hits() const { return m_hits; }

inline void LHCb::MCRichDigit::setHits( const LHCb::MCRichDigitHit::Vector& value ) { m_hits = value; }

inline const LHCb::MCRichDigitHistoryCode& LHCb::MCRichDigit::history() const { return m_history; }

inline void LHCb::MCRichDigit::setHistory( const LHCb::MCRichDigitHistoryCode& value ) { m_history = value; }

inline LHCb::RichSmartID LHCb::MCRichDigit::richSmartID() const { return LHCb::RichSmartID( key() ); }
