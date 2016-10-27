#ifndef DUMPMCEVENTALG_H
#define DUMPMCEVENTALG_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

struct ICheckTool;

/** @class DumpMCEventAlg DumpMCEventAlg.h
 *  Dump contents of an MCEvent.
 *  Reimplements functionality previously in DumpEventExample
 *  Amount printed depends on OutputLevel:
 *    INFO: prints EventHeader and Collisions
 *   DEBUG: prints also first "NumberOfObjectsToPrint" MCVertices and
 *          MCParticles (default is 5)
 * VERBOSE: prints all MCVertices and MCParticles
 *
 *  @author Marco Cattaneo
 *  @date   2004-07-14
 */
class DumpMCEventAlg : public GaudiAlgorithm {
public:
  /// Standard constructor
  DumpMCEventAlg( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  unsigned int m_numObjects; ///< Number of objects to print
  ICheckTool*  m_checker = nullptr;    ///< Tool to check event integrity
};

#endif // DUMPMCEVENTALG_H
