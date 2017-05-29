#ifndef UNPACKTWOPRONGVERTEX_H
#define UNPACKTWOPRONGVERTEX_H 1

#include <string>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;


/** @class UnpackTwoProngVertex UnpackTwoProngVertex.h
 *  Unoack the Two Prong vertex class
 *
 *  @author Olivier Callot
 *  @date   2009-01-21
 */
class UnpackTwoProngVertex : public GaudiAlgorithm
{

public:

  /// Standard constructor
  UnpackTwoProngVertex( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:

  std::string m_inputName;
  std::string m_outputName;
  bool m_alwaysOutput; ///< Flag to turn on the creation of output, even when input is missing

};

#endif // UNPACKTWOPRONGVERTEX_H
