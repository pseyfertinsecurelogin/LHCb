#ifndef COMPARETRACK_H
#define COMPARETRACK_H 1

#include <string>

#include "Event/PackedTrack.h"
#include "Event/Track.h"
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

/** @class CompareTrack CompareTrack.h
 *  Compare two containers of Tracks
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
class CompareTrack : public GaudiAlgorithm
{

public:

  /// Standard constructor
  CompareTrack( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:

  std::string m_inputName;
  std::string m_testName;

};

#endif // COMPARETRACK_H
