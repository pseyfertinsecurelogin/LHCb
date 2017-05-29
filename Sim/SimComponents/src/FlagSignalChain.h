#ifndef FLAGSIGNALCHAIN_H
#define FLAGSIGNALCHAIN_H 1

#include <string>
#include <vector>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/extends.h"
// Interface
#include "MCInterfaces/IFlagSignalChain.h"

class IInterface;
namespace LHCb {
class MCParticle;
}  // namespace LHCb
struct IFlagSignalChain;


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
