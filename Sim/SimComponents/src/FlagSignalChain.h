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
#ifndef FLAGSIGNALCHAIN_H
#define FLAGSIGNALCHAIN_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
// Interface
#include "MCInterfaces/IFlagSignalChain.h"


/** @class FlagSignalChain FlagSignalChain.h
 *
 *
 *  @author Gloria Corti
 *  @date   2015-07-23
 */
struct FlagSignalChain : extends<GaudiTool,IFlagSignalChain> {

  /// Standard constructor
  using base_class::base_class;

  /// Set flag if particle is from signal
  void setFromSignalFlag( const LHCb::MCParticle* mcPart ) override;

};
#endif // FLAGSIGNALCHAIN_H
