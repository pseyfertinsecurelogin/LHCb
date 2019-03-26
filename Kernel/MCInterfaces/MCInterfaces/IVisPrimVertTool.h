/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef KERNEL_IVISPRIMVERTTOOL_H
#define KERNEL_IVISPRIMVERTTOOL_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

namespace LHCb {
  class MCVertex;
  class MCParticle;
} // namespace LHCb

/** @class IVisPrimVertTool IVisPrimVertTool.h
 *
 *  Tells if a primary MCVertex is visible and a little more
 *  The recommended way to use it is
 *
 *  @author Christian Jacoby (IPHE, UNIL, Lausanne)
 *  @date   2002-12-17
 */
struct IVisPrimVertTool : extend_interfaces<IAlgTool> {
  /// Retrieve interface ID
  DeclareInterfaceID( IVisPrimVertTool, 2, 0 );
  virtual long countVertices() const                                 = 0; ///< count number of visible PVs
  virtual bool isVisible( const LHCb::MCVertex* primvtx ) const      = 0; ///< is visible ?
  virtual bool isB( const LHCb::MCVertex* primvtx ) const            = 0; ///< has a B?
  virtual long countVisTracks( const LHCb::MCVertex* primvtx ) const = 0; ///< count visible tracks
};
#endif // KERNEL_IVISPRIMVERTTOOL_H
