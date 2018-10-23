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
#ifndef CALOFUTURETRIGGERADCSFROMRAW_H 
#define CALOFUTURETRIGGERADCSFROMRAW_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "CaloFutureDAQ/ICaloFutureTriggerAdcsFromRaw.h"            // Interface
#include "CaloFutureReadoutTool.h"

/** @class CaloFutureTriggerAdcsFromRaw CaloFutureTriggerAdcsFromRaw.h
 *  
 *
 *  @author Olivier Callot
 *  @date   2005-01-05
 */
class CaloFutureTriggerAdcsFromRaw final 
: public extends<CaloFutureReadoutTool, ICaloFutureTriggerAdcsFromRaw> {
public: 
  /// Standard constructor
  CaloFutureTriggerAdcsFromRaw( const std::string& type, 
                      const std::string& name,
                      const IInterface* parent);

  StatusCode initialize() override;


  const std::vector<LHCb::L0CaloAdc>& adcs( ) override;
  const std::vector<LHCb::L0CaloAdc>& adcs( int source ) override;
  const std::vector<LHCb::L0CaloAdc>& adcs( const LHCb::RawBank& bank) override;
  const std::vector<LHCb::L0CaloAdc>& pinAdcs( ) override;
  void clear() override;
  void cleanData(int feb) override;

private:
  bool getData ( const LHCb::RawBank& bank );
  std::vector<LHCb::L0CaloAdc> m_data;
  std::vector<LHCb::L0CaloAdc> m_pinData;
};
#endif // CALOFUTURETRIGGERADCSFROMRAW_H
