#ifndef PACKRECVERTEX_H
#define PACKRECVERTEX_H 1

#include <string>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;


/** @class PackRecVertex PackRecVertex.h
 *
 *  Pack the RecVertex
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
class PackRecVertex : public GaudiAlgorithm
{

public:

  /// Standard constructor
  PackRecVertex( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:

  std::string m_inputName;
  std::string m_outputName;
  bool m_alwaysOutput;      ///< Flag to turn on the creation of output, even when input is missing
  bool m_deleteInput;       ///< Flag to turn on the removal of the input data after packing
  unsigned int m_version;   ///< Version schema number

};

#endif // PACKRECVERTEX_H
