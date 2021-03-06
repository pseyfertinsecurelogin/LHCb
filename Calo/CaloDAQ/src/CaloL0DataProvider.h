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
#ifndef CALOL0DATAPROVIDER_H
#define CALOL0DATAPROVIDER_H 1

// Include files
// from Gaudi
#include "CaloDAQ/ICaloL0DataProvider.h" // Interface
#include "CaloReadoutTool.h"
#include "GaudiAlg/GaudiTool.h"

/** @class CaloL0DataProvider CaloL0DataProvider.h
 *  Fast access to calorimeter L0 data from raw
 *
 *  @author Olivier Deschamps
 *  @date   2007-12-03
 */
class CaloL0DataProvider : public CaloReadoutTool, virtual public ICaloL0DataProvider {

public:
  /// Standard constructor
  CaloL0DataProvider( const std::string& type, const std::string& name, const IInterface* parent );

  StatusCode                         initialize() override;
  void                               clear() override;
  void                               cleanData( int feb ) override;
  int                                l0Adc( LHCb::CaloCellID id, int def = 0 ) override;
  unsigned int                       nTell1s() const override { return m_tell1s; }
  const CaloVector<LHCb::L0CaloAdc>& l0Adcs( int source = -1, bool clean = true ) override;
  const CaloVector<LHCb::L0CaloAdc>& l0Adcs( std::vector<int> sources, bool clean = true ) override;

protected:
  bool decodeCell( LHCb::CaloCellID id );
  bool decodeTell1( int tell1 );
  bool decodeBank( const LHCb::RawBank& bank );
  bool decodePrsTriggerBank( const LHCb::RawBank& bank );

private:
  LHCb::L0CaloAdc fillL0ADC( LHCb::CaloCellID id, int adc, int sourceID ) {
    LHCb::L0CaloAdc temp( id, adc );
    if ( 0 > m_adcs.index( id ) ) {
      m_adcs.addEntry( temp, id );
    } else {
      counter( "Duplicate L0ADC found" ) += 1;
      m_status->addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::DuplicateEntry );
    }
    return temp;
  }
  CaloVector<LHCb::L0CaloAdc> m_adcs;
  unsigned int                m_tell1s = 0;
};
#endif // CALOL0DATAPROVIDER_H
