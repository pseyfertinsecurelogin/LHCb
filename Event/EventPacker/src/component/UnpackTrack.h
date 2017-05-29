#ifndef UNPACKTRACK_H
#define UNPACKTRACK_H 1

#include <string>

#include "Event/PackedTrack.h"
#include "Event/Track.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

/** @class UnpackTrack UnpackTrack.h
 *
 *  Unpack the PackedTrack
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
class UnpackTrack : public GaudiAlgorithm
{

public:

  /// Standard constructor
  UnpackTrack( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:

  std::string m_inputName;  ///< Input location
  std::string m_outputName; ///< Output location
  bool m_alwaysOutput;      ///< Flag to turn on the creation of output, even when input is missing
  std::string m_ancestorFor;  ///< Container name for which ancestors shoudl be built
  std::string m_ancestorSource; ///< Container where ancestors are.

};

#endif // UNPACKTRACK_H
