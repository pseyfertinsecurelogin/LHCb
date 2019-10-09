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
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class HltDecReport HltDecReport.h
   *
   * Hlt Trigger Decision
   *
   * @author Tomasz Skwarnicki
   *
   */

  class HltDecReport final {
  public:
    /// create from integer
    HltDecReport( unsigned int decRep ) : m_decReport( decRep ) {}

    /// create from subfield inputs with allowed range checking
    HltDecReport( bool dec, int stage, unsigned int eb, int noc, int decID );

    /// create from subset of subfield inputs with allowed range checking
    HltDecReport( bool dec, unsigned int eb, int noc, int decID );

    /// Default Constructor
    HltDecReport() : m_decReport( 0 ) {}

    /// set decision bit; keep event= (dec==true)
    void setDecision( bool dec );

    /// Update code specifying highest step reached during the trigger line execution (0-255)
    void setExecutionStage( int stage );

    /// if numberOfCandidates in HltDecReport equals this then actual number might have been larger
    static unsigned int saturatedNumberOfCandidates();

    /// set numberOfCandidates; set to saturation value if larger than max allowed; set to 0 if negative
    void setNumberOfCandidates( int noc );

    /// deprecated backwards compatibility stub
    unsigned int intSelectionID() const;

    /// if intDecisionID in HltDecReport equals this then passed decision ID was out of allowed range
    static unsigned int illegalIntDecisionID();

    /// set intDecisionID; set to illegal value if out of allowed range
    void setIntDecisionID( int decID );

    /// checked if intDecisionID was invalid when set
    bool invalidIntDecisionID() const;

    /// intelligent printout
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  encoded decision, execution stage, errors, number of candidates and decision ID
    unsigned int decReport() const;

    /// Update  encoded decision, execution stage, errors, number of candidates and decision ID
    void setDecReport( unsigned int value );

    /// Retrieve decision 1(true)=this trigger line asked to keep the event; 0(false)=this trigger did not ask to keep
    /// the event
    unsigned int decision() const;

    /// Retrieve error code 0=none
    unsigned int errorBits() const;

    /// Update error code 0=none
    void setErrorBits( unsigned int value );

    /// Retrieve number of selected candidates passing trigger decision criteria
    unsigned int numberOfCandidates() const;

    /// Retrieve code specifying highest step reached during the trigger line execution (0-255)
    unsigned int executionStage() const;

    /// Retrieve integer decision ID
    unsigned int intDecisionID() const;

    friend std::ostream& operator<<( std::ostream& str, const HltDecReport& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Offsets of bitfield decReport
    enum decReportBits {
      decisionBits           = 0,
      errorBitsBits          = 1,
      numberOfCandidatesBits = 4,
      executionStageBits     = 8,
      intDecisionIDBits      = 16
    };

    /// Bitmasks for bitfield decReport
    enum decReportMasks {
      decisionMask           = 0x1L,
      errorBitsMask          = 0xeL,
      numberOfCandidatesMask = 0xf0L,
      executionStageMask     = 0xff00L,
      intDecisionIDMask      = 0xffff0000L
    };

    unsigned int m_decReport; ///< encoded decision, execution stage, errors, number of candidates and decision ID

  }; // class HltDecReport

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::HltDecReport::HltDecReport( bool dec, int stage, unsigned int eb, int noc, int decID ) {

  m_decReport = 0;
  setDecision( dec );
  setExecutionStage( stage );
  setErrorBits( eb );
  setNumberOfCandidates( noc );
  setIntDecisionID( decID );
}

inline LHCb::HltDecReport::HltDecReport( bool dec, unsigned int eb, int noc, int decID ) {

  m_decReport = 0;
  setDecision( dec );
  setExecutionStage( 0 ); // not specified
  setErrorBits( eb );
  setNumberOfCandidates( noc );
  setIntDecisionID( decID );
}

inline unsigned int LHCb::HltDecReport::decReport() const { return m_decReport; }

inline void LHCb::HltDecReport::setDecReport( unsigned int value ) { m_decReport = value; }

inline unsigned int LHCb::HltDecReport::decision() const {
  return (unsigned int)( ( m_decReport & decisionMask ) >> decisionBits );
}

inline unsigned int LHCb::HltDecReport::errorBits() const {
  return (unsigned int)( ( m_decReport & errorBitsMask ) >> errorBitsBits );
}

inline void LHCb::HltDecReport::setErrorBits( unsigned int value ) {
  unsigned int val = (unsigned int)value;
  m_decReport &= ~errorBitsMask;
  m_decReport |= ( ( ( (unsigned int)val ) << errorBitsBits ) & errorBitsMask );
}

inline unsigned int LHCb::HltDecReport::numberOfCandidates() const {
  return (unsigned int)( ( m_decReport & numberOfCandidatesMask ) >> numberOfCandidatesBits );
}

inline unsigned int LHCb::HltDecReport::executionStage() const {
  return (unsigned int)( ( m_decReport & executionStageMask ) >> executionStageBits );
}

inline unsigned int LHCb::HltDecReport::intDecisionID() const {
  return (unsigned int)( ( m_decReport & intDecisionIDMask ) >> intDecisionIDBits );
}

inline void LHCb::HltDecReport::setDecision( bool dec ) {

  m_decReport &= ~decisionMask;
  if ( dec ) { m_decReport |= ( ( ( (unsigned int)1 ) << decisionBits ) & decisionMask ); }
}

inline void LHCb::HltDecReport::setExecutionStage( int stage ) {

  unsigned int val = ( stage < 0 ) ? 0 : ( (unsigned int)stage );
  val              = ( val < 255 ) ? val : 255;
  m_decReport &= ~executionStageMask;
  m_decReport |= ( ( ( (unsigned int)val ) << executionStageBits ) & executionStageMask );
}

inline unsigned int LHCb::HltDecReport::saturatedNumberOfCandidates() {

  return numberOfCandidatesMask >> numberOfCandidatesBits;
}

inline void LHCb::HltDecReport::setNumberOfCandidates( int noc ) {

  unsigned int val = ( noc < 0 ) ? 0 : ( (unsigned int)noc );
  val              = ( val < saturatedNumberOfCandidates() ) ? val : saturatedNumberOfCandidates();
  m_decReport &= ~numberOfCandidatesMask;
  m_decReport |= ( ( val << numberOfCandidatesBits ) & numberOfCandidatesMask );
}

inline unsigned int LHCb::HltDecReport::intSelectionID() const { return intDecisionID(); }

inline unsigned int LHCb::HltDecReport::illegalIntDecisionID() {

  return (unsigned int)( intDecisionIDMask >> intDecisionIDBits );
}

inline void LHCb::HltDecReport::setIntDecisionID( int decID ) {

  unsigned int val = ( decID <= 0 ) ? illegalIntDecisionID() : ( (unsigned int)decID );
  val              = ( val < illegalIntDecisionID() ) ? val : illegalIntDecisionID();
  m_decReport &= ~intDecisionIDMask;
  m_decReport |= ( ( val << intDecisionIDBits ) & intDecisionIDMask );
}

inline bool LHCb::HltDecReport::invalidIntDecisionID() const {

  return ( ( intDecisionID() == illegalIntDecisionID() ) || ( intDecisionID() == 0 ) );
}
