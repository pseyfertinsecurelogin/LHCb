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
#ifndef CALODATAPROVIDER_H
#define CALODATAPROVIDER_H 1

// Include files
// from Gaudi
#include "CaloDAQ/ICaloDataProvider.h" // Interface
#include "CaloReadoutTool.h"
#include "GaudiAlg/GaudiTool.h"

/** @class CaloDataProvider CaloDataProvider.h
 *  Fast access to calorimeter data from raw
 *
 *  @author Olivier Deschamps
 *  @date   2007-02-27
 */
class CaloDataProvider : public extends<CaloReadoutTool, ICaloDataProvider> {

public:
  /// Standard constructor
  using extends::extends;

  StatusCode                         initialize() override;
  void                               clear() override;
  void                               cleanData( int feb ) override;
  int                                adc( LHCb::CaloCellID id, int def = 0 ) override;
  double                             digit( LHCb::CaloCellID id, double def = 0. ) override;
  unsigned int                       nTell1s() override { return m_tell1s; }
  const CaloVector<LHCb::CaloAdc>&   adcs( int source = -1, bool clean = true ) override;
  const CaloVector<LHCb::CaloAdc>&   adcs( std::vector<int> source, bool clean = true ) override;
  const CaloVector<LHCb::CaloDigit>& digits( int source = -1, bool clean = true ) override;
  const CaloVector<LHCb::CaloDigit>& digits( std::vector<int> sources, bool clean = true ) override;
  ICaloDataProvider::CaloAdcPair     adcRange() override {
    LHCb::CaloAdc min = ( m_minADC.cellID() == LHCb::CaloCellID() ) ? LHCb::CaloAdc( LHCb::CaloCellID(), 0 )
                                                                    : static_cast<LHCb::CaloAdc>( m_minADC );
    LHCb::CaloAdc max = ( m_maxADC.cellID() == LHCb::CaloCellID() ) ? LHCb::CaloAdc( LHCb::CaloCellID(), 0 )
                                                                    : static_cast<LHCb::CaloAdc>( m_maxADC );
    return {min, max};
  }
  ICaloDataProvider::CaloAdcPair pinRange() override {
    LHCb::CaloAdc min = ( m_minPinADC.cellID() == LHCb::CaloCellID() ) ? LHCb::CaloAdc( LHCb::CaloCellID(), 0 )
                                                                       : static_cast<LHCb::CaloAdc>( m_minPinADC );
    LHCb::CaloAdc max = ( m_maxPinADC.cellID() == LHCb::CaloCellID() ) ? LHCb::CaloAdc( LHCb::CaloCellID(), 0 )
                                                                       : static_cast<LHCb::CaloAdc>( m_maxPinADC );
    return {min, max};
  }

protected:
  void adc2digit();
  bool decodeCell( LHCb::CaloCellID id );
  bool decodeTell1( int tell1 );
  bool decodeBank( const LHCb::RawBank& bank );
  bool decodePrsTriggerBank( const LHCb::RawBank& bank );

private:
  CaloVector<LHCb::CaloAdc>   m_adcs;
  CaloVector<LHCb::CaloDigit> m_digits;
  unsigned int                m_tell1s    = 0;
  details::ADC                m_minADC    = {LHCb::CaloCellID(), 3840};
  details::ADC                m_minPinADC = {LHCb::CaloCellID(), 3840};
  details::ADC                m_maxADC    = {LHCb::CaloCellID(), -256};
  details::ADC                m_maxPinADC = {LHCb::CaloCellID(), -256};

  LHCb::CaloAdc fillAdc( details::ADC temp, int sourceID ) {
    if ( 0 > m_adcs.index( temp.cellID() ) ) {
      m_adcs.addEntry( temp, temp.cellID() );
    } else {
      counter( "Duplicate ADC found" ) += 1;
      m_status->addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::DuplicateEntry );
      return temp;
    }

    if ( temp.cellID().area() != CaloCellCode::PinArea ) {
      if ( temp.adc() < m_minADC.adc() ) m_minADC = temp;
      if ( temp.adc() > m_maxADC.adc() ) m_maxADC = temp;
    } else {
      if ( temp.adc() < m_minPinADC.adc() ) m_minPinADC = temp;
      if ( temp.adc() > m_maxPinADC.adc() ) m_maxPinADC = temp;
    }
    return temp;
  }
};
#endif // CALODATAPROVIDER_H
