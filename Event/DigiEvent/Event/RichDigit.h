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
#include "Kernel/RichSmartID.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_RichDigit = 12407;

  // Namespace for locations in TDS
  namespace RichDigitLocation {
    inline const std::string Default = "Raw/Rich/Digits";
  }

  /** @class RichDigit RichDigit.h
   *
   * The Rich Digitisation object
   *
   * @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *
   */

  class RichDigit final : public KeyedObject<LHCb::RichSmartID> {
  public:
    /// typedef for std::vector of RichDigit
    typedef std::vector<RichDigit*>       Vector;
    typedef std::vector<const RichDigit*> ConstVector;

    /// typedef for KeyedContainer of RichDigit
    typedef KeyedContainer<RichDigit, Containers::HashMap> Container;

    /// Default Constructor
    RichDigit() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Get the channel identifier for this digit (RichSmartID)
    RichSmartID richSmartID() const;

    friend std::ostream& operator<<( std::ostream& str, const RichDigit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
  }; // class RichDigit

  /// Definition of Keyed Container for RichDigit
  typedef KeyedContainer<RichDigit, Containers::HashMap> RichDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::RichDigit::clID() const { return LHCb::RichDigit::classID(); }

inline const CLID& LHCb::RichDigit::classID() { return CLID_RichDigit; }

inline std::ostream& LHCb::RichDigit::fillStream( std::ostream& s ) const { return s; }

inline LHCb::RichSmartID LHCb::RichDigit::richSmartID() const { return LHCb::RichSmartID( key() ); }
