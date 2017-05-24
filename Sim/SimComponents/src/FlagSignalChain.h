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
