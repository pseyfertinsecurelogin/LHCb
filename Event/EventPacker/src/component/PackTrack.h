#ifndef PACKTRACK_H
#define PACKTRACK_H 1

#include <string>

#include "Event/PackedTrack.h"
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

/** @class PackTrack PackTrack.h
 *
 *  Pack a track container
 *
 *  @author Olivier Callot
 *  @date   2008-11-12
 */
class PackTrack : public GaudiAlgorithm
{

public:

  /// Standard constructor
  PackTrack( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:

  std::string m_inputName;  ///< Input location
  std::string m_outputName; ///< Output location
  bool m_alwaysOutput;      ///< Flag to turn on the creation of output, even when input is missing
  bool m_deleteInput;       ///< Flag to turn on the removal of the input data after packing
  bool m_enableCheck;       ///< Flag to turn on automatic unpacking and checking of the output post-packing

};

#endif // PACKTRACK_H
