/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

#include <cstdint>
#include <iostream>
#include <string>

// forward declaration to allow use as friend
class LbAppInit;
class HltConfigSvc;

namespace LHCb {
  /**
   Simple class to record binary (`BINARY_TAG`, AKA `CMTCONFIG`) and host
   information (instruction set as defined in `VectorClass/instrset.h`).

   The default contructor initializes the data member to the correct values for
   the binary tag used at build time and for the instructions set of the CPU
   (run time).

   Users cannot alter the data members.

   \note{
    If LbAppInit is used by the application, it ensures that the CPU
    instructions set is correct in case of checkpointing.
   }
  */
  class PlatformInfo final {
  public:
    /// Initialize the instance with build time `BINARY_TAG` and run time CPU
    /// instructions set level (`VectorClass/instrset.h`).
    PlatformInfo();
    /// Copy infos from an existing instance (thay may not match the current
    /// run time info).
    PlatformInfo( const PlatformInfo& rhs ) = default;

    /// Numeric id of the build time `BINARY_TAG` (defined by an internal map).
    std::uint16_t binaryId() const { return m_binaryId; }
    /// Instructions set level of the CPU (run time).
    std::uint16_t hostInstrSetLevel() const { return m_hostInstrSetLevel; }

    /// Build time string version of `BINARY_TAG` (AKA `CMTCONFIG`).
    const std::string& binaryTag() const;
    /// String name of the CPU instructions set.
    const std::string& hostInstrSet() const;

  private:
    PlatformInfo( std::uint16_t binaryId, std::uint16_t hostInstrSetLevel );

    std::uint16_t m_binaryId;
    std::uint16_t m_hostInstrSetLevel;

    /// static cache to speed up constration
    static std::uint16_t s_hostInstrSetLevel;

    /// Helper for a nice print out of the instance content.
    friend std::ostream& operator<<( std::ostream& s, const PlatformInfo& id ) {
      return s << id.binaryTag() << ':' << id.hostInstrSet();
    }

    // This is needed to allow LbAppInit to override the cached s_hostInstrSetLevel
    // after the library was loaded (needed to correctly support checkpointing)
    friend LbAppInit;
    friend HltConfigSvc;
  };
} // namespace LHCb
