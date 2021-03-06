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
#ifndef VELOERRORBANK_H
#define VELOERRORBANK_H 1

#include <exception>
#include <ostream>
#include <stdexcept>
#include <string>

// Include files
#if defined( __GNUC__ ) && __GNUC__ > 8
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wdeprecated-copy"
#endif
#include "GaudiKernel/KeyedObject.h"
#include "VeloEvent/EvtInfo.h"
#if defined( __GNUC__ ) && __GNUC__ > 8
#  pragma GCC diagnostic pop
#endif

#include "Tell1Kernel/VeloDecodeCore.h"

/** @class VeloErrorBank VeloErrorBank.h
 *
 *
 *  @author Tomasz Szumlak
 *  @date   2006-05-02
 */

namespace VeloErrorBankLocation {
  inline const std::string Default = "Raw/Velo/VeloErrorBank";
}

class VeloErrorBank : public KeyedObject<int> {
public:
  /// Standard constructor
  VeloErrorBank( const int numberOfTELL1 ) : KeyedObject<int>( numberOfTELL1 ), m_evtInfoData( numberOfTELL1 ) {}

  void               setEvtInfoSection( EvtInfo&& inInfo );
  void               setErrorInfoSection( VeloTELL1::allError& inSec );
  void               setErrorSources( VeloTELL1::dataVec& sources );
  void               setBankLength( const unsigned int bl );
  void               setBankType( const unsigned int bt );
  void               setBankVersion( const unsigned int bv );
  void               setMagicPattern( const unsigned int mp );
  unsigned int       bankLength() const;
  unsigned int       bankType() const;
  unsigned int       bankVersion() const;
  unsigned int       magicPattern() const;
  unsigned int       eventInformation( const int PPFPGA = 0 ) const;
  unsigned int       bankList( const int PPFPGA = 0 ) const;
  unsigned int       detectorID( const int PPFPGA = 0 ) const;
  unsigned int       bunchCounter( const int PPFPGA = 0 ) const;
  unsigned int       l0EventID( const int PPFPGA = 0 ) const;
  bool               isEmpty() const;
  const EvtInfo*     evtInfo() const;
  VeloTELL1::dataVec errorSources() const;
  unsigned int       clusterDataSectionLength( const int PPFPGA = 0 ) const;
  unsigned int       adcDataSectionLength( const int PPFPGA = 0 ) const;
  unsigned int       processInfo( const int PPFPGA = 0 ) const;
  unsigned int       PCN( const int PPFPGA = 0 ) const;
  unsigned int       numberOfCluster( const int PPFPGA = 0 ) const;
  unsigned int       errorBankLength( const int PPFPGA = 0 ) const;
  unsigned int       nzsBankLength( const int PPFPGA = 0 ) const;

private:
  EvtInfo             m_evtInfoData;
  VeloTELL1::allError m_errorInfoData;
  VeloTELL1::dataVec  m_errorSources;
  unsigned int        m_bankLength;
  unsigned int        m_bankType;
  unsigned int        m_bankVersion;
  unsigned int        m_magicPattern;
};
//
inline void VeloErrorBank::setEvtInfoSection( EvtInfo&& inInfo ) { m_evtInfoData = std::move( inInfo ); }
//
inline void VeloErrorBank::setBankLength( unsigned int bl ) { m_bankLength = bl; }
//
inline void VeloErrorBank::setBankType( unsigned int bt ) { m_bankType = bt; }
//
inline void VeloErrorBank::setBankVersion( unsigned int bv ) { m_bankVersion = bv; }
//
inline void VeloErrorBank::setMagicPattern( unsigned int mp ) { m_magicPattern = mp; }
//
inline unsigned int VeloErrorBank::bankVersion() const { return ( m_bankVersion ); }
//
inline unsigned int VeloErrorBank::bankLength() const { return ( m_bankLength ); }
//
inline unsigned int VeloErrorBank::bankType() const { return ( m_bankType ); }
//
inline unsigned int VeloErrorBank::magicPattern() const { return ( m_magicPattern ); }
//
inline void VeloErrorBank::setErrorInfoSection( VeloTELL1::allError& inSec ) { m_errorInfoData = inSec; }
//
inline void VeloErrorBank::setErrorSources( VeloTELL1::dataVec& sources ) { m_errorSources = sources; }
//
inline unsigned int VeloErrorBank::eventInformation( const int PPFPGA ) const {
  VeloTELL1::errorInfo anInfo = m_errorInfoData[PPFPGA];
  //
  return ( ( anInfo[0] >> VeloTELL1::bitShift24 ) & VeloTELL1::bitMask8 );
}
//
inline unsigned int VeloErrorBank::bankList( const int PPFPGA ) const {
  VeloTELL1::errorInfo anInfo = m_errorInfoData[PPFPGA];
  //
  return ( ( anInfo[0] >> VeloTELL1::bitShift16 ) & VeloTELL1::bitMask8 );
}
//
inline unsigned int VeloErrorBank::detectorID( const int PPFPGA ) const {
  VeloTELL1::errorInfo anInfo = m_errorInfoData[PPFPGA];
  //
  return ( ( anInfo[0] >> VeloTELL1::bitShift12 ) & VeloTELL1::bitMask4 );
}
//
inline unsigned int VeloErrorBank::bunchCounter( const int PPFPGA ) const {
  VeloTELL1::errorInfo anInfo = m_errorInfoData[PPFPGA];
  //
  return ( anInfo[0] & VeloTELL1::bitMask12 );
}
//
inline unsigned int VeloErrorBank::l0EventID( const int PPFPGA ) const {
  VeloTELL1::errorInfo anInfo = m_errorInfoData[PPFPGA];
  //
  return ( anInfo[1] );
}
//
inline bool VeloErrorBank::isEmpty() const { return ( ( m_errorSources.size() == 0 ) ? true : false ); }
//
inline const EvtInfo* VeloErrorBank::evtInfo() const { return ( &m_evtInfoData ); }
//
inline VeloTELL1::dataVec VeloErrorBank::errorSources() const { return ( m_errorSources ); }
//
inline unsigned int VeloErrorBank::clusterDataSectionLength( const int PPFPGA ) const {
  VeloTELL1::errorInfo anInfo = m_errorInfoData[PPFPGA];
  //
  return ( ( anInfo[2] >> VeloTELL1::bitShift16 ) & VeloTELL1::bitMask16 );
}
//
inline unsigned int VeloErrorBank::adcDataSectionLength( const int PPFPGA ) const {
  VeloTELL1::errorInfo anInfo = m_errorInfoData[PPFPGA];
  //
  return ( anInfo[2] & VeloTELL1::bitMask16 );
}
//
inline unsigned int VeloErrorBank::processInfo( const int PPFPGA ) const {
  VeloTELL1::errorInfo anInfo = m_errorInfoData[PPFPGA];
  //
  return ( ( anInfo[3] >> VeloTELL1::bitShift24 ) & VeloTELL1::bitMask8 );
}
//
inline unsigned int VeloErrorBank::PCN( const int PPFPGA ) const {
  VeloTELL1::errorInfo anInfo = m_errorInfoData[PPFPGA];
  //
  return ( ( anInfo[3] >> VeloTELL1::bitShift16 ) & VeloTELL1::bitMask8 );
}
//
inline unsigned int VeloErrorBank::numberOfCluster( const int PPFPGA ) const {
  VeloTELL1::errorInfo anInfo = m_errorInfoData[PPFPGA];
  //
  return ( anInfo[3] & VeloTELL1::bitMask16 );
}
//
inline unsigned int VeloErrorBank::errorBankLength( const int PPFPGA ) const {
  VeloTELL1::errorInfo anInfo = m_errorInfoData[PPFPGA];
  //
  unsigned int errorBankLength = 0;
  try {
    if ( ( ( anInfo[4] ) & VeloTELL1::bitMask8 ) == 0x00 ) {
      errorBankLength = ( anInfo[4] >> VeloTELL1::bitShift16 ) & VeloTELL1::bitMask16;
    } else {
      throw std::string( " ==> Error Bank data corruption at first marker!" );
    }
  } catch ( std::string& str ) { std::cout << ( str ) << std::endl; }
  //
  return ( errorBankLength );
}
inline unsigned int VeloErrorBank::nzsBankLength( const int PPFPGA ) const {
  VeloTELL1::errorInfo anInfo   = m_errorInfoData[PPFPGA];
  unsigned int         bankSize = 0;
  try {
    if ( ( anInfo[7] & VeloTELL1::bitMask8 ) == 0x03 ) {
      std::cout << " non-zero suppressed data bank length: ";
      bankSize = ( anInfo[7] >> VeloTELL1::bitShift16 ) & VeloTELL1::bitMask16;
    } else if ( ( anInfo[7] & VeloTELL1::bitMask8 ) == 0x04 ) {
      std::cout << " pedestal bank length: " << std::endl;
      bankSize = ( anInfo[7] >> VeloTELL1::bitShift16 ) & VeloTELL1::bitMask16;
    } else {
      throw std::string( " ==> No NZS data bank present in file" );
    }
  } catch ( std::string& str ) { std::cout << ( str ) << std::endl; }
  //
  return ( bankSize );
}
//
typedef KeyedContainer<VeloErrorBank, Containers::HashMap> VeloErrorBanks;
#endif // VELOERRORBANK_H
