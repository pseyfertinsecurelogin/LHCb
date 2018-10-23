/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
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
