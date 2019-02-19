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
#ifndef CALOTRIGGERADCSFROMRAW_H
#define CALOTRIGGERADCSFROMRAW_H 1

// Include files
// from Gaudi
#include "CaloDAQ/ICaloTriggerAdcsFromRaw.h" // Interface
#include "CaloReadoutTool.h"
#include "GaudiAlg/GaudiTool.h"

/** @class CaloTriggerAdcsFromRaw CaloTriggerAdcsFromRaw.h
 *
 *
 *  @author Olivier Callot
 *  @date   2005-01-05
 */
class CaloTriggerAdcsFromRaw final : public extends<CaloReadoutTool, ICaloTriggerAdcsFromRaw> {
public:
  /// Standard constructor
  CaloTriggerAdcsFromRaw( const std::string& type, const std::string& name, const IInterface* parent );

  StatusCode initialize() override;

  const std::vector<LHCb::L0CaloAdc>& adcs() override;
  const std::vector<LHCb::L0CaloAdc>& adcs( int source ) override;
  const std::vector<LHCb::L0CaloAdc>& adcs( const LHCb::RawBank& bank ) override;
  const std::vector<LHCb::L0CaloAdc>& pinAdcs() override;
  void                                clear() override;
  void                                cleanData( int feb ) override;

private:
  bool                         getData( const LHCb::RawBank& bank );
  std::vector<LHCb::L0CaloAdc> m_data;
  std::vector<LHCb::L0CaloAdc> m_pinData;
};
#endif // CALOTRIGGERADCSFROMRAW_H
