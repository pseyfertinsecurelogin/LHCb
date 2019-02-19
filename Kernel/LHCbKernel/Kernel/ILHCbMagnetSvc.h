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
#ifndef KERNEL_ILHCBMAGNETSVC_H
#define KERNEL_ILHCBMAGNETSVC_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/IMagneticFieldSvc.h"

#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

namespace LHCb {
  class MagneticFieldGrid;
}

/** @class ILHCbMagnetSvc ILHCbMagnetSvc.h Kernel/ILHCbMagnetSvc.h
 *  LHCb extensions to the Gaudi IMagneticFieldSvc interface
 *
 *  @author Marco Cattaneo
 *  @date   2008-07-18
 */
struct ILHCbMagnetSvc : extend_interfaces<IMagneticFieldSvc> {
  DeclareInterfaceID( ILHCbMagnetSvc, 2, 0 );

  virtual bool   useRealMap() const            = 0; ///< True if using measured map
  virtual bool   isDown() const                = 0; ///< True if the down polarity map is loaded
  virtual double signedRelativeCurrent() const = 0; ///< scale factor including polarity and current

  /** implementation of the IMagneticFieldSvc interface.
   * @param[in]  xyz Point at which magnetic field vector will be given
   * @param[out] fvec Magnectic field vector.
   * @return StatusCode SUCCESS if calculation was performed.
   */
  StatusCode fieldVector( const ROOT::Math::XYZPoint& xyz, ROOT::Math::XYZVector& fvec ) const override final {
    fvec = fieldVector( xyz );
    return StatusCode::SUCCESS;
  }

  /**
   * @param[in]  xyz Point at which magnetic field vector will be given
   * @return fvec Magnectic field vector.
   */
  virtual ROOT::Math::XYZVector fieldVector( const ROOT::Math::XYZPoint& xyz ) const = 0;

  /// Get direct access to the field grid
  virtual const LHCb::MagneticFieldGrid* fieldGrid() const = 0;
};
#endif // KERNEL_ILHCBMAGNETSVC_H
