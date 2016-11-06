#ifndef KERNEL_IEVENTTUPLETOOL_H
#define KERNEL_IEVENTTUPLETOOL_H  1

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

namespace Tuples{
  class Tuple;
}

// ==================================================
// ==================================================

/** @class IEventTupleTool
 * 
 * Fill some variables into a tuple.
 *
 * \sa IParticleTupleTool
 * 
 * @author Jeremie Borel
 * @date November 2007
 */
struct GAUDI_API IEventTupleTool : extend_interfaces<IAlgTool> {

  DeclareInterfaceID(IEventTupleTool, 2, 0);

  //! Fill the tuple. 
  virtual StatusCode fill( Tuples::Tuple& )=0;
};



#endif // KERNEL_IEVENTTUPLETOOL_H
