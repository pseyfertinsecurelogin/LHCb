#ifndef COMPARERECVERTEX_H
#define COMPARERECVERTEX_H 1

#include <string>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;


/** @class CompareRecVertex CompareRecVertex.h
 *  Compare two containers of RecVertices
 *
 *  @author Olivier Callot
 *  @date   2008-11-17
 */
class CompareRecVertex : public GaudiAlgorithm
{

public:

  /// Standard constructor
  CompareRecVertex( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override;  ///< Algorithm execution

private:

  std::string m_inputName;
  std::string m_testName;

};

#endif // COMPARERECVERTEX_H
