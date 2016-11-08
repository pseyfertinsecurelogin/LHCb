#ifndef IFLAGSIGNALCHAIN_H 
#define IFLAGSIGNALCHAIN_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

namespace LHCb {
  class MCParticle;
}


/** @class IFlagSignalChain IFlagSignalChain.h
 *  
 *
 *  @author Gloria Corti
 *  @date   2015-07-23
 */
struct IFlagSignalChain : extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID( IFlagSignalChain, 2, 0 );

  /// Set flag if particle is signal or from signal
  virtual void setFromSignalFlag( const LHCb::MCParticle* mcPart ) = 0;

};
#endif // IFLAGSIGNALCHAIN_H
