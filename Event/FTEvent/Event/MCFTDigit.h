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
#include "Event/MCFTDeposit.h"
#include "Event/MCHit.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "Kernel/FTChannelID.h"

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MCFTDigit = 14002;

  // Namespace for locations in TDS
  namespace MCFTDigitLocation {
    inline const std::string Default = "MC/FT/Digits";
  }

  /** @class MCFTDigit MCFTDigit.h
   *
   * This class represents the ADC charge collected per FTChannelID
   *
   * @author Eric Cogneras
   *
   */

  class MCFTDigit final : public KeyedObject<LHCb::FTChannelID> {
  public:
    /// typedef for KeyedContainer of MCFTDigit
    typedef KeyedContainer<MCFTDigit, Containers::HashMap> Container;

    /// Constructor
    MCFTDigit( const LHCb::FTChannelID& id, float PE, int adc, const std::vector<const LHCb::MCFTDeposit*>& deposits );

    /// Default Constructor
    MCFTDigit() : m_photoElectrons( 0.0 ), m_adcCount( 0 ), m_deposits() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Retrieve the FTChannelID, key of the object
    const LHCb::FTChannelID& channelID() const;

    /// Comparison for sorting by channel ID
    static bool lowerByChannelID( MCFTDigit* d1, MCFTDigit* d2 );

    /// Retrieve const  photoElectrons for ChannelID
    float photoElectrons() const;

    /// Update  photoElectrons for ChannelID
    void setPhotoElectrons( float value );

    /// Retrieve const  ADC Counts (2bit ADC PE threshold check) for ChannelID
    int adcCount() const;

    /// Update  ADC Counts (2bit ADC PE threshold check) for ChannelID
    void setAdcCount( int value );

    /// Retrieve const  the MCFTDeposits corresponding to this digit
    const std::vector<const LHCb::MCFTDeposit*>& deposits() const;

  protected:
  private:
    float                                 m_photoElectrons; ///< photoElectrons for ChannelID
    int                                   m_adcCount;       ///< ADC Counts (2bit ADC PE threshold check) for ChannelID
    std::vector<const LHCb::MCFTDeposit*> m_deposits;       ///< the MCFTDeposits corresponding to this digit

  }; // class MCFTDigit

  /// Definition of Keyed Container for MCFTDigit
  typedef KeyedContainer<MCFTDigit, Containers::HashMap> MCFTDigits;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::MCFTDigit::MCFTDigit( const LHCb::FTChannelID& id, float PE, int adc,
                                   const std::vector<const LHCb::MCFTDeposit*>& deposits )
    : m_photoElectrons( PE ), m_adcCount( adc ), m_deposits( deposits ) {
  this->setKey( id );
}

inline const CLID& LHCb::MCFTDigit::clID() const { return LHCb::MCFTDigit::classID(); }

inline const CLID& LHCb::MCFTDigit::classID() { return CLID_MCFTDigit; }

inline float LHCb::MCFTDigit::photoElectrons() const { return m_photoElectrons; }

inline void LHCb::MCFTDigit::setPhotoElectrons( float value ) { m_photoElectrons = value; }

inline int LHCb::MCFTDigit::adcCount() const { return m_adcCount; }

inline void LHCb::MCFTDigit::setAdcCount( int value ) { m_adcCount = value; }

inline const std::vector<const LHCb::MCFTDeposit*>& LHCb::MCFTDigit::deposits() const { return m_deposits; }

inline const LHCb::FTChannelID& LHCb::MCFTDigit::channelID() const { return key(); }

inline bool LHCb::MCFTDigit::lowerByChannelID( MCFTDigit* d1, MCFTDigit* d2 ) {
  return d1->channelID() < d2->channelID();
}
