#ifndef DEFTTESTALG_H
#define DEFTTESTALG_H 1

// Gaudi
#include "GaudiAlg/GaudiTupleAlg.h" 

// LHCb Event
#include "Event/MCHit.h"

// FTDet
#include "FTDet/DeFTDetector.h"


/** @class DeFTTestAlg DeFTTestAlg.h "FTDet/DeFTTestAlg.h"
 *
 *  A GaudiAlgorithm with solely test purposes, which calls (tests)
 *  the methods defined in the FT det. element classes.
 *
 *  @author Plamen Hopchev
 *  @date   2012-04-25
 */

class DeFTTestAlg : public GaudiTupleAlg { 

public:
  /// Standard constructor
  DeFTTestAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:

  /// A pointer to the top-level FT det. element
  DeFTDetector* m_deFT = nullptr;

  Gaudi::Property<std::string> m_mcHitsLocation{ this,
    "MCHitsLocation", LHCb::MCHitLocation::FT,
    "TES location of the FT MCHits" };
};
#endif // DEFTTESTALG_H
