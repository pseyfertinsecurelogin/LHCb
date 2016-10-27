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
public:
  /// Standard constructor
  FlagSignalChain( const std::string& type,
                   const std::string& name,
                   const IInterface* parent);

  /// Set flag if particle is from signal
  void setFromSignalFlag( const LHCb::MCParticle* mcPart ) override;

};
#endif // FLAGSIGNALCHAIN_H
