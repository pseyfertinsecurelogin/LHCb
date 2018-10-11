/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef CALOFUTURETRIGGERBITSFROMRAW_H 
#define CALOFUTURETRIGGERBITSFROMRAW_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "CaloFutureDAQ/ICaloFutureTriggerBitsFromRaw.h"            // Interface
#include "CaloFutureReadoutTool.h"


/** @class CaloFutureTriggerBitsFromRaw CaloFutureTriggerBitsFromRaw.h
 *  Decode the PRS bits. 
 *
 *  @author Olivier Callot
 *  @date   2005-01-06
 */
class CaloFutureTriggerBitsFromRaw : public CaloFutureReadoutTool , virtual public ICaloFutureTriggerBitsFromRaw {
public: 
  /// Standard constructor
  CaloFutureTriggerBitsFromRaw( const std::string& type, 
                          const std::string& name,
                          const IInterface* parent);

  StatusCode initialize() override;

  const LHCb::CaloFuture::FiredCells& prsCells( ) override; // get prs FiredCells
  const LHCb::CaloFuture::FiredCells& spdCells( ) override; // get spd FiredCells
  const LHCb::CaloFuture::PrsSpdFiredCells& prsSpdCells( ) override; // get all FiredCells
  const LHCb::CaloFuture::PrsSpdFiredCells& prsSpdCells(int source ) override; // get FiredCells for a single bank
  const LHCb::CaloFuture::PrsSpdFiredCells& prsSpdCells( const LHCb::RawBank& bank ) override; // get FiredCells for a single bank
  void clear() override;
  void cleanData(int feb) override;
  
private:
  bool getData(const LHCb::RawBank& bank );

  LHCb::CaloFuture::PrsSpdFiredCells m_data;  
};
#endif // CALOFUTURETRIGGERBITSFROMRAW_H
