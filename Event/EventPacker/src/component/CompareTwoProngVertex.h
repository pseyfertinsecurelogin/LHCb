#ifndef COMPARETWOPRONGVERTEX_H
#define COMPARETWOPRONGVERTEX_H 1

#include <string>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;


/** @class CompareTwoProngVertex CompareTwoProngVertex.h
 *
 *
 *  @author Olivier Callot
 *  @date   2009-01-21
 */
class CompareTwoProngVertex : public GaudiAlgorithm
{

public:

  /// Standard constructor
  CompareTwoProngVertex( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:

  std::string m_inputName;
  std::string m_testName;

};

#endif // COMPARETWOPRONGVERTEX_H
