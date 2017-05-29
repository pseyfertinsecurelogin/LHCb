#ifndef UNPACKRECVERTEX_H
#define UNPACKRECVERTEX_H 1

#include <string>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;


/** @class UnpackRecVertex UnpackRecVertex.h
 *  Unpack a RecVertex
 *
 *  @author Olivier Callot
 *  @date   2008-11-17
 */
class UnpackRecVertex : public GaudiAlgorithm
{

public:

  /// Standard constructor
  UnpackRecVertex( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:

  std::string m_inputName;
  std::string m_outputName;
  bool m_alwaysOutput;      ///< Flag to turn on the creation of output, even when input is missing
  std::string m_weightsLoc; ///< Location of the weights vector

};

#endif // UNPACKRECVERTEX_H
