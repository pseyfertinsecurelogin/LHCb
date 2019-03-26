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
#ifndef KERNEL_IBINTEGRATOR_H
#define KERNEL_IBINTEGRATOR_H 1

// Include files
// -------------
// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

/** @class IBIntegrator IBIntegrator.h Kernel/IBIntegrator.h
 *  Interface class for field integral tool.
 *
 *  @author Rutger Hierck
 *  @date   2002-05-28
 */
struct IBIntegrator : extend_interfaces<IAlgTool> {

  // Return the interface ID
  DeclareInterfaceID( IBIntegrator, 3, 0 );

  /// Get the z of center and the total Bdl
  virtual StatusCode calculateBdlAndCenter( const Gaudi::XYZPoint& beginPoint, const Gaudi::XYZPoint& endPoint,
                                            const double tX, const double tY, double& zCenter,
                                            Gaudi::XYZVector& Bdl ) const = 0;
};
#endif // KERNEL_IBINTEGRATOR_H
