#ifndef VELOALGORITHMS_IMCVELOFETYPE_H
#define VELOALGORITHMS_IMCVELOFETYPE_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"


/** @class IMCVeloFEType IMCVeloFEType.h VeloAlgorithms/IMCVeloFEType.h
 *
 *
 *  @author Tomasz Szumlak
 *  @date   2005-11-16
 */

namespace LHCb
{
  class MCVeloFE;
}

struct IMCVeloFEType : extend_interfaces<IAlgTool>
{

public:

  DeclareInterfaceID(IMCVeloFEType, 2, 0 );

  enum Types{ Signal=0, Noise, Other }; // Return the interface ID
  virtual void FEType(const LHCb::MCVeloFE* fe, int& feType)=0;

};
#endif // VELOALGORITHMS_IMCVELOFETYPE_H
