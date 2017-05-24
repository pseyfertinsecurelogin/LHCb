#ifndef FORCEDBDECAYTOOL_H
#define FORCEDBDECAYTOOL_H 1
// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"

//from Event
#include "Event/MCParticle.h"
#include "Event/GenHeader.h"

// Interface
#include "MCInterfaces/IForcedBDecayTool.h"

/** @class ForcedBDecayTool ForcedBDecayTool.h
 *
 *  v1.0
 *  @author Marco Musy
 *  @date   2006-10-02
 */
class ForcedBDecayTool : public extends<GaudiTool, IForcedBDecayTool> {
public:
  /// Standard constructor
  using base_class::base_class;

  const LHCb::MCParticle* forcedB(void) override;

private:

  LHCb::MCParticle* associatedofHEP(HepMC::GenParticle* ) ;

};
#endif // FORCEDBDECAYTOOL_H
