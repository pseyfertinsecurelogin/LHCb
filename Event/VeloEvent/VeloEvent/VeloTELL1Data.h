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
#include "GaudiKernel/SerializeSTL.h"
#include "Tell1Kernel/VeloTell1Core.h"
#include <cassert>
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_VeloTELL1Data = 4005;

  // Namespace for locations in TDS
  namespace VeloTELL1DataLocation {
    inline const std::string ADCs              = "Raw/Velo/DecodedADC";
    inline const std::string Tell1ADCs         = "Raw/Velo/RealAndDummy";
    inline const std::string Pedestals         = "Raw/Velo/DecodedPed";
    inline const std::string Headers           = "Raw/Velo/DecodedHeaders";
    inline const std::string SimADCs           = "Raw/Velo/SimulatedADC";
    inline const std::string SimPeds           = "Raw/Velo/SimulatedPed";
    inline const std::string SubPeds           = "Raw/Velo/SubtractedPed";
    inline const std::string PedSubADCs        = "Raw/Velo/SubtractedPedADCs";
    inline const std::string FIRCorrectedADCs  = "Raw/Velo/FIRCorrected";
    inline const std::string BitLimitADCs      = "Raw/Velo/ADC8Bit";
    inline const std::string ReorderedADCs     = "Raw/Velo/ADCReordered";
    inline const std::string CMSuppressedADCs  = "Raw/Velo/ADCCMSuppressed";
    inline const std::string CMSNoise          = "Raw/Velo/CMSNoise";
    inline const std::string MCMSCorrectedADCs = "Raw/Velo/ADCMCMSCorrected";
  } // namespace VeloTELL1DataLocation

  /** @class VeloTELL1Data VeloTELL1Data.h
   *
   * The VeloTELL1Data is keyed data object, with sensor number as a key, and is
   * meant to store non-zero suppressed data (2304 ADC samples, 2048 pedestals or
   * 256 Headers) * * * The class is meant to store Raw Non-Zero Suppressed data
   * * from Velo sensor * *
   *
   * @author Tomasz Szumlak
   *
   */

  class VeloTELL1Data : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of VeloTELL1Data
    typedef KeyedContainer<VeloTELL1Data, Containers::HashMap> Container;

    /// Configuration constants depending on stored data type
    enum NZSDataType { VeloFull = 18, VeloPedestal = 30, Headers = 40 };
    /// Different bank content
    enum ALinkContent { HEADERS = 4, FPGA_ALINKS = 16, ADC_PER_ALINK = 32, SENSOR_ALINKS = 64 };
    /// Sensor types
    enum SensType { PHI_SENSOR = 1, R_SENSOR = 2, PILE_UP = 3 };

    /// constructor with key
    VeloTELL1Data( const int numberOfTELL1, const int dataType )
        : KeyedObject<int>( numberOfTELL1 )
        , m_decodedData()
        , m_dataType( dataType )
        , m_isReordered( false )
        , m_sensorType( 0 ) {}

    /// Default Constructor
    VeloTELL1Data() : m_decodedData(), m_dataType( 0 ), m_isReordered(), m_sensorType( 0 ) {}

    /// Default Destructor
    virtual ~VeloTELL1Data() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// overloaded subscripting operator
    VeloTELL1::ALinkPair operator[]( const unsigned int ALinkNumber ) const;

    /// Returns true if R type
    bool isR() const;

    /// Returns true if Pile-Up type
    bool isPileUp() const;

    /// Returns true if Phi type
    bool isPhi() const;

    /// return value of the charge deposited in a given channel
    signed int channelADC( const unsigned int channel ) const;

    /// return value of a pedestal
    signed int pedestalADC( const unsigned int channel ) const;

    /// returns adc value for a given strip - valid after reordering
    signed int stripADC( const unsigned int strip ) const;

    /// returns vector with all decoded samples
    std::vector<signed int>& data();

    /// fill the vector with adc Velo data, use restricted within TELL1 algorithms
    void addALink( const std::vector<signed int>& inVec );

    /// Retrieve const  Vector with decoded data
    const std::vector<signed int>& decodedData() const;

    /// Update  Vector with decoded data
    void setDecodedData( const std::vector<signed int>& value );

    /// Retrieve const  Check if the data stored are reorderd
    bool isReordered() const;

    /// Update  Check if the data stored are reorderd
    void setIsReordered( bool value );

    /// Retrieve const  Sensor Type
    unsigned int sensorType() const;

    /// Update  Sensor Type
    void setSensorType( unsigned int value );

    friend std::ostream& operator<<( std::ostream& str, const VeloTELL1Data& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Strips
    enum strips {
      DUMMY_CHANNELS  = 64,
      ERROR           = 99,
      INNER_STRIPS_1  = 170,
      INNER_STRIPS_2  = 171,
      INNER_SHIFT     = 192,
      OUTER_STRIPS_1  = 341,
      OUTER_STRIPS_2  = 342,
      PPFPGA_CHANNELS = 512,
      PPFPGA_SHIFT    = 576,
      INNER_STRIPS    = 683,
      SENSOR_STRIPS   = 2048,
      ALL_STRIPS      = 2304
    };

    std::vector<signed int> m_decodedData; ///< Vector with decoded data
    int                     m_dataType;    ///< Type of decoded data to be stored
    bool                    m_isReordered; ///< Check if the data stored are reorderd
    unsigned int            m_sensorType;  ///< Sensor Type

  }; // class VeloTELL1Data

  /// Definition of Keyed Container for VeloTELL1Data
  typedef KeyedContainer<VeloTELL1Data, Containers::HashMap> VeloTELL1Datas;

  inline std::ostream& operator<<( std::ostream& s, LHCb::VeloTELL1Data::NZSDataType e ) {
    switch ( e ) {
    case LHCb::VeloTELL1Data::VeloFull:
      return s << "VeloFull";
    case LHCb::VeloTELL1Data::VeloPedestal:
      return s << "VeloPedestal";
    case LHCb::VeloTELL1Data::Headers:
      return s << "Headers";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::VeloTELL1Data::NZSDataType";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::VeloTELL1Data::ALinkContent e ) {
    switch ( e ) {
    case LHCb::VeloTELL1Data::HEADERS:
      return s << "HEADERS";
    case LHCb::VeloTELL1Data::FPGA_ALINKS:
      return s << "FPGA_ALINKS";
    case LHCb::VeloTELL1Data::ADC_PER_ALINK:
      return s << "ADC_PER_ALINK";
    case LHCb::VeloTELL1Data::SENSOR_ALINKS:
      return s << "SENSOR_ALINKS";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::VeloTELL1Data::ALinkContent";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::VeloTELL1Data::SensType e ) {
    switch ( e ) {
    case LHCb::VeloTELL1Data::PHI_SENSOR:
      return s << "PHI_SENSOR";
    case LHCb::VeloTELL1Data::R_SENSOR:
      return s << "R_SENSOR";
    case LHCb::VeloTELL1Data::PILE_UP:
      return s << "PILE_UP";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::VeloTELL1Data::SensType";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::VeloTELL1Data::clID() const { return LHCb::VeloTELL1Data::classID(); }

inline const CLID& LHCb::VeloTELL1Data::classID() { return CLID_VeloTELL1Data; }

inline std::ostream& LHCb::VeloTELL1Data::fillStream( std::ostream& s ) const {
  char l_isReordered = ( m_isReordered ) ? 'T' : 'F';
  s << "{ "
    << "decodedData :	" << m_decodedData << std::endl
    << "dataType :	" << m_dataType << std::endl
    << "isReordered :	" << l_isReordered << std::endl
    << "sensorType :	" << m_sensorType << std::endl
    << " }";
  return s;
}

inline const std::vector<signed int>& LHCb::VeloTELL1Data::decodedData() const { return m_decodedData; }

inline void LHCb::VeloTELL1Data::setDecodedData( const std::vector<signed int>& value ) { m_decodedData = value; }

inline bool LHCb::VeloTELL1Data::isReordered() const { return m_isReordered; }

inline void LHCb::VeloTELL1Data::setIsReordered( bool value ) { m_isReordered = value; }

inline unsigned int LHCb::VeloTELL1Data::sensorType() const { return m_sensorType; }

inline void LHCb::VeloTELL1Data::setSensorType( unsigned int value ) { m_sensorType = value; }

inline VeloTELL1::ALinkPair LHCb::VeloTELL1Data::operator[]( const unsigned int ALinkNumber ) const {

  int ALinkBegin = 0, ALinkEnd = 0;
  // use it only before reordering
  if ( !m_isReordered ) {
    if ( ALinkNumber < ALinkContent::SENSOR_ALINKS ) {
      if ( m_dataType == NZSDataType::Headers ) {
        ALinkBegin = ALinkNumber * ALinkContent::HEADERS;
        ALinkEnd   = ALinkBegin + ALinkContent::HEADERS;
      } else if ( m_dataType == NZSDataType::VeloPedestal ) {
        ALinkBegin = ALinkNumber * ALinkContent::ADC_PER_ALINK;
        ALinkEnd   = ALinkBegin + ALinkContent::ADC_PER_ALINK;
      } else if ( m_dataType == NZSDataType::VeloFull ) {
        unsigned int processor = ALinkNumber / ALinkContent::FPGA_ALINKS;
        ALinkBegin             = ( ALinkNumber + 2 * processor ) * ALinkContent::ADC_PER_ALINK;
        ALinkEnd               = ALinkBegin + ALinkContent::ADC_PER_ALINK;
      }
    }
  }
  //
  return {m_decodedData.begin() + ALinkBegin, m_decodedData.begin() + ALinkEnd};
}

inline bool LHCb::VeloTELL1Data::isR() const { return m_isReordered && m_sensorType == SensType::R_SENSOR; }

inline bool LHCb::VeloTELL1Data::isPileUp() const { return m_isReordered && m_sensorType == SensType::PILE_UP; }

inline bool LHCb::VeloTELL1Data::isPhi() const { return m_isReordered && m_sensorType == SensType::PHI_SENSOR; }

inline signed int LHCb::VeloTELL1Data::channelADC( const unsigned int channel ) const {

  signed int adc = 0;
  if ( !m_isReordered && m_dataType == NZSDataType::VeloFull ) {
    if ( channel < SENSOR_STRIPS ) {
      unsigned int processor = channel / strips::PPFPGA_CHANNELS;
      unsigned int index     = channel + DUMMY_CHANNELS * processor;
      adc                    = m_decodedData[index];
    }
  }
  return adc;
}

inline signed int LHCb::VeloTELL1Data::pedestalADC( const unsigned int channel ) const {

  signed int ped = 0;
  if ( m_dataType == NZSDataType::VeloPedestal && channel < strips::SENSOR_STRIPS ) { ped = m_decodedData[channel]; }
  return ped;
}

inline signed int LHCb::VeloTELL1Data::stripADC( const unsigned int strip ) const {

  signed int   adc        = 0;
  unsigned int localStrip = 0, index = 0;
  if ( m_isReordered && strip < strips::SENSOR_STRIPS && m_dataType != NZSDataType::Headers ) {
    if ( m_sensorType == SensType::R_SENSOR || m_sensorType == SensType::PILE_UP ) {
      unsigned int processor = strip / strips::PPFPGA_CHANNELS;
      index                  = strip + DUMMY_CHANNELS * processor;
      adc                    = m_decodedData[index];
    } else if ( m_sensorType == SensType::PHI_SENSOR ) {
      if ( strip < strips::INNER_STRIPS ) {
        // pp_fpga 0
        if ( strip < strips::INNER_STRIPS_2 ) {
          adc = m_decodedData[strip];
          // pp_fpga 1
        } else if ( strip < 2 * strips::INNER_STRIPS_2 ) {
          localStrip = strip - strips::INNER_STRIPS_2;
          index      = localStrip + strips::PPFPGA_SHIFT;
          adc        = m_decodedData[index];
          // pp_fpga 2
        } else if ( strip < 2 * strips::INNER_STRIPS_2 + strips::INNER_STRIPS_1 ) {
          localStrip = strip - 2 * strips::INNER_STRIPS_2;
          index      = localStrip + 2 * strips::PPFPGA_SHIFT;
          adc        = m_decodedData[index];
          // pp_fpga 3
        } else if ( strip < 3 * strips::INNER_STRIPS_2 + strips::INNER_STRIPS_1 ) {
          localStrip = strip - ( 2 * strips::INNER_STRIPS_2 + strips::INNER_STRIPS_1 );
          index      = localStrip + 3 * strips::PPFPGA_SHIFT;
          adc        = m_decodedData[index];
        }
      } else {
        // pp_fpga 0
        if ( strip < strips::INNER_STRIPS + strips::OUTER_STRIPS_1 ) {
          localStrip = strip - strips::INNER_STRIPS;
          index      = localStrip + strips::INNER_SHIFT;
          adc        = m_decodedData[index];
          // pp_fpga 1
        } else if ( strip < strips::INNER_STRIPS + 2 * strips::OUTER_STRIPS_1 ) {
          localStrip = strip - ( strips::INNER_STRIPS + strips::OUTER_STRIPS_1 );
          index      = localStrip + strips::PPFPGA_SHIFT + strips::INNER_SHIFT;
          adc        = m_decodedData[index];
          // pp_fpga 2
        } else if ( strip < strips::INNER_STRIPS + 2 * strips::OUTER_STRIPS_1 + strips::OUTER_STRIPS_2 ) {
          localStrip = strip - ( strips::INNER_STRIPS + 2 * strips::OUTER_STRIPS_1 );
          index      = localStrip + 2 * strips::PPFPGA_SHIFT + strips::INNER_SHIFT;
          adc        = m_decodedData[index];
          // pp_fpga 3
        } else if ( strip < strips::INNER_STRIPS + 3 * strips::OUTER_STRIPS_1 + strips::OUTER_STRIPS_2 ) {
          localStrip = strip - ( strips::INNER_STRIPS + 2 * strips::OUTER_STRIPS_1 + strips::OUTER_STRIPS_2 );
          index      = localStrip + 3 * strips::PPFPGA_SHIFT + strips::INNER_SHIFT;
          adc        = m_decodedData[index];
        }
      }
    }
  } else {
    adc = ( -ERROR );
  }
  return ( adc );
}

inline std::vector<signed int>& LHCb::VeloTELL1Data::data() { return m_decodedData; }

inline void LHCb::VeloTELL1Data::addALink( const std::vector<signed int>& inVec ) {

  m_decodedData.insert( m_decodedData.end(), inVec.begin(), inVec.end() );
  assert( m_decodedData.size() <= strips::ALL_STRIPS );
}
