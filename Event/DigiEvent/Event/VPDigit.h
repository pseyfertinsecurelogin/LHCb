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
#include "Kernel/VPChannelID.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_VPDigit = 4503;

  // Namespace for locations in TDS
  namespace VPDigitLocation {
    inline const std::string Default = "Raw/VP/Digits";
  }

  /** @class VPDigit VPDigit.h
   *
   * Digitized pixel hit in VP
   *
   * @author Marcin Kucharczyk
   *
   */

  class VPDigit final : public KeyedObject<LHCb::VPChannelID> {
  public:
    /// typedef for KeyedContainer of VPDigit
    typedef KeyedContainer<VPDigit, Containers::HashMap> Container;

    /// Default Constructor
    VPDigit() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Return channel = key
    VPChannelID channelID() const;

    friend std::ostream& operator<<( std::ostream& str, const VPDigit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
  }; // class VPDigit

  /// Definition of Keyed Container for VPDigit
  typedef KeyedContainer<VPDigit, Containers::HashMap> VPDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::VPDigit::clID() const { return LHCb::VPDigit::classID(); }

inline const CLID& LHCb::VPDigit::classID() { return CLID_VPDigit; }

inline std::ostream& LHCb::VPDigit::fillStream( std::ostream& s ) const { return s; }

inline LHCb::VPChannelID LHCb::VPDigit::channelID() const { return key(); }
