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
#include "Kernel/VeloChannelID.h"
#include <ostream>
#include <string>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_VeloFullFPGADigit = 4003;

  // Namespace for locations in TDS
  namespace VeloFullFPGADigitLocation {
    inline const std::string Default = "Raw/Velo/FullDigits";
  }

  /** @class VeloFullFPGADigit VeloFullFPGADigit.h
   *
   * Full information on a reconstructed single digitised velo channel. This
   * info. is available inside the data processor but is not normally output on
   * DAQ stream for all channels. Updated VeloFullDigit object contains info.
   * about ADC sample througout all stages of FPGA processing
   *
   * @author Chris Parkes, update Tomasz Szumlak
   *
   */

  class VeloFullFPGADigit : public KeyedObject<LHCb::VeloChannelID> {
  public:
    /// typedef for KeyedContainer of VeloFullFPGADigit
    typedef KeyedContainer<VeloFullFPGADigit, Containers::HashMap> Container;

    /// processing stages performed by FPGA processor
    enum processingStages { RAW = 0, FIR, PedestalSubtracted, CMSubtracted };
    /// masks
    enum processingListMasks { mRAW = 0x01, mFIR = 0x02, mPedestalSubtracted = 0x04, mCMSubtracted = 0x08 };

    /// Constructor with key
    VeloFullFPGADigit( const LHCb::VeloChannelID& key )
        : KeyedObject<LHCb::VeloChannelID>( key )
        , m_processingList( 0 )
        , m_rawADCValue( 0 )
        , m_ADCFIRcorrection( 0.0 )
        , m_subtractedPedestal( 0.0 )
        , m_subtractedCM( 0.0 )
        , m_rawNoise( 0.0 )
        , m_noise( 0.0 )
        , m_flag() {}

    /// Default Constructor
    VeloFullFPGADigit()
        : m_processingList( 0 )
        , m_rawADCValue( 0.0 )
        , m_ADCFIRcorrection( 0.0 )
        , m_subtractedPedestal( 0.0 )
        , m_subtractedCM( 0.0 )
        , m_rawNoise( 0.0 )
        , m_noise( 0.0 )
        , m_flag() {}

    /// Default Destructor
    virtual ~VeloFullFPGADigit() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// combined sensor and strip number ID
    VeloChannelID channelID() const;

    /// Sensor Number
    long sensor() const;

    /// Strip Number
    long strip() const;

    /// Set the value of the Raw ADC and proper flag
    void setRawADCValue( double value );

    /// Set the value of the FIR corrections
    void setFIRValue( double value );

    /// Set the value of the Pedestal
    void setSubtractedPedestal( double value );

    /// Set the value of the Common Mode noise
    void setSubtractedCM( double value );

    /// Returns processing stage flag as integer
    int processingStageValue() const;

    /// Returns processing stage flag as name of the processing stage
    std::string processingStageName() const;

    /// Returns current ADC Value related with appropriate stage of processing
    double adcValue( int stage ) const;

    /// returns rawADCValue
    double adcValue() const;

    /// returns history of the processing
    int getProcessingList() const;

    /// signal to noise after common mode subtracted
    double signalToNoise() const;

    /// Retrieve const
    double rawNoise() const;

    /// Update
    void setRawNoise( double value );

    /// Retrieve const
    double noise() const;

    /// Update
    void setNoise( double value );

    friend std::ostream& operator<<( std::ostream& str, const VeloFullFPGADigit& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    short int        m_processingList;     ///< processing history
    double           m_rawADCValue;        ///<
    double           m_ADCFIRcorrection;   ///<
    double           m_subtractedPedestal; ///<
    double           m_subtractedCM;       ///<
    double           m_rawNoise;           ///<
    double           m_noise;              ///<
    processingStages m_flag;               ///<

  }; // class VeloFullFPGADigit

  /// Definition of Keyed Container for VeloFullFPGADigit
  typedef KeyedContainer<VeloFullFPGADigit, Containers::HashMap> VeloFullFPGADigits;

  inline std::ostream& operator<<( std::ostream& s, LHCb::VeloFullFPGADigit::processingStages e ) {
    switch ( e ) {
    case LHCb::VeloFullFPGADigit::RAW:
      return s << "RAW";
    case LHCb::VeloFullFPGADigit::FIR:
      return s << "FIR";
    case LHCb::VeloFullFPGADigit::PedestalSubtracted:
      return s << "PedestalSubtracted";
    case LHCb::VeloFullFPGADigit::CMSubtracted:
      return s << "CMSubtracted";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::VeloFullFPGADigit::processingStages";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::VeloFullFPGADigit::processingListMasks e ) {
    switch ( e ) {
    case LHCb::VeloFullFPGADigit::mRAW:
      return s << "mRAW";
    case LHCb::VeloFullFPGADigit::mFIR:
      return s << "mFIR";
    case LHCb::VeloFullFPGADigit::mPedestalSubtracted:
      return s << "mPedestalSubtracted";
    case LHCb::VeloFullFPGADigit::mCMSubtracted:
      return s << "mCMSubtracted";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::VeloFullFPGADigit::processingListMasks";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::VeloFullFPGADigit::clID() const { return LHCb::VeloFullFPGADigit::classID(); }

inline const CLID& LHCb::VeloFullFPGADigit::classID() { return CLID_VeloFullFPGADigit; }

inline std::ostream& LHCb::VeloFullFPGADigit::fillStream( std::ostream& s ) const {
  s << "{ "
    << "processingList :	" << m_processingList << std::endl
    << "rawADCValue :	" << (float)m_rawADCValue << std::endl
    << "ADCFIRcorrection :	" << (float)m_ADCFIRcorrection << std::endl
    << "subtractedPedestal :	" << (float)m_subtractedPedestal << std::endl
    << "subtractedCM :	" << (float)m_subtractedCM << std::endl
    << "rawNoise :	" << (float)m_rawNoise << std::endl
    << "noise :	" << (float)m_noise << std::endl
    << "flag :	" << m_flag << std::endl
    << " }";
  return s;
}

inline double LHCb::VeloFullFPGADigit::rawNoise() const { return m_rawNoise; }

inline void LHCb::VeloFullFPGADigit::setRawNoise( double value ) { m_rawNoise = value; }

inline double LHCb::VeloFullFPGADigit::noise() const { return m_noise; }

inline void LHCb::VeloFullFPGADigit::setNoise( double value ) { m_noise = value; }

inline LHCb::VeloChannelID LHCb::VeloFullFPGADigit::channelID() const { return key(); }

inline long LHCb::VeloFullFPGADigit::sensor() const { return key().sensor(); }

inline long LHCb::VeloFullFPGADigit::strip() const { return key().strip(); }

inline void LHCb::VeloFullFPGADigit::setRawADCValue( double value ) {

#ifdef __INTEL_COMPILER             // Disable ICC remark
#  pragma warning( disable : 2259 ) // non-pointer conversion from "int" to "short" may lose significant bits
#  pragma warning( push )
#endif
  m_processingList = m_processingList ^ processingListMasks::mRAW;
#ifdef __INTEL_COMPILER // End disable ICC remark
#  pragma warning( pop )
#endif
  m_flag        = processingStages::RAW;
  m_rawADCValue = value;
}

inline void LHCb::VeloFullFPGADigit::setFIRValue( double value ) {

#ifdef __INTEL_COMPILER             // Disable ICC remark
#  pragma warning( disable : 2259 ) // non-pointer conversion from "int" to "short" may lose significant bits
#  pragma warning( push )
#endif
  m_processingList = m_processingList ^ processingListMasks::mFIR;
#ifdef __INTEL_COMPILER // End disable ICC remark
#  pragma warning( pop )
#endif
  m_flag             = processingStages::FIR;
  m_ADCFIRcorrection = value;
}

inline void LHCb::VeloFullFPGADigit::setSubtractedPedestal( double value ) {

#ifdef __INTEL_COMPILER             // Disable ICC remark
#  pragma warning( disable : 2259 ) // non-pointer conversion from "int" to "short" may lose significant bits
#  pragma warning( push )
#endif
  m_processingList = m_processingList ^ processingListMasks::mPedestalSubtracted;
#ifdef __INTEL_COMPILER // End disable ICC remark
#  pragma warning( pop )
#endif
  m_flag               = processingStages::PedestalSubtracted;
  m_subtractedPedestal = value;
}

inline void LHCb::VeloFullFPGADigit::setSubtractedCM( double value ) {

#ifdef __INTEL_COMPILER             // Disable ICC remark
#  pragma warning( disable : 2259 ) // non-pointer conversion from "int" to "short" may lose significant bits
#  pragma warning( push )
#endif
  m_processingList = m_processingList ^ processingListMasks::mCMSubtracted;
#ifdef __INTEL_COMPILER // End disable ICC remark
#  pragma warning( pop )
#endif
  m_flag         = processingStages::CMSubtracted;
  m_subtractedCM = value;
}

inline int LHCb::VeloFullFPGADigit::processingStageValue() const { return m_flag; }

inline std::string LHCb::VeloFullFPGADigit::processingStageName() const {

  switch ( m_flag ) {
  case processingStages::RAW:
    return "RAW";
    break;
  case processingStages::FIR:
    return "FIR";
    break;
  case processingStages::PedestalSubtracted:
    return "PedestalSubtracted";
    break;
  case processingStages::CMSubtracted:
    return "CMSubtracted";
    break;
  default:
    return ( " " );
    break;
  }
}

inline double LHCb::VeloFullFPGADigit::adcValue( int stage ) const {

  switch ( stage ) {
  case processingStages::RAW:
    return m_rawADCValue;
    break;
  case processingStages::FIR:
    return m_ADCFIRcorrection;
    break;
  case processingStages::PedestalSubtracted:
    return m_subtractedPedestal;
    break;
  case processingStages::CMSubtracted:
    return m_subtractedCM;
    break;
  default:
    return 0.0;
    break;
  }
}

inline double LHCb::VeloFullFPGADigit::adcValue() const { return m_rawADCValue; }

inline int LHCb::VeloFullFPGADigit::getProcessingList() const {

  int temp = 0;
  if ( m_processingList & processingListMasks::mRAW ) temp += 1;
  if ( m_processingList & processingListMasks::mFIR ) temp += 10;
  if ( m_processingList & processingListMasks::mPedestalSubtracted ) temp += 100;
  if ( m_processingList & processingListMasks::mCMSubtracted ) temp += 1000;
  return temp;
}

inline double LHCb::VeloFullFPGADigit::signalToNoise() const {

  return ( noise() > 0. ) ? adcValue() / noise() : adcValue();
}
